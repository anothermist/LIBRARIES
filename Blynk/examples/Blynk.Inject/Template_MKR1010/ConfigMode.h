/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

#include <WiFiClient.h>

WiFiServer server(WIFI_AP_CONFIG_PORT);

String urlDecode(const String& text);
String urlFindArg(const String& url, const String& arg);

enum Request {
  REQ_BOARD_INFO,
  REQ_ROOT,
  REQ_CONFIG,
  REQ_RESET,
  REQ_REBOOT
};

const char* config_form = R"html(
<!DOCTYPE HTML>
<html>
<head>
  <title>WiFi setup</title>
  <style>
  body {
    background-color: #fcfcfc;
    box-sizing: border-box;
  }
  body, input {
    font-family: Roboto, sans-serif;
    font-weight: 400;
    font-size: 16px;
  }
  .centered {
    position: fixed;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);

    padding: 20px;
    background-color: #ccc;
    border-radius: 4px;
  }
  td { padding:0 0 0 5px; }
  label { white-space:nowrap; }
  input { width: 20em; }
  input[name="port"] { width: 5em; }
  input[type="submit"], img { margin: auto; display: block; width: 30%; }
  </style>
</head> 
<body>
<div class="centered">
  <form method="get" action="config">
    <table>
    <tr><td><label for="ssid">WiFi SSID:</label></td>  <td><input type="text" name="ssid" length=64 required="required"></td></tr>
    <tr><td><label for="pass">Password:</label></td>   <td><input type="text" name="pass" length=64></td></tr>
    <tr><td><label for="blynk">Auth token:</label></td><td><input type="text" name="blynk" placeholder="a0b1c2d..." pattern="[a-zA-Z0-9]{32}" maxlength="32" required="required"></td></tr>
    <tr><td><label for="host">Host:</label></td>       <td><input type="text" name="host" length=64></td></tr>
    <tr><td><label for="port">Port:</label></td>       <td><input type="number" name="port" value="80" min="1" max="65535"></td></tr>
    </table><br/>
    <input type="submit" value="Apply">
  </form>
</div>
</body>
</html>
)html";

void restartMCU() {
  NVIC_SystemReset();
}

void enterConfigMode()
{
  byte mac[6];
  memset(mac, 0, sizeof(mac));
  WiFi.macAddress(mac);
  uint32_t chipId = *(uint32_t*)(mac+2) & 0xFFFFFF;

  randomSeed(chipId);
  const uint32_t unique = random(0xFFFFF);
  char ssidBuff[64];
  snprintf(ssidBuff, sizeof(ssidBuff), "%s-%05X", PRODUCT_WIFI_SSID, unique);

  WiFi.beginAP(ssidBuff);

  delay(500);
  IPAddress myIP = WiFi.localIP();
  DEBUG_PRINT(String("AP SSID: ") + ssidBuff);
  DEBUG_PRINT(String("AP IP:   ") + myIP[0] + "." + myIP[1] + "." + myIP[2] + "." + myIP[3]);

  server.begin();

  while(BlynkState::is(MODE_WAIT_CONFIG)) {
    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             // if you get a client,
      String currentLine = "";                // make a String to hold incoming data from the client
      String config_line = "";
      Request req = REQ_ROOT;
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              String responce = "200 OK";
              String content = "";
              String content_type = "text/html";

  switch(req) {
  case REQ_ROOT: {
    content = config_form;
  } break;
  case REQ_CONFIG: {
    String ssid = urlFindArg(config_line, "ssid");
    String ssidManual = urlFindArg(config_line, "ssidManual");
    String pass = urlFindArg(config_line, "pass");
    if (ssidManual != "") {
      ssid = ssidManual;
    }
    String token = urlFindArg(config_line, "blynk");
    String host  = urlFindArg(config_line, "host");
    String port  = urlFindArg(config_line, "port");

    DEBUG_PRINT(String("WiFi SSID: ") + ssid + " Pass: " + pass);
    DEBUG_PRINT(String("Blynk cloud: ") + token + " @ " + host + ":" + port);

    if (token.length() == 32 && ssid.length() > 0) {
      configStore.flagConfig = false;
      CopyString(ssid, configStore.wifiSSID);
      CopyString(pass, configStore.wifiPass);
      CopyString(token, configStore.cloudToken);
      if (host.length()) {
        CopyString(host,  configStore.cloudHost);
      }
      if (port.length()) {
        configStore.cloudPort = port.toInt();
      }

      content = R"json({"status":"ok","msg":"Configuration saved"})json";

      BlynkState::set(MODE_SWITCH_TO_STA);
    } else {
      DEBUG_PRINT("Configuration invalid");
      content = R"json({"status":"error","msg":"Configuration invalid"})json";
    }
    content_type = "application/json";
  } break;
  case REQ_BOARD_INFO: {
    char buff[256];
    snprintf(buff, sizeof(buff),
      R"json({"board":"%s","vendor":"%s","tmpl_id":"%s","fw_ver":"%s","hw_ver":"%s"})json",
      BOARD_NAME,
      BOARD_VENDOR,
      BOARD_TEMPLATE_ID,
      BOARD_FIRMWARE_VERSION,
      BOARD_HARDWARE_VERSION
    );
    content = buff;
    content_type = "application/json";
  } break;
  case REQ_RESET: {
    BlynkState::set(MODE_RESET_CONFIG);
    content = R"json({"status":"ok","msg":"Configuration reset"})json";
    content_type = "application/json";
  } break;
  case REQ_REBOOT: {
    restartMCU();
  } break;
  }

              client.println("HTTP/1.1 " + responce);
              client.println("Content-type:" + content_type);
              client.println();
              client.println(content);
              // break out of the while loop:
              break;
            } else {      // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
  
          if (currentLine.indexOf("GET /board_info.json") >= 0) {
            req = REQ_BOARD_INFO;
          } else if (currentLine.indexOf(" /config") >= 0) {
            req = REQ_CONFIG;
            int idx = currentLine.indexOf("?");
            config_line = "&" + currentLine.substring(idx+1, currentLine.lastIndexOf(' ')) + "&";
          } else if (currentLine.indexOf(" /reset") >= 0) {
            req = REQ_RESET;
          } else if (currentLine.indexOf(" /reboot") >= 0) {
            req = REQ_REBOOT;
          }
        }
      }
      client.stop();
    }
  }
}

