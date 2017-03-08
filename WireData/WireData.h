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
|| |
|| | Based on I2C_Anything library by Nick Gammon
|| #
||
|| @notes
|| | Initially based on Atmel AVR 8 bit GCC compiler.
|| |
|| | IMPORTANT: This library bases its deconstruction of data types on the
|| | compiler's data type representations.  Be careful!  If you use two
|| | drastically different compilers for slaves/masters, then you will have to
|| | reconstruct data accordingly.
||
|| @license Please see license.txt.
||
*/

#include <Arduino.h>
#include <Wire.h>

/*
|| @usage
|| | e.g.
|| | float myNumber = 3.14159;
|| | uint16_t numberOfBytesSent = wireWriteData(myNumber);
|| #
*/
template <typename T> uint16_t wireWriteData(const T &value)
{
  const uint8_t *p = (const uint8_t *) &value;

  Wire.write(p, sizeof value);

  return sizeof value;
}


/*
|| @usage
|| | e.g.
|| | float myNumber;
|| | uint16_t numberOfBytesReceived = wireReadData(myNumber);
|| | Serial.println(myNumber);
|| #
*/
template <typename T> uint16_t wireReadData(T &value)
{
  uint8_t *p = (uint8_t *) &value;
  uint16_t i;

  for (i = 0; i < sizeof value; i++)
    *p++ = Wire.read();

  return i;
}

