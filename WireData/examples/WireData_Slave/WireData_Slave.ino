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
|| | I2C Slave example.  Implement an I2C slave to receive and send a float.
|| #
||
|| @license Please see license.txt.
||
*/

#include <Wire.h>
#include <WireData.h>

const int myAddress = 0x08;

float myData = 3.14159;

void setup()
{
  Wire.begin(myAddress);
  Wire.onReceive(i2cReceive);
  Wire.onRequest(i2cRequest);
}

void loop()
{
  // Nothing to do - all the work is done in the Wire data handlers (onReceive, onRequest).
}

void i2cReceive(int byteCount)
{
  // Get and store the data.
  wireReadData(myData);
}

void i2cRequest()
{
  // FYI: The Atmel AVR 8 bit microcontroller provides for clock stretching while using
  // the ISR for the data request.  This is, by extension, happening here, since this
  // callback is called from the ISR.

  // Send the data.
  wireWriteData(myData);
}

