/*
   humidity.cpp

   Example on SmartEverything humidity / temperature sensor reading

   Created: 4/27/2015 10:32:11 PM
    Author: speirano
*/

#include <Wire.h>
#include <Arduino.h>

#include <HTS221.h>

// the setup function runs once when you press reset or power the board
void setup() {
  //Initiate the Wire library and join the I2C bus
  Wire.begin();
  pinMode(PIN_LED_13, OUTPUT);

  smeHumidity.begin();
  SerialUSB.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

  double data = 0;

  data = smeHumidity.readHumidity();
  SerialUSB.print("Humidity   : ");
  SerialUSB.print(data);
  SerialUSB.println(" %");

  data = smeHumidity.readTemperature();
  SerialUSB.print("Temperature: ");
  SerialUSB.print(data);
  SerialUSB.println(" celsius");

  digitalWrite(PIN_LED_13, LOW);
  delay(300);

  digitalWrite(PIN_LED_13, HIGH);       // turn the LED on
  delay(700);              // wait for a second

}