String urlDecode(const String& text)
{
  String decoded = "";
  char temp[] = "0x00";
  unsigned int len = text.length();
  unsigned int i = 0;
  while (i < len) {
    char decodedChar;
    char encodedChar = text.charAt(i++);
    if ((encodedChar == '%') && (i + 1 < len)) {
      temp[2] = text.charAt(i++);
      temp[3] = text.charAt(i++);

      decodedChar = strtol(temp, NULL, 16);
    } else {
      if (encodedChar == '+') {
        decodedChar = ' ';
      } else {
        decodedChar = encodedChar;
      }
    }
    decoded += decodedChar;
  }
  return decoded;
}

String urlFindArg(const String& url, const String& arg)
{
  int s = url.indexOf("&" + arg + "=");
  if (s < 0)
    return "";
  int s_len = arg.length() + 2;
  int e = url.indexOf('&', s + s_len);
  return urlDecode(url.substring(s + s_len, e));
}

void enterConnectNet() {
  BlynkState::set(MODE_CONNECTING_NET);
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);

  WiFi.end();

  unsigned long timeoutMs = millis() + WIFI_NET_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) && (WiFi.status() != WL_CONNECTED))
  {
    WiFi.begin(configStore.wifiSSID, configStore.wifiPass);
    delay(100);
    if (!BlynkState::is(MODE_CONNECTING_NET)) {
      WiFi.disconnect();
      return;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    BlynkState::set(MODE_CONNECTING_CLOUD);
  } else {
    BlynkState::set(MODE_ERROR);
  }
}

void enterConnectCloud() {
  BlynkState::set(MODE_CONNECTING_CLOUD);

  Blynk.disconnect();
  Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
  Blynk.connect(0);

  unsigned long timeoutMs = millis() + WIFI_CLOUD_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) &&
        (Blynk.connected() == false))
  {
    Blynk.run();
    if (!BlynkState::is(MODE_CONNECTING_CLOUD)) {
      Blynk.disconnect();
      return;
    }
  }
  
  if (Blynk.connected()) {
    BlynkState::set(MODE_RUNNING);

    if (!configStore.flagConfig) {
      configStore.flagConfig = true;
      config_save();
      DEBUG_PRINT("Configuration stored to flash");
    }
  } else {
    BlynkState::set(MODE_ERROR);
  }
}

void enterSwitchToSTA() {
  BlynkState::set(MODE_SWITCH_TO_STA);

  DEBUG_PRINT("Switching to STA...");

  WiFi.end();
  delay(1000);

  BlynkState::set(MODE_CONNECTING_NET);
}

void enterError() {
  BlynkState::set(MODE_ERROR);
  
  unsigned long timeoutMs = millis() + 10000;
  while (timeoutMs > millis() || g_buttonPressed)
  {
    delay(10);
    if (!BlynkState::is(MODE_ERROR)) {
      return;
    }
  }
  DEBUG_PRINT("Restarting after error.");
  delay(10);

  restartMCU();
}

