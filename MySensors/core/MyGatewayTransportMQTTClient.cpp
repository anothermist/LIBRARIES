/*
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2018 Sensnology AB
* Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*/


// Topic structure: MY_MQTT_PUBLISH_TOPIC_PREFIX/NODE-ID/SENSOR-ID/CMD-TYPE/ACK-FLAG/SUB-TYPE

#include "MyGatewayTransport.h"

// housekeeping, remove for 3.0.0
#ifdef MY_ESP8266_SSID
#warning MY_ESP8266_SSID is deprecated, use MY_WIFI_SSID instead!
#define MY_WIFI_SSID MY_ESP8266_SSID
#undef MY_ESP8266_SSID // cleanup
#endif

#ifdef MY_ESP8266_PASSWORD
#warning MY_ESP8266_PASSWORD is deprecated, use MY_WIFI_PASSWORD instead!
#define MY_WIFI_PASSWORD MY_ESP8266_PASSWORD
#undef MY_ESP8266_PASSWORD // cleanup
#endif

#ifdef MY_ESP8266_BSSID
#warning MY_ESP8266_BSSID is deprecated, use MY_WIFI_BSSID instead!
#define MY_WIFI_BSSID MY_ESP8266_BSSID
#undef MY_ESP8266_BSSID // cleanup
#endif

#ifdef MY_ESP8266_HOSTNAME
#warning MY_ESP8266_HOSTNAME is deprecated, use MY_HOSTNAME instead!
#define MY_HOSTNAME MY_ESP8266_HOSTNAME
#undef MY_ESP8266_HOSTNAME // cleanup
#endif

#ifndef MY_WIFI_BSSID
#define MY_WIFI_BSSID NULL
#endif

#if defined MY_CONTROLLER_IP_ADDRESS
IPAddress _brokerIp(MY_CONTROLLER_IP_ADDRESS);
#endif

#if defined(MY_IP_ADDRESS)
IPAddress _MQTT_clientIp(MY_IP_ADDRESS);
#if defined(MY_IP_GATEWAY_ADDRESS)
IPAddress _gatewayIp(MY_IP_GATEWAY_ADDRESS);
#elif defined(MY_GATEWAY_ESP8266) || defined(MY_GATEWAY_ESP32)
// Assume the gateway will be the machine on the same network as the local IP
// but with last octet being '1'
IPAddress _gatewayIp(_MQTT_clientIp[0], _MQTT_clientIp[1], _MQTT_clientIp[2], 1);
#endif /* End of MY_IP_GATEWAY_ADDRESS */

#if defined(MY_IP_SUBNET_ADDRESS)
IPAddress _subnetIp(MY_IP_SUBNET_ADDRESS);
#elif defined(MY_GATEWAY_ESP8266) || defined(MY_GATEWAY_ESP32)
IPAddress _subnetIp(255, 255, 255, 0);
#endif /* End of MY_IP_SUBNET_ADDRESS */
#endif /* End of MY_IP_ADDRESS */

#if defined(MY_GATEWAY_ESP8266)  || defined(MY_GATEWAY_ESP32)
#define EthernetClient WiFiClient
#elif defined(MY_GATEWAY_LINUX)
// Nothing to do here
#else
uint8_t _MQTT_clientMAC[] = { MY_MAC_ADDRESS };
#endif /* End of MY_GATEWAY_ESP8266 */

#if defined(MY_GATEWAY_TINYGSM)
#if defined(MY_GSM_RX) && defined(MY_GSM_TX)
SoftwareSerial SerialAT(MY_GSM_RX, MY_GSM_TX);
#endif
static TinyGsm modem(SerialAT);
static TinyGsmClient _MQTT_gsmClient(modem);
static PubSubClient _MQTT_client(_MQTT_gsmClient);
#if defined(MY_GSM_BAUDRATE)
uint32_t rate = MY_GSM_BAUDRATE;
#else /* Else part of MY_GSM_BAUDRATE */
uint32_t rate = 0;
#endif /* End of MY_GSM_BAUDRATE */
#else /* Else part of MY_GATEWAY_TINYGSM */
static EthernetClient _MQTT_ethClient;
static PubSubClient _MQTT_client(_MQTT_ethClient);
#endif /* End of MY_GATEWAY_TINYGSM */


