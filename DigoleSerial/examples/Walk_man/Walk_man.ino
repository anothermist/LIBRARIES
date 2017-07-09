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
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_

//if you need up data to flash, you need to connect the read pin (eg. RX, SDI)
//un-comment the next line if your serial module has 2M bytes flash memory on board
//#define _Digole_2M_Flash_
#define Ver 34 //if firmware version is not V3.3, remove this line

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


const unsigned char myfont[] PROGMEM = {
0,32,60,0,0,60,0,0,0,0,32,52,0,0,0,60
,0,0,0,0,28,0,60,255,255,255,255,255,255,255,255,255
,255,255,255,255,255,255,30,59,236,32,0,0,0,3,128,0
,0,7,224,0,0,15,240,0,0,15,240,0,0,15,240,0
,0,15,240,0,0,7,240,0,0,7,224,0,0,15,224,0
,0,15,128,0,0,15,128,0,0,31,128,0,0,31,192,0
,0,63,224,0,0,63,224,0,0,63,224,0,0,63,224,0
,0,63,240,0,0,55,240,0,0,119,240,0,0,127,240,0
,0,127,240,0,0,127,240,0,0,127,240,0,0,255,252,0
,0,255,255,0,0,255,255,192,0,255,231,224,0,255,225,224
,0,255,225,224,0,255,240,0,1,223,240,0,3,223,248,0
,3,223,248,0,3,255,248,0,1,191,252,0,0,63,252,0
,0,63,252,0,0,127,254,0,0,126,254,0,0,126,126,0
,0,124,62,0,0,252,31,0,1,248,31,0,3,248,31,0
,7,240,31,0,7,224,31,0,7,192,15,0,15,192,15,0
,15,128,15,128,31,0,15,128,30,0,15,128,56,0,7,128
,248,0,7,128,240,0,3,128,240,0,3,192,248,0,3,192
,120,0,3,224,56,0,3,252,24,59,177,29,0,0,0,63
,0,0,63,128,0,127,128,0,127,128,0,127,128,0,63,192
,0,63,128,0,63,128,0,63,0,0,62,0,0,62,0,0
,127,0,0,127,128,0,255,128,0,255,128,0,255,192,0,255
,128,0,255,192,0,255,192,0,255,192,0,255,192,0,255,192
,0,255,192,0,255,192,0,255,192,0,255,192,0,255,192,0
,255,224,0,255,240,0,255,248,0,255,252,1,255,252,3,191
,216,3,255,224,3,223,224,1,159,224,0,31,224,0,31,224
,0,31,224,0,15,240,0,15,240,0,15,240,0,63,240,0
,127,240,0,255,240,1,253,240,3,249,240,7,241,240,7,193
,240,15,129,240,63,0,240,252,0,240,248,0,240,120,0,112
,56,0,112,56,0,112,56,0,120,28,0,124,0,0,127,16
,60,120,25,0,0,0,248,1,252,1,254,3,254,3,252,1
,254,1,252,1,252,1,252,3,240,3,240,3,240,7,248,7
,252,7,252,7,252,7,252,7,252,3,254,3,254,3,254,3
,254,3,252,3,252,3,252,3,252,3,252,7,252,7,252,15
,248,15,248,7,252,7,252,3,252,3,252,3,252,3,254,3
,254,1,254,1,254,1,254,1,255,1,255,0,255,1,254,3
,248,7,248,7,224,15,240,31,240,63,240,249,240,249,224,240
,224,240,224,112,224,120,224,56,240,24,248,0,254,23,59,177
,25,0,0,1,248,0,3,252,0,3,252,0,3,252,0,3
,252,0,3,252,0,1,252,0,1,252,0,1,248,0,3,224
,0,3,224,0,7,240,0,15,248,0,15,248,0,15,252,0
,15,252,0,15,252,0,15,252,0,15,252,0,15,252,0,15
,252,0,15,252,0,31,254,0,31,255,128,31,255,192,31,255
,240,31,251,252,31,248,124,31,240,60,63,248,0,63,252,0
,127,254,0,255,255,0,255,255,0,239,255,128,79,255,128,15
,255,128,15,223,192,15,143,224,31,135,224,31,3,224,31,3
,224,31,3,224,62,3,224,62,3,224,62,3,224,126,3,224
,124,3,224,124,3,192,124,3,192,124,3,192,56,1,192,56
,1,192,56,1,192,120,1,240,120,1,254,124,1,254,126,1
,240,127,128,128,28,60,240,28,0,0,0,8,0,0,0,63
,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127
,128,0,0,127,128,0,0,63,128,0,0,63,0,0,0,127
,0,0,0,124,0,0,0,252,0,0,0,254,0,0,0,254
,0,0,1,255,0,0,1,255,0,0,1,255,128,0,1,255
,128,0,3,255,128,0,3,255,128,0,3,255,128,0,3,255
,128,0,3,255,128,0,3,255,240,0,7,255,255,0,7,255
,255,128,7,255,63,128,15,255,7,128,15,254,0,0,15,254
,0,0,15,254,0,0,31,255,0,0,31,255,0,0,63,255
,128,0,63,255,128,0,59,255,192,0,3,255,192,0,3,255
,192,0,3,255,192,0,3,239,192,0,3,231,192,0,7,199
,224,0,7,195,224,0,7,195,224,0,15,129,240,0,31,129
,240,0,31,1,240,0,30,1,248,0,62,1,248,0,62,0
,248,0,62,0,252,0,60,0,124,0,60,0,60,0,56,0
,62,0,56,0,30,0,112,0,15,240,120,0,7,240,248,0
,7,240,252,0,7,128,126,0,7,0};
/*----
Free Universal fonts: 17,20 and 42 names:
  u8g_font_fur17r,u8g_font_fur20r,u8g_font_fur42n  //3146, 3976, 2194 bytes
Free Universal bold fonts 17,20,42 names
  u8g_font_fub17r,u8g_font_fub20r,u8g_font_fub42n  //3222, 4022, 2270 bytes
----*/
unsigned char dx[]={0,7,15,22,25};
#define Flash_Address 0x100000
void setup() {
  delay(4000);
  mydisp.begin();
  mydisp.displayStartScreen(0);  //disable welcome screen
  mydisp.displayConfig(0);  //disable show module config
  mydisp.clearScreen();
  unsigned char section=0;
  mydisp.clearScreen();
  mydisp.print("uploading user font now...#1 size:");
  mydisp.print(sizeof(myfont));
  #if defined(_Digole_2M_Flash_)
    mydisp.flashErase(Flash_Address,8192);
    mydisp.flashWrite(Flash_Address,sizeof(myfont),myfont);
    delay(500);
    mydisp.clearScreen();
    mydisp.setFlashFont(Flash_Address);
  #else
    delay(1000);            //this delay is very important, it will let module to clear the receive buffer
    mydisp.downloadUserFont(sizeof(myfont),myfont,section);
    delay(500);
    mydisp.clearScreen();
    mydisp.setFont(200+section);
  #endif
  mydisp.setColor(1);
//  mydisp.drawStr(2,3,"01234");
  mydisp.setMode('~');
  unsigned int x=0;
  unsigned char dx[]={0,7,15,22,25};
  while(1)
  {
    for(unsigned char i=0;i<5;i++)
    {
    mydisp.setTextPosAbs(x+dx[i],60);
    mydisp.print((char)(i+48));
    delay(200);
    mydisp.setTextPosAbs(x+dx[i],60);
    mydisp.print((char)(i+48));
    }
  x+=30;
  if(x>200)
      x=0;
   }
}
void loop() {
}

