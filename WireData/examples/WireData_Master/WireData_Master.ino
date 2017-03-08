/*
||
|| @author         Brett Hagman <bhagman@wiring.org.co>
|| @url            http://wiring.org.co/
|| @url            http://loftypremises.com/
|| @contribution   Nick Gammon
|| @url            http://www.gammon.com.au/i2c
||
|| @description
|| |
|| | WireData - Send data via I2C/TWI, using the Wire library.
|| | I2C Master example.  Sends a float to and from a slave.
|| #
||
|| @license Please see license.txt.
||
*/

#include <Wire.h>
#include <WireData.h>

// N.B. I2C 7-bit addresses can only be in the range of 0x08 -> 0x77
const int slaveAddress = 0x08;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  delay(1000);  // Virtual coffee break.
}

void changeData()
{
  // Make up some random data to send to the I2C slave.
  float data = random(100) / 100.0;

  // Send data to the I2C slave.
  Wire.beginTransmission(slaveAddress);
  wireWriteData(data);
  Wire.endTransmission();
}


void loop()
{
  float data;

  // Request data from the I2C slave.
  Wire.requestFrom(slaveAddress, sizeof data);

  // Get and store the data.
  wireReadData(data);

  Serial.print("Data: ");
  Serial.println(data);
  
  delay(500);

  changeData();
}

