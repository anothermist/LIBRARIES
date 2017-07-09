/*********************
   http://www.digole.com/forum   communicate with us and other user
   http://www.digole.com    buyer Digole module
   We expose the modules's internal of 976 bytes of eeprom since firmware V3.3
   you can save and read your data using the eeprom
   if the COM header is 5 pin, the eeprom only work at I2C
 **********************/


//You have to change the following setting according to the Digole display

#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#define Ver 34           //if the version of firmware on display is V3.3 and newer, use this
//end changing

//define 8 bit color, see:https://en.wikipedia.org/wiki/8-bit_color
#define WHITE 0xFF
#define BLACK 0
#define RED 0xE0
#define GREEN 0x1A
#define BLUE 0x03

#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 38400); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire, '\x27'); //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8, 9, 10, 11); //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
unsigned char i,buf[30],j;
void delay1(void) //for demo use, reset display position and clean the demo line
{
  delay(3000); //delay 1.5 seconds
  mydisp.setColor(255);
}
void setup() {
  mydisp.begin(); //initiate serial port
  Serial.begin(9600);
  mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
  mydisp.setBgColor(0); //set another back ground color
  mydisp.setColor(255); //set fore ground color, 8bit format, for 24bit color format, use:setTrueColor(R,G,B)
  mydisp.clearScreen(); //CLear screen
  /*----Internal 976B EEPROM read/write (V3.3 and later)----*/
#if Ver>32  //the functions in this block only available on firmware version 3.3 or later
  //read, write data to internal 976B EEPROM
    mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Read 10 bytes of data from EEPROM, 976 bytes usable:");
  mydisp.readE2prom(1, 10); //ask module read 10 bytes of data start from address 1
//  delay(100);
  for (i = 0; i < 10; i++)
    buf[i]=mydisp.read1();
  for (i = 0; i < 10; i++)
  {
    mydisp.print(buf[i]); //read a byte of data from module EEPROM
    mydisp.print(" ");
  }
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Write 10 bytes in to EEPROM:");
  mydisp.setColor(RED | BLUE);
  for (i = 0; i < 10; i++)
  {
    buf[i] = random(256);
    mydisp.print(buf[i]);
    mydisp.print(" ");
  }
  mydisp.writeE2prom(1, 10, buf); //write 10 byte of data to address of 1 in EEPROM, the source data is in buf array
  delay1();
  mydisp.print("\n\n\rCheck what stored in EEPROM:");
  mydisp.readE2prom(1, 20);
  for (i = 0; i < 20; i++)
    buf[i]=mydisp.read1();
  mydisp.setColor(RED | BLUE);
  for (i = 0; i < 20; i++)
  {
    mydisp.print(buf[i]); //read a byte of data from module
    Serial.print(buf[i]);
    Serial.print(" ");
    mydisp.print(" ");
  }
  mydisp.print("End...");
#else
  mydisp.drawStr(0, 0, "Is the firmware version on the moudle is V3.3 or later? if it is, please uncomment:\n\r#define V33 \n\rline");
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
