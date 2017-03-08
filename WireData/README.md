# WireData Library

## Description

An [Arduino](https://en.wikipedia.org/wiki/Arduino)/[Wiring](https://en.wikipedia.org/wiki/Wiring_(development_platform)) library to simplify sending data via [I2C](https://en.wikipedia.org/wiki/I%C2%B2C) (aka [TWI](https://en.wikipedia.org/wiki/I%C2%B2C#Derivative_technologies)).

Oftentimes when you are writing some code that uses I2C, you will want to send multi-byte data over I2C protocol.  The Wire library has no concept of data types, and only permits sending data as bytes.

Nick Gammon came up with a fantastic C++ template to allow sending data, regardless of data types.

NOTE: This library bases its deconstruction of data types on the compiler's data type representations. Be careful!  If you use two drastically different compilers for slaves/masters, then you will have to reconstruct data accordingly.

## Example

### Master

```cpp
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
```

### Slave

```cpp
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
```