static bool _MQTT_connecting = true;
static bool _MQTT_available = false;
static MyMessage _MQTT_msg;

bool gatewayTransportSend(MyMessage &message)
{
	if (!_MQTT_client.connected()) {
		return false;
	}
	setIndication(INDICATION_GW_TX);
	char *topic = protocolFormatMQTTTopic(MY_MQTT_PUBLISH_TOPIC_PREFIX, message);
	GATEWAY_DEBUG(PSTR("GWT:TPS:TOPIC=%s,MSG SENT\n"), topic);
#if defined(MY_MQTT_CLIENT_PUBLISH_RETAIN)
	bool retain = mGetCommand(message) == C_SET ||
	              (mGetCommand(message) == C_INTERNAL && message.type == I_BATTERY_LEVEL);
#else
	bool retain = false;
#endif /* End of MY_MQTT_CLIENT_PUBLISH_RETAIN */
	return _MQTT_client.publish(topic, message.getString(_convBuffer), retain);
}

void incomingMQTT(char *topic, uint8_t *payload, unsigned int length)
{
	GATEWAY_DEBUG(PSTR("GWT:IMQ:TOPIC=%s, MSG RECEIVED\n"), topic);
	_MQTT_available = protocolMQTTParse(_MQTT_msg, topic, payload, length);
	setIndication(INDICATION_GW_RX);
}

bool reconnectMQTT(void)
{
	GATEWAY_DEBUG(PSTR("GWT:RMQ:MQTT RECONNECT\n"));
	// Attempt to connect
	if (_MQTT_client.connect(MY_MQTT_CLIENT_ID
#if defined(MY_MQTT_USER) && defined(MY_MQTT_PASSWORD)
	                         , MY_MQTT_USER, MY_MQTT_PASSWORD
#endif
	                        )) {
		GATEWAY_DEBUG(PSTR("GWT:RMQ:MQTT CONNECTED\n"));

		// Send presentation of locally attached sensors (and node if applicable)
		presentNode();

		// Once connected, publish an announcement...
		//_MQTT_client.publish("outTopic","hello world");
		// ... and resubscribe
		_MQTT_client.subscribe(MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "/+/+/+/+/+");
		return true;
	}
	return false;
}

