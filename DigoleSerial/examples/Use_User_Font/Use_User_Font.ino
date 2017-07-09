/*********************
   http://www.digole.com/forum   communicate with us and other user
   http://www.digole.com    buyer Digole module
   This sample sketch is used to test new functions in V3.0 firmware:
 **********************/


//You have to change the following setting according to the Digole display

#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
#define LCDWidth 240
#//define FLASH_CHIP    //if the module equipt with 2MB or 4MB flash chip, use it, otherwise  use // to disable it
#define Ver 34           //if the version of firmware on display is V3.3 and newer, use it
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
//#define MONO  //if the screen panel is monochrome

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

#include "Demo_Data.h" //include images and fonts

unsigned char i, j;
int x, y;
const  unsigned char dx[] = {0, 7, 15, 22, 25};

void delay1(void) //for demo use, reset display position and clean the demo line
{
  delay(3000); //delay 1.5 seconds
  mydisp.setColor(WHITE);
}
void setup() {
  mydisp.begin(); //initiate serial port
  mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
  mydisp.setBgColor(BLACK); //set another back ground color
  mydisp.setColor(WHITE); //set fore ground color, 8bit format, for 24bit color format, use:setTrueColor(R,G,B)
  //mydisp.clearScreen(); //CLear screen
  //mydisp.drawStr(0,0,text);
  //delay1();
  mydisp.clearScreen(); //CLear screen
  /*-----use user font----*/
  //if module equipt with flash chip, all splash screen and user fonts stored in the chip(2MB), otherwise, stored in the MCU(16KB)
#if (Ver<33)&&defined(FLASH_CHIP)
  mydisp.print("This module only can use user fonts in flash chip.\n\r");
  delay1();
  delay1();
#else
  //in V3.3, predefined user font number 200,201,202,203 will occupy 2nd,3rd,4th,fifth 65KB in flash chip
  test_User_Font_Standard();
#endif
#ifdef FLASH_CHIP
  //you also can use flash chip to store user font freely on any version
  delay1();
  use_User_Font_In_Flash_Chip();
  delay(3000);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
void test_User_Font_Standard(void)
{
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Down load walk men motion as user font,please wait...");
  delay(1000); //This delay is very important, it will let the module clean the receiving buffer,then accept bulk data bellow
  mydisp.downloadUserFont(sizeof(myfont), myfont, 1); //download a user font: (font length, font address, #userfont), one time download needed
  delay(500);
  mydisp.clearScreen();
  mydisp.setFont(201);  //user #1 user font, there are 4 user fonts can be used:200 ~ 203
  mydisp.setMode('~');
  x = 0;
  for (j = 0; j < 7; j++)
  {
    for (unsigned char i = 0; i < 5; i++)
    {
      mydisp.setTextPosAbs(x + dx[i], 60);  //set text position as pixels
      mydisp.print((char)(i + 48)); //display the motion
      delay(200);
      mydisp.setTextPosBack();  //set text position to last one
      mydisp.print((char)(i + 48)); //dispear the last motion
    }
    x += 30;
    if (x > (LCDWidth - 30))
      break;
  }
  mydisp.setFont(0);
}
void use_User_Font_In_Flash_Chip(void)
{
  mydisp.cleanDrawWindow();
  mydisp.setFont(0);
  mydisp.drawStr(0, 0, "Download user font in flash chip and use it");
  mydisp.flashErase(600000, 8588); //erase 8588 bytes in flash to store font data, this function will use screen memory to buffer the 4096 erase block
  delay(400);
  mydisp.flashWrite(600000, 8588, userfont3);  //write array d to flash memory start at 0, length=8588
  mydisp.flashWrite(700000, 8588, userfont3);  //write array d to flash memory start at 0, length=8588
  delay(1000);
  mydisp.clearScreen();
  mydisp.setFlashFont(600000);  //use user font in flash chip from any address
  mydisp.drawStr(0, 0, "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUXYZ");
  mydisp.setFont(0);
}
