/*
The upload speed for Start Screen and User font is about 7s/1K bytes
This demo code show you how to upload start screen and
 user fonts, the user fonts have 4 sections-0,1,2,3, each section has 4096 bytes
 you can upload your user font to any section, and you also can combine
 adjacent sections together for large font data.
 the font identify number for each sections are: 200 to 203.
 if you upload your font to section #2, use setFont(202) to use it.
 
 NOTE: once you uploaded start screen or user font, you don't need upload
 it again if unchanged, the adapter will store it in flash memory
 the DATA structure of user font is same as U8Glib
 */
#define Ver 33	//the 2 bytes sequence of data size are changed since firmware V3.3
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8,9,10);  //SPI:Pin 8: data, 9:clock, 10: SS, you can assign 255 to SS, and hard ground SS pin on module
#endif
#define LCDCol 16
#define LCDRow 2
#define LCDW 240
#include <sampledata.h>
void resetpos(void) //for demo use, reset display position and clean the demo line
{
  mydisp.setPrintPos(0, 1,_TEXT_);
  delay(3000); //delay 2 seconds
  mydisp.println("                "); //display space, use to clear the demo line
  mydisp.setPrintPos(0, 1,_TEXT_);
}
void setup() {
  mydisp.begin();
  mydisp.clearScreen();
  mydisp.print("downloading start screen now...(1024 bytes)");
  delay(500);            //this delay is very important, it will let module to clear the receive buffer
  mydisp.downloadStartScreen(1024,startscreenMV);
  delay(500);
  mydisp.clearScreen();
  mydisp.print("downloading 1st user font now...(1423 bytes)");
  delay(500);            //this delay is very important, it will let module to clear the receive buffer
  mydisp.downloadUserFont(1423,userfont1,0);
  delay(500);
  mydisp.clearScreen();
  mydisp.setFont(200);
  mydisp.drawStr(0,0,"0123456789ABCDEFGHIJK This is my 1st user font ZZZzzz");
  resetpos();
  mydisp.print("downloading my second user font now...(8588 bytes)");
  delay(500);            //this delay is very important, it will let module to clear the receive buffer
  mydisp.downloadUserFont(8588,userfont3,1);
  delay(1000);
  mydisp.clearScreen();
  mydisp.setFont(201);
  mydisp.drawStr(0,0,"my 2nd");
  mydisp.displayConfig(0);
}
void loop() {
}

