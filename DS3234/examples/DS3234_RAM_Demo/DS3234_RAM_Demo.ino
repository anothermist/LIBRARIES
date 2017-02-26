// DS3234_RAM_Demo 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my DS3234 library to 
// read and write to the internal RAM of the DS3234.
// All output is sent to the serial-port at 115200 baud.
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

DS3234 rtc(8); 

uint8_t rambuffer[256];

void setup()
{
  // Initialize the rtc object
  rtc.begin();

  // Setup Serial connection
  Serial.begin(115200);
#ifdef __AVR_ATmega32U4__
  while (!Serial) {}; // wait for serial port to connect. Needed for Arduino Leonardo only
#endif
}

// Dump the rambuffer over the serial connection
void bufferDump(char st[])
{
  Serial.write(st);
  Serial.println("");
  for (int i=0; i<256; i++)
  {
    Serial.print("0x");
    if (rambuffer[i]<16)
      Serial.print("0");   
    Serial.print(rambuffer[i], HEX);
    Serial.print(" ");
    if (((i+1) % 32) == 0)
      Serial.println();
  }
  Serial.println("");
  Serial.println("--------------------------------------------------------");
}

// Print a comment over the serial connection
void comment(char st[])
{
  Serial.println("");
  Serial.print("---> ");
  Serial.write(st);
  Serial.println("");
  Serial.println("");
}

void loop()
{
  // Fill the rambuffer with data
  comment("Filling buffer with data...");
  for (int i=0; i<256; i++)
    rambuffer[i]=i;

  // Write the rambuffer to the DS3234 RAM
  comment("Writing buffer to RAM...");
  for (int i=0; i<256; i++)
    rtc.poke(i, rambuffer[i]);
  bufferDump("Buffer written to RAM...");
  
  // Clear the rambuffer
  comment("Clearing buffer...");
  for (int i=0; i<256; i++)
    rambuffer[i]=0;
  bufferDump("Cleared buffer...");

  // Write a single byte to the DS3234 RAM  
  comment("Setting byte 15 (0x0F) to value 160 (0xA0)...");
  rtc.poke(15,160);
  
  // Read the DS3234 RAM into the rambuffer
  comment("Reading buffer from RAM...");
  for (int i=0; i<256; i++)
    rambuffer[i]=rtc.peek(i);
  bufferDump("Buffer read from RAM...");

  // Read a single byte and send the result over the serial connection
  int temp;
  comment("Reading address 18 (0x12). This should return 18, 0x12.");
  temp = rtc.peek(18);
  Serial.print("Return value: ");
  Serial.print(temp, DEC);
  Serial.print(", 0x");
  Serial.println(temp, HEX);

  // The end!
  Serial.println("");
  Serial.println("");
  Serial.println("***** End of demo *****");
 
  // Do not do anything else
  while (1){};
}