bool gatewayTransportConnect(void)
{
#if defined(MY_GATEWAY_ESP8266) || defined(MY_GATEWAY_ESP32)
	while (WiFi.status() != WL_CONNECTED) {
		wait(500);
		GATEWAY_DEBUG(PSTR("GWT:TPC:CONNECTING...\n"));
	}
	GATEWAY_DEBUG(PSTR("GWT:TPC:IP=%s\n"),WiFi.localIP().toString().c_str());
#elif defined(MY_GATEWAY_LINUX)
#if defined(MY_IP_ADDRESS)
	_MQTT_ethClient.bind(_MQTT_clientIp);
#endif /* End of MY_IP_ADDRESS */
#elif defined(MY_GATEWAY_TINYGSM)
	GATEWAY_DEBUG(PSTR("GWT:TPC:IP=%s\n"), modem.getLocalIP().c_str());
#else
#if defined(MY_IP_ADDRESS)
	Ethernet.begin(_MQTT_clientMAC, _MQTT_clientIp);
#else /* Else part of MY_IP_ADDRESS */
	// Get IP address from DHCP
	if (!Ethernet.begin(_MQTT_clientMAC)) {
		GATEWAY_DEBUG(PSTR("!GWT:TPC:DHCP FAIL\n"));
		_MQTT_connecting = false;
		return false;
	}
#endif /* End of MY_IP_ADDRESS */
	GATEWAY_DEBUG(PSTR("GWT:TPC:IP=%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n"),
	              Ethernet.localIP()[0],
	              Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
	// give the Ethernet interface a second to initialize
	delay(1000);
#endif
	return true;
}

bool gatewayTransportInit(void)
{
	_MQTT_connecting = true;

#if defined(MY_GATEWAY_TINYGSM)

#if !defined(MY_GSM_BAUDRATE)
	rate = TinyGsmAutoBaud(SerialAT);
#endif /* End of MY_GSM_BAUDRATE */

	SerialAT.begin(rate);
	delay(3000);

	modem.restart();

#if defined(MY_GSM_PIN) && !defined(TINY_GSM_MODEM_ESP8266)
	modem.simUnlock(MY_GSM_PIN);
#endif /* End of MY_GSM_PIN */

#ifndef TINY_GSM_MODEM_ESP8266
	if (!modem.waitForNetwork()) {
		GATEWAY_DEBUG(PSTR("!GWT:TIN:ETH FAIL\n"));
		while (true);
	}
	GATEWAY_DEBUG(PSTR("GWT:TIN:ETH OK\n"));

	if (!modem.gprsConnect(MY_GSM_APN, MY_GSM_USR, MY_GSM_PSW)) {
		GATEWAY_DEBUG(PSTR("!GWT:TIN:ETH FAIL\n"));
		while (true);
	}
	GATEWAY_DEBUG(PSTR("GWT:TIN:ETH OK\n"));
	delay(1000);
#else /* Else part of TINY_GSM_MODEM_ESP8266 */
	if (!modem.networkConnect(MY_GSM_SSID, MY_GSM_PSW)) {
		GATEWAY_DEBUG(PSTR("!GWT:TIN:ETH FAIL\n"));
		while (true);
	}
	GATEWAY_DEBUG(PSTR("GWT:TIN:ETH OK\n"));
	delay(1000);
#endif /* End of TINY_GSM_MODEM_ESP8266 */

#endif /* End of MY_GATEWAY_TINYGSM */

#if defined(MY_CONTROLLER_IP_ADDRESS)
	_MQTT_client.setServer(_brokerIp, MY_PORT);
#else
	_MQTT_client.setServer(MY_CONTROLLER_URL_ADDRESS, MY_PORT);
#endif /* End of MY_CONTROLLER_IP_ADDRESS */

	_MQTT_client.setCallback(incomingMQTT);

#if defined(MY_GATEWAY_ESP8266)
	// Turn off access point
	WiFi.mode(WIFI_STA);
#if defined(MY_HOSTNAME)
	WiFi.hostname(MY_HOSTNAME);
#endif /* End of MY_ESP8266_HOSTNAME */
#if defined(MY_IP_ADDRESS)
	WiFi.config(_MQTT_clientIp, _gatewayIp, _subnetIp);
#endif /* End of MY_IP_ADDRESS */
#ifndef MY_WIFI_BSSID
#define MY_WIFI_BSSID NULL
#endif
	(void)WiFi.begin(MY_WIFI_SSID, MY_WIFI_PASSWORD, 0, MY_WIFI_BSSID);
#elif defined(MY_GATEWAY_ESP32)
	// Turn off access point
	WiFi.mode(WIFI_STA);
#if defined(MY_HOSTNAME)
	WiFi.setHostname(MY_HOSTNAME);
#endif /* End of MY_HOSTNAME */
#if defined(MY_IP_ADDRESS)
	WiFi.config(_MQTT_clientIp, _gatewayIp, _subnetIp);
#endif /* End of MY_IP_ADDRESS */
#ifndef MY_WIFI_BSSID
#define MY_WIFI_BSSID NULL
#endif
	(void)WiFi.begin(MY_WIFI_SSID, MY_WIFI_PASSWORD, 0, MY_WIFI_BSSID);
#endif

	gatewayTransportConnect();

	_MQTT_connecting = false;
	return true;
}

bool gatewayTransportAvailable(void)
{
	if (_MQTT_connecting) {
		return false;
	}
	//keep lease on dhcp address
	//Ethernet.maintain();
	if (!_MQTT_client.connected()) {
		//reinitialise client
		if (gatewayTransportConnect()) {
			reconnectMQTT();
		}
		return false;
	}
	_MQTT_client.loop();
	return _MQTT_available;
}

MyMessage & gatewayTransportReceive(void)
{
	// Return the last parsed message
	_MQTT_available = false;
	return _MQTT_msg;
}
