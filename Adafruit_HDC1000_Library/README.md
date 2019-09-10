# Adafruit HDC1000 Library [![Build Status](https://travis-ci.org/adafruit/Adafruit_HDC1000_Library.svg?branch=master)](https://travis-ci.org/adafruit/Adafruit_HDC1000_Library)

This is a library for the HDC1008 Humidity + Temp sensor

Designed specifically to work with the HDC1008 breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/2635

These displays use I2C to communicate, 2 pins are required to interface
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution

Check out the links above for our tutorials and wiring diagrams

To download. click the ZIP button in the top-middle navbar,
rename the uncompressed folder Adafruit_HDC1000.
Check that the Adafruit_HDC1000 folder contains Adafruit_HDC1000.cpp and Adafruit_HDC1000.h

Place the Adafruit_HDC1000 library folder your arduinosketchfolder/libraries/ folder.
You may need to create the libraries subfolder if its your first library. Restart the IDE.

We also have a great tutorial on Arduino library installation at:
http://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use

<!-- START COMPATIBILITY TABLE -->

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Atmega328 @ 16MHz  |      X       |             |            | 
Atmega328 @ 12MHz  |      X       |             |            | 
Atmega32u4 @ 16MHz |      X       |             |            | 
Atmega32u4 @ 8MHz  |      X       |             |            | 
ESP8266            |      X       |             |            | 
Atmega2560 @ 16MHz |      X       |             |            | Use SDA/SCL on pins 20 &amp; 21
ATSAM3X8E          |      X       |             |            | Use SDA/SCL on pins 20 &amp; 21
ATSAM21D           |      X       |             |            | Use SDA and SCL pins.
ATtiny85 @ 16MHz   |      X       |             |            | 
ATtiny85 @ 8MHz    |      X       |             |            | 
Intel Curie @ 32MHz |             |             |     X       | 
STM32F2            |             |             |     X       | 

  * ATmega328 @ 16MHz : Arduino UNO, Adafruit Pro Trinket 5V, Adafruit Metro 328, Adafruit Metro Mini
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Teensy 2.0
  * ATmega32u4 @ 8MHz : Adafruit Flora, Bluefruit Micro
  * ESP8266 : Adafruit Huzzah
  * ATmega2560 @ 16MHz : Arduino Mega
  * ATSAM3X8E : Arduino Due
  * ATSAM21D : Arduino Zero, M0 Pro
  * ATtiny85 @ 16MHz : Adafruit Trinket 5V
  * ATtiny85 @ 8MHz : Adafruit Gemma, Arduino Gemma, Adafruit Trinket 3V

<!-- END COMPATIBILITY TABLE -->
