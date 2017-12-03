/*
   This example is designed to be used with the Arduino 1.6.6+ IDE Serial Plotter.
   Blow on the sensor and you will see the humidity change in real time.

   From http://www.st.com/web/en/resource/technical/document/datasheet/DM00116291.pdf
   The HTS221 is an ultra compact sensor for relative humidity and temperature. It includes a
   sensing element and a mixed signal ASIC to provide the measurement information through
   digital serial interfaces.
   The sensing element consists of a polymer dielectric planar capacitor structure capable of
   detecting relative humidity variations.
*/
#include <Wire.h>
#include <HTS221.h>
#include <Arduino.h>

double humidity;
unsigned int pollingInterval = 1000; // in milliseconds

// The setup function runs once when you press reset or power the board
void setup() {
  pinMode(PIN_LED_13, OUTPUT);
  
  Wire.begin();
  if (!smeHumidity.begin()) { // Led On if there is an error initializing the sensor
        digitalWrite(PIN_LED_13, HIGH);
  }
  else {
    digitalWrite(PIN_LED_13, LOW);
  }
  SerialUSB.begin(115200);
}

// The loop function runs over and over again forever
void loop() {
  // The value is returned as a percentage of humidity (0 = dry desert, 100 = rain forest)
  humidity = smeHumidity.readHumidity();
  SerialUSB.println(humidity);
  delay(pollingInterval);
}

