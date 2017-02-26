// DS3234_Serial_Easy 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my DS3234 library to 
// quickly send time and date information over a serial link
//
// Connections:
// ------------
//            |              Arduino             |         |        chipKit        |
// | DS3234   |  Uno  | Mega  |  Due  | Leonardo | Bobuino | Uno32 | uC32  | Max32 |
// +----------+-------+-------+-------+----------+---------+-------+-------+-------+
// | DIN      |  D11  |  D51  |  D75  |   D16    |   D11   |  D11  |  D11  |  D51  |  -> MOSI
// | DOUT     |  D12  |  D50  |  D74  |   D14    |   D12   |  D12  |  D12  |  D50  |  -> MISO
// | SCLK     |  D13  |  D52  |  D76  |   D15    |   D13   |  D13  |  D13  |  D52  |
// | CS       |        User selectable (Set to D8 by default in this demo)         |
// +----------+-------+-------+-------+----------+---------+-------+-------+-------+
//
// Note that the SPI pins are only available on the ICSP header
// on the Arduino Due and Arduino Leonardo.
// Boards with SPI Master/Slave Select jumpers should be set to the Master position.
//
// ICSP header pinout:      +---+---+
//                1 - MISO  | X | X |  2 - Vcc
//                          +---+---+
//                3 - SCLK  | X | X |  4 - MOSI
//                          +---+---+
//                5 - Reset | X | X |  6 - GND
//                          +---+---+
//

#include <DS3234.h>

// Init the DS3234
DS3234 rtc(8);

void setup()
{
  // Initialize the rtc object
  rtc.begin();
  
  // Setup Serial connection
  Serial.begin(115200);
#ifdef __AVR_ATmega32U4__
  while (!Serial) {}; // wait for serial port to connect. Needed for Arduino Leonardo only
#endif

  // The following lines can be uncommented and edited to set the time and date in the DS3234
//  rtc.setDOW(SATURDAY);        // Set Day-of-Week to SATURDAY
//  rtc.setTime(12, 0, 0);       // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(25, 1, 2014);    // Set the date to January 25th, 2014
}

void loop()
{
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.print(rtc.getTimeStr());
  Serial.print(" -- ");
  
  // Send temperature
  Serial.print("Internal temp: ");
  Serial.print(rtc.getTemp());
  Serial.println("C");
  // Wait one second before repeating :)
  delay (1000);
}
