/*********************
   http://www.digole.com/forum   communicate with us and other user
   http://www.digole.com    buyer Digole module
   This sample sketch is used to test new functions in V3.0 firmware:
 **********************/


//You have to change the following setting according to the Digole display

#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, 
//#define FLASH_CHIP    //if the module equipt with 2MB or 4MB flash chip, use it, otherwise  use // to disable it
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

const unsigned char commandset[] PROGMEM = "CLSF\x12TP\x03\x0F ESC\xf0\xc0\xffTTRun Command set in Flash Memory\n\r\x00SC\xff TTHELLO\x00\xff";

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
#if Ver>32||defined(FLASH_CHIP)
  delay1();
  use_Command_Set();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
void use_Command_Set(void)
{
#ifdef FLASH_CHIP
#define comAddress 400000 //this address is in flash chip
#else
#define comAddress 2048 //this address is in MCU interal
#endif
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Use Command set in flash\n\r");
  mydisp.print("Write command set into flash\n\r");
  mydisp.flashErase(comAddress, sizeof(commandset)); //erase flash memory
  delay(1500); //wait a movement
  //the module will return a value of 17 to indicate flashWrite done, if you writing large data into flush, please use:flashWriteL()
  //you also can use flashWrite to write splash screen to address 0, or user font to flash memory, the user font address is different
  //depending on the module equipt flash chip or not
  mydisp.flashWrite(comAddress, sizeof(commandset), commandset);
  delay(1500); //wait done writing
  mydisp.cleanDrawWindow();
  mydisp.print("Run command set just downloaded");
  delay1();
  //run command set in flash chip
  mydisp.runCommandSet(comAddress); //command set must endded by an extra value of 255
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Use splash screen as command set\n\r");
  mydisp.print(sizeof(welNew));
  mydisp.flashErase(comAddress, sizeof(welNew)); //erase flash memory
  delay(500); //wait a movement
  mydisp.flashWrite(comAddress, sizeof(welNew), welNew);
  delay(2000); //wait done writing
  mydisp.cleanDrawWindow();
  mydisp.runCommandSet(comAddress);
}

