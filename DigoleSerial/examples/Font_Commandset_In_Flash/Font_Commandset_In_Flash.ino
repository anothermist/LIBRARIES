/*********************
 * Digole Serial display module with touch screen demo
 * This demo will show you how to write data (u8g fonts, command set to onboard
 * flush memory, then use it.
 * The onboard flash memory is 2M bytes
 * NOTE: if you using UART mode, you need to disconnect the RX from Arduino
 * When you uploading your sketch
 **********************/
#define _Digole_Serial_SPI_  //To tell compiler compile the special communication only, 
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
//IMPORTANT: A 4.7K~20K PULL DOWN RESISTOR NEEDED BETWEEN SI(11) AND GROUND IN ORDER TO OPERATE TOUCH SCREEN
//AND FLASH MEMORY CORRECTLY
DigoleSerialDisp mydisp(8,9,10,11);  //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
//the flowing function can be put in DigoleSerialDisp class
void flashErase(unsigned long int addr,unsigned long int len);
void flashWriteL(unsigned long int addr,unsigned long int len, const unsigned char *data);
void flashWrite(unsigned long int addr,unsigned long int len, const unsigned char *data);
void flashReadStart(unsigned long int addr,unsigned long int len);
void setFlashFont(unsigned long int addr);
void runCommandSet(unsigned long int addr);
const unsigned char commandset[] PROGMEM="CLSF\x12TP\x03\x0F ESC\xf0\xc0\xffTTRun Command set in Flash Memory\x00 TTHELLO";
#include <sampledata.h>
unsigned char a,b;
const unsigned char DigoleStartScreen[] PROGMEM= {
4, 40,
67, 76, 13, 10, 69, 68, 73, 77, 49, 0, 16, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 217, 180, 181, 0, 0, 0, 74, 2, 75, 75, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 218,
181, 217, 181, 181, 0, 0, 74, 38, 2, 1, 1, 41, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 218, 254, 254, 177, 145, 0, 0, 74, 2, 111, 147, 73, 5, 1, 1, 110, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 218, 181, 217, 181, 145, 0, 0, 74, 38, 0, 0, 0, 0, 78, 37, 1, 74, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 183, 146, 0, 0, 145, 145, 104, 0, 0, 0, 74, 38, 0, 0,
0, 0, 0, 0, 74, 1, 37, 183, 0, 0, 0, 0, 0, 0, 0, 183, 73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 37, 0, 0, 0, 0, 0, 0, 0, 110, 37, 37, 0, 0, 0, 0, 0, 0, 0, 74, 1, 110, 0, 0,
0, 0, 0, 0, 0, 0, 74, 74, 0, 0, 0, 0, 0, 0, 0, 0, 110, 1, 37, 0, 0, 0, 0, 0, 110, 1, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 1, 110,
0, 0, 0, 0, 37, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 37, 0, 0, 0, 110, 37, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 70, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 110, 1, 110, 0, 0, 74, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 37, 0, 111, 37, 74, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 74, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 73, 1, 0, 42, 6, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146,
1, 74, 38, 2, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 37, 37, 6, 6, 151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 5, 6, 39, 2, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 1, 38, 38, 38, 115, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 74, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 1, 37, 75, 1, 147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147,
37, 37, 111, 1, 74, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 73, 1, 0, 0, 37, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 37, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 73, 37, 0, 0, 110, 37, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110, 37, 110, 0, 0, 0, 1, 37, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 74, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 110, 1, 74, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110, 33, 110,
0, 0, 0, 0, 0, 69, 0, 110, 0, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0, 0, 0, 0, 0, 0, 0, 142, 33, 73, 0, 0, 0, 0, 0, 0, 0, 37, 0, 146, 0, 0, 0, 0, 0, 0, 0, 74, 38, 0, 0,
0, 0, 0, 0, 0, 146, 32, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 0, 110, 0, 0, 0, 0, 0, 0, 74, 70, 0, 0, 0, 117, 81, 154, 0, 0, 73, 179, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 73, 0,
69, 74, 0, 0, 0, 0, 70, 38, 0, 0, 149, 153, 121, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 142, 37, 37, 37, 73, 146, 147, 74, 74, 0, 0, 149, 121, 121, 48, 4, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110, 37, 1, 33, 1, 70, 37, 0, 0, 117, 85, 84, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 74, 38, 1, 74, 0, 0,
4, 12, 12, 117, 0, 0, 0, 0, 0, 0, 0, 0, 13, 10, 83, 70, 120, 32, 83, 67, 241, 32, 69, 84, 80, 34, 41, 32, 84, 84, 87, 101, 108, 99, 111, 109, 101, 0, 32, 83, 70, 10, 32, 83, 67, 19, 32, 84, 82, 84,
32, 84, 82, 84, 32, 69, 84, 80, 8, 63, 84, 84, 68, 105, 103, 111, 108, 101, 32, 68, 105, 103, 105, 116, 97, 108, 32, 83, 111, 108, 117, 116, 105, 111, 110, 115, 0, 83, 67, 31, 32, 83, 70, 1, 32, 84, 82, 84, 84, 84,
89, 111, 117, 32, 119, 105, 108, 108, 32, 102, 105, 110, 100, 32, 105, 116, 39, 115, 32, 115, 111, 32, 101, 97, 115, 121, 32, 116, 111, 32, 117, 115, 101, 32, 97, 110, 100, 32, 102, 117, 110, 58, 0, 83, 67, 224, 84, 84, 119, 119,
119, 46, 68, 105, 103, 111, 108, 101, 46, 99, 111, 109, 0};
#define saddr 0x102500
unsigned char c;
void setup()
{
  mydisp.begin();
  mydisp.clearScreen();
  mydisp.setColor(0B11111100);  //256 color format RRRGGGBB
  mydisp.drawStr(7,10,"Test Flash Memory");
  mydisp.setColor(1);
  //  mydisp.setTrueColor(15,15,15);
#ifndef _Digole_Serial_UART_
  Serial.begin(9600);
#endif
  //Test font in flash memory
  mydisp.setFont(0);
  mydisp.flashErase(saddr,8588);  //erase 8588 bytes in flash to store font data, this function will use screen memory to buffer the 4096 erase block
  delay(400);
  mydisp.flashWrite(saddr, 8588, userfont3);  //write array d to flash memory start at 0, length=8588
  mydisp.setFlashFont(saddr);
  mydisp.drawStr(0,0,"abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUXYZ");
  delay(3000);
  mydisp.clearScreen();
  //Test command set in flash memory
  mydisp.flashErase(4096,4096);  //if you erase the whole blocks, the module will not use the screen buffer to save data
  mydisp.flashWrite(4096, sizeof(commandset),commandset);
  mydisp.runCommandSet(4096);  //run command set of: commandset
  delay(4000);
  mydisp.flashErase(0x1ff000,sizeof(DigoleStartScreen)+3);  //another command set to show Digole welcome screen
  mydisp.flashWrite(0x1ff000, sizeof(DigoleStartScreen), DigoleStartScreen);
  mydisp.runCommandSet(0x1ff000);  //run command set of: DigoleStartScreen
  mydisp.flashReadStart(8180,20);  //read flash memory, you can see what in flash memory begining from address of 1000 (commandset)
  for(int i=0;i<20;i++)
  {
    b=mydisp.read1();
    Serial.print(b);
    Serial.print('\n');
  }
  Serial.print("end");
  Serial.print('\n');
}
void loop()
{
}


