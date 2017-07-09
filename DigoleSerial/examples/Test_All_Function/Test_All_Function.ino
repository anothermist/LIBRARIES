/*********************
   http://www.digole.com/forum   communicate with us and other user
   http://www.digole.com    buyer Digole module
   This sample sketch is used to test new functions in V3.0 firmware:
   setBgColor----set background color
   setDrawWindow----set the drawing window
   resetDrawWindow--reset the drawing window to default
 **********************/


//You have to change the following setting according to the Digole display

#define LCDWidth 160  //define screen width,height
#define LCDHeight 128
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//#define TOUCH_SCEEN   //if the module equipt with touch screen, use this, otherwise use // to disable it
//#define FLASH_CHIP    //if the module equipt with 2MB or 4MB flash chip, use it, otherwise  use // to disable it
#define Ver 34           //if the version of firmware on display is V3.3 and newer, use this
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
//#define MONO  //if the screen panel is monochrome

//end changing

//define 8 bit color, see:https://en.wikipedia.org/wiki/8-bit_color
#define WHITE 0xFF
#define BLACK 0
#define RED 0xE0
#define GREEN 0x1A
#define BLUE 0x03
//define draw window
#define DW_X 5
#define DW_Y 8
#define DW_W (LCDWidth - 10)
#define DW_H (LCDHeight - 15)
#ifdef MONO
#define COLORRG 2
#define BGCOLOR 1
#else
#define BGCOLOR 256
#define COLORRG 256
#endif

#define basex 25
#define basey 25
#define R 20

#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
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

void test_Touch_Screen(void);
void test_Draw_Window(void);
void save_Data_In_Flash_Chip(void);
void draw_Pixels(void);
void draw_Lines(void);
void draw_Linetos(void);
void draw_Circles(void);
void draw_Rectangles(void);
void move_Area(void);
void display_Mono_Image(void);
void display_8bit_Image(void);
void draw_Filled_Rectangles_Fast(void);
void use_EEPROM(void);
void test_User_Font_Standard(void);
void use_User_Font_In_Flash_Chip(void);
void use_Command_Set(void);
void adjust_Backlight(void);

unsigned char i, j, fill;
int x, y, r, r1;
unsigned char buf[10], buf1[10];
const char smode[] = {'^', '!', '&', '|', 'C'};
const char *smodename[] = { "XOR", "NOT", "AND",  "OR ", "COPY"};
const  unsigned char dx[] = {0, 7, 15, 22, 25};
const unsigned char intfonts[] = {0, 6, 10, 18, 51, 120, 123};
//const char text[] PROGMEM="You need to change the setting for your module at the top of sketch!";
void delay1(void) //for demo use, reset display position and clean the demo line
{
  delay(1500); //delay 1.5 seconds
  mydisp.setColor(WHITE);
}
void setup() {
  mydisp.begin(); //initiate serial port
  //mydisp.setSPIMode(0); //set SPI mode in V3.4, the available mode are 0,1,2,3
  //  mydisp.displayConfig(1);
  //  mydisp.enableCursor(); //disable cursor, enable cursore use: enableCursor();
  /*-----drawing functions  -----*/
  mydisp.setBgColor(0); //set another back ground color
  mydisp.setColor(WHITE); //set fore ground color, 8bit format, for 24bit color format, use:setTrueColor(R,G,B)
  mydisp.clearScreen(); //CLear screen
//display string, \n move cursor to next line,\r move cursor the beginning of current line
  mydisp.print("Clear screen,\nNext Line.\r-Return-\n\rNew Line...");
  delay1();
  mydisp.setBgColor(random(BGCOLOR)); //set back ground color
  mydisp.clearScreen(); //CLear screen use back ground color
  mydisp.setBgColor(random(BGCOLOR)); //set another back ground color, 8bit format
  test_Draw_Window();
  mydisp.setRotation(0);
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setMode('C');  //set pixels operation mode as "COPY"
  mydisp.drawStr(0, 0, "use NOT or XOR mode to highlight an area with color GREEN:");
  for (j = 0; j < 2; j++)
  {
    mydisp.setColor(WHITE);
    mydisp.setMode('C');  //set pixels operation mode as "COPY"
    mydisp.setPrintPos(DW_W / 2 - 20, DW_H / 2 + 7, _GRAPH_);
    mydisp.print(smodename[j]); //print operation mode name
    mydisp.setColor(GREEN);
    mydisp.setMode(smode[j]); //set pixels operation mode
    for (i = 0; i < 6; i++)
    {
      mydisp.drawBox(DW_W / 2 - 20, DW_H / 2 - 7, 25, 15);
      delay(500);
    }
  }
  delay1();
  draw_Pixels();
  delay1();
  draw_Lines();
  delay1();
  draw_Linetos();
  delay1();
  draw_Circles();
  delay1();
  draw_Rectangles();
  delay1();
  draw_Filled_Rectangles_Fast();
  mydisp.setRotation(0);
  mydisp.setDrawWindow(DW_X, DW_Y, DW_W, DW_H);
  //move area
  delay1();
  move_Area();
  //show image
  delay1();
  display_Mono_Image();
  mydisp.undoRotation();  //same with setRotation(0);
  mydisp.resetDrawWindow(); //set the draw window to full screen

  delay1();
  display_8bit_Image();

  /*----Internal 976B EEPROM read/write (V3.3 and later)----*/
#if Ver>32  //the functions in this block only available on firmware version 3.3 or later
#if defined(_Digole_Serial_UART_)||defined(_Digole_Serial_I2C_) //only work when using UART or I2C, if you want
  //read, write data to internal 976B EEPROM
  use_EEPROM();
  delay(5000);
#endif  //end EEPROM R/W in 976B
#endif

  /*------Splash screen------*/
#ifdef FLASH_CHIP
#if Ver>32  //the functions in this block only available on firmware version 3.3 or later
  //in V3.3, splash screen stored since address 0, the data length unlimited, but since the address:65536, the flash space are
  //  mydisp.downloadStartScreen(sizeof(welNew),welNew);
#else
  //in early version, you can use command set stored in flash chip to do splash screen
  //that means you need send the run command set command to module when power up
#endif
#else
  //  mydisp.downloadStartScreen(sizeof(welNew),welNew);
#endif

  /*-----use user font----*/
  //if module equipt with flash chip, all splash screen and user fonts stored in the chip(2MB), otherwise, stored in the MCU(16KB)
#if Ver>32||!defined(FLASH_CHIP)
  //in V3.3, predefined user font number 200,201,202,203 will occupy 2nd,3rd,4th,fifth 65KB in flash chip
  delay1();
  test_User_Font_Standard();
#endif
#ifdef FLASH_CHIP
  //you also can use flash chip to store user font freely on any version
  delay1();
  use_User_Font_In_Flash_Chip();
  delay(3000);
#endif

  /*------command set-------*/
#if Ver>32||defined(FLASH_CHIP)
  delay1();
  use_Command_Set();
#endif

  /*-----use flash chip to store data ------*/
#ifdef FLASH_CHIP
  //you can write and read data to/from flash chip, the splash screen and 4 user font also stored in chip, each occupy 64KB
  //so, the address from 0 to 327680 are used for splash screen and user font, but you also can use these space
  //if splash screen or user font didn't fullfill that
  //you also can run command set stored in flash chip.
  delay1();
  save_Data_In_Flash_Chip();
  delay(5000);  //give you time to compare datas
#endif

  /*----Touch screen function ----*/
#ifdef TOUCH_SCEEN
  test_Touch_Screen();
  //  x=mydisp.readBattery(); //this function is read the battery voltage connected on Vbat pin
  //  x=mydisp.readAux(); //this function is read the voltage connected on Aux pin
  //  x=mydisp.readTemperature(); //this function is read the chip temperature
#endif

  /*-----Backlight control-----*/
  /*----Test backlight brightness (not for firmware 3.0 and older)----*/
  delay1();
  adjust_Backlight();
  /*------Set Uart Baud------*/
  //in order to set Uart Baud same with Arduino, we suggest you set the Baud at the beginning code, but you also
  //can change the Baud by send "SB" follow the rate to moudle
  //  Serial->print("SB");
  //  Serial->println(newBaud);
  //  delay(100);
  //  Serial->begin(newBaud);

  /*------Change I2C address------*/
  //mydisp.setI2CAddress(newI2Caddress);

  /*------Change splash screen and display serial config setting ----*/
  //mydisp.displayConfig(0);  //0=disable, 1=enable display the serial port configuration show when power up
  //mydisp.displayStartScreen(0); //0=disable, 1=enable display flush screen when power up


#ifdef MONO
  //set contrast
  //set instant flush for mono
#endif
  /*------Power management------*/
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Turn off backlight");
  delay1();
  mydisp.backLightOff();
  delay1();
  mydisp.backLightOn();
  mydisp.drawStr(0, 0, "Backlight is on now");
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Turn screen off");
  delay1();
  mydisp.screenOnOff(0);  //turn screen off
  delay1();
  mydisp.screenOnOff(1);  //turn screen on
  mydisp.drawStr(0, 0, "Turn screen on ");
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Turn CPU off, you can measure the module current");
  mydisp.cpuOff();  //turn cpu off
  delay1();
  delay1();
  for (i = 0; i < 10; i++)
    mydisp.write(0);  //write some dummy data to wake up cpu
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "CPU is on now");
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Shut down whole module");
  delay1();
  mydisp.moduleOff();   //put whole module in sleep: Back light off, screen in sleep, MCU in sleep
  delay1();
  for (i = 0; i < 10; i++)
    mydisp.write(0);  //write some dummy data to wake up cpu
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Module resumed");
}
void loop()
{
}
void font_in_flash_chip(void) {

}
void test_Touch_Screen(void) {
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(4, 0, "Test touch screen");
  mydisp.setTextPosAbs(LCDWidth / 2 - 20, LCDHeight - 30);
  mydisp.print("O K");
  //  mydisp.calibrateTouchScreen();  //calibrate touch screen, the module will restart after done calibration
  mydisp.setMode('C');
  while (1) {
    mydisp.readTouchScreen(&x,&y); //read a touch screen touched, another function is: readClick(), this will waiting a click
    mydisp.setPrintPos(LCDWidth / 2 - 40, 25, 1);
    mydisp.print(x);
    mydisp.print(',');
    mydisp.print(y);
    mydisp.print("    ");
    mydisp.drawPixel(x, y);
    if (x > (LCDWidth / 2 - 22) && x < (LCDWidth / 2 + 10) && y > (LCDHeight - 50) && y < (LCDHeight - 25)) //if you click OK, then exit
      break;
  }
}
void test_Draw_Window(void) {
  mydisp.setRotation(0);  //change screen direction to 1, screen width and screen height exchanged
  mydisp.setDrawWindow(DW_X, DW_Y, DW_W, DW_H); //set a draw window left/top:5,8, Height=LCDHeight-15, width=LCDWidth-10
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  for (j = 0; j < 5; j++)
  {
    mydisp.setRotation(0);
    mydisp.setFont(18); //set font to internal font:18, available internal font: 6,10,18,51,120. 200,201,202 and 203 are user font
    mydisp.setMode('C');  //set pixels operation mode as "COPY"
    //set display position in draw window as pixels, change _GRAPH_ to _TEXT_ will set position according charactor size.
    //_GRAPH_ equal 1, _TEXT_ equal 0
    mydisp.setPrintPos(DW_W / 2 - 20, DW_H / 2 + 7, _GRAPH_);
    mydisp.setColor(WHITE);
    mydisp.print(smodename[j]); //print operation mode name
    mydisp.setFont(0);  //set font to default: font 10
    mydisp.setColor(RED);
    for (i = 0; i < 4; i++)
    {
      mydisp.setFont(intfonts[i + 1]);
      mydisp.setRotation(i);
      mydisp.drawStr(3, 0, "Direction:"); //print string at 3,0 in draw window
      mydisp.print(i);
    }
    delay(1000);
    mydisp.setColor(~RED);
    mydisp.setMode(smode[j]); //set pixels operation mode
    for (i = 0; i < 4; i++)
    {
      mydisp.setFont(intfonts[i + 1]);
      mydisp.setRotation(i);
      mydisp.drawStr(3, 0, "Direction:"); //print string at 3,0 in draw window
      mydisp.print(i);
    }
    delay1();
    mydisp.setFont(0);
  }
}
void save_Data_In_Flash_Chip(void) {
#define comAddress 420000 //this address is in flash chip
  mydisp.cleanDrawWindow();
  //read data from flash chip
  mydisp.drawStr(0, 0, "Read 10 bytes of data from flash chip:");
  mydisp.flashReadStart(comAddress, 10); //ask module read 10 bytes of data start from address 1
  for (i = 0; i < 10; i++)
    buf[i] = mydisp.read1();
  for (i = 0; i < 10; i++)
  {
    mydisp.print(buf[i]); //read a byte of data from module flash chip
    mydisp.print(" ");
  }
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.flashErase(comAddress, 10);  //erase data in flash chip from 420000 to 420009
  mydisp.drawStr(0, 0, "Erased these 10 bytes:");
  mydisp.flashReadStart(comAddress, 10); //ask module read 10 bytes of data start from address 1
  for (i = 0; i < 10; i++)
    buf[i] = mydisp.read1();
  for (i = 0; i < 10; i++)
  {
    mydisp.print(buf[i]); //read a byte of data from module flash chip
    mydisp.print(" ");
  }
  delay1();
  mydisp.drawStr(0, 0, "Write 10 bytes in to flash chip:");
  mydisp.setColor(RED | BLUE);
  for (i = 0; i < 10; i++)
  {
    buf[i] = random(256);
    mydisp.print(buf[i]);
    mydisp.print(" ");
  }
  //write data to flash chip
  mydisp.flashWrite(comAddress, 10, buf); //write 10 byte of data to address of 1 in EEPROM, the source data is in buf array
  delay1();
  mydisp.print("\n\n\rCheck what stored in flash chip:");
  mydisp.flashReadStart(comAddress, 10);
  for (i = 0; i < 10; i++)
    buf[i] = mydisp.read1();
  mydisp.setColor(RED | BLUE);
  for (i = 0; i < 10; i++)
  {
    mydisp.print(buf[i]); //read a byte of data from module
    mydisp.print(" ");
  }
}
void draw_Pixels(void)
{
  mydisp.setBgColor(BLACK);
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setMode('C');  //set pixels operation mode as "COPY"
  mydisp.drawStr(0, 0, "Graphic Fucntion:pixels");
  for (i = 0; i < 200; i++)
  {
    mydisp.setColor(random(COLORRG));
    mydisp.drawPixel(random(DW_W), random(DW_H));
  }
}
void draw_Lines(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.drawStr(0, 0, "Graphic Fucntion:LINEs");
  for (i = 0; i < 50; i++)
  {
    mydisp.setColor(random(COLORRG));
    mydisp.setLinePattern(random(0x70, 256)); //set the line patter, 255 is regular line
    //draw line from x0,y0 to x1,y1
    mydisp.drawLine(random(DW_W), random(DW_H), random(DW_W), random(DW_H));
  }
}
void draw_Linetos(void) {
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setRotation(1);
  mydisp.drawStr(0, 0, "Graphic Fucntion:LINE TO");
  for (i = 0; i < 30; i++)
  {
    mydisp.setColor(random(COLORRG));
    mydisp.setLinePattern(random(0x70, 256)); //set the line patter, 255 is regular line
    //draw line from current graph position to x,y
    mydisp.drawLineTo(random(DW_H), random(DW_W));
  }
}
void draw_Circles(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setRotation(2);
  mydisp.drawStr(0, 0, "Graphic Fucntion:circle");
  for (i = 0; i < 20; i++)
  {
    fill = random(2);
    mydisp.setTrueColor(random(COLORRG / 4), random(COLORRG / 4), random(COLORRG / 4));
    x = random(DW_W);
    y = random(20, DW_H);
    r1 = min(x, DW_W - x);
    r = min(y, DW_H - y);
    r = min(r1, r);
    r = random(r);
    mydisp.drawCircle(x, y, r, fill); //draw a circle, if fill==1, fill the circle
  }
}
void draw_Rectangles(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setRotation(3);
  mydisp.drawStr(0, 0, "Graphic Fucntion:rectangle");
  for (i = 0; i < 10; i++)
  {
    mydisp.setLinePattern(random(0x70, 256)); //set the line patter, 255 is regular line
    mydisp.setColor(random(COLORRG));
    x = random(DW_H - 10);
    y = random(15, DW_W - 10);
    r = random(DW_H - x);
    r1 = random(DW_W - y);
    mydisp.drawBox(x, y, r, r1); //draw a filled rectangle:x,y,width,height
    mydisp.setColor(random(COLORRG));
    x = random(DW_H - 10);
    y = random(15, DW_W - 10);
    r = random(DW_H - x); //width
    r1 = random(DW_W - y);  //height
    mydisp.drawFrame(x, y, r, r1); //draw a non-filled rectangle:x,y,width,height
  }
}
void draw_Filled_Rectangles_Fast(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setRotation(0);
  mydisp.drawStr(0, 0, "Draw filled rectangle use draw window");
  for (i = 0; i < 20; i++)
  {
    mydisp.setBgColor(random(BGCOLOR));
    x = random(15, DW_W - 10);
    y = random(DW_H - 10);
    r = random(DW_W - x);
    r1 = random(DW_H - y);
    mydisp.setDrawWindow(x, y, r, r1); //draw a filled rectangle:x,y,width,height
    mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  }
}
void move_Area(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setMode('C');
  mydisp.setRotation(0);
  mydisp.drawStr(0, 0, "Move area");
  for (i = 0; i < 4; i++)
  {
    mydisp.setColor(random(1,COLORRG));
    mydisp.setRotation(i);
    mydisp.drawCircle(basex, basey, R, 1);
    delay(2000);
    mydisp.moveArea(basex - R, basey - R, R, R, -2, -2);
    mydisp.moveArea(basex - R, basey, R, R, -2, 2);
    mydisp.moveArea(basex, basey - R, R, R, 2, -2);
    mydisp.moveArea(basex, basey, R, R, 2, 2);
  }
}
void display_Mono_Image(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.drawStr(0, 0, "Draw Mono image in 4 dir");
  for (uint8_t i = 0; i < 4; i++) {
    mydisp.setRotation(i);
    mydisp.setColor(random(254) + 1);
    //draw image function have:drawBitmap() for 1bit image,drawBitmap256() for 8bit image,drawBitmap262K() for 18bit image
    mydisp.drawBitmap(12, 12, 41, 21, welcomeimage);
  }
}
void display_8bit_Image(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color,same with clearScreen() now
  mydisp.drawStr(0, 0, "Draw 256 color image");
  //draw image function have:drawBitmap() for 1bit image,drawBitmap256() for 8bit image,drawBitmap262K() for 18bit image
  mydisp.drawBitmap256(max(LCDWidth / 2 - 60, 0), max(LCDHeight / 2 - 15, 0), 120, 30, digole256); //use our image convert tool to convert, www.digole.com/tools
}
void use_EEPROM(void)
{
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Read 10 bytes of data from EEPROM, 976 bytes usable:");
  mydisp.readE2prom(1, 10); //ask module read 10 bytes of data start from address 1
  for (i = 0; i < 10; i++)
    buf[i] = mydisp.read1();
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
  mydisp.readE2prom(1, 10);
  for (i = 0; i < 10; i++)
    buf[i] = mydisp.read1();
  mydisp.setColor(RED | BLUE);
  for (i = 0; i < 10; i++)
  {
    mydisp.print(buf[i]); //read a byte of data from module
    mydisp.print(" ");
  }
}
void test_User_Font_Standard(void)
{
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color,same with clearScreen() now
  //the welNew must endded with a extra value 255 to indicate the end of data
  //download a new splash screen(or start screen, or welcome screen) to module
  //you also can use flashWrite() function to write splash screen data to address 0, if firmware is 3.3
  //  mydisp.uploadStartScreen(sizeof(welNew),welNew);
  //the myfont is a walk men Animation, we put each motion in "0" to "4"
  mydisp.drawStr(0, 0, "Down load walk men motion as user font");
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
  mydisp.setFlashFont(600000);  //use user font in flash chip from any address
  mydisp.drawStr(0, 0, "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUXYZ");
  mydisp.setFont(0);
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
  delay(500); //wait a movement
  //the module will return a value of 17 to indicate flashWrite done, if you writing large data into flush, please use:flashWriteL()
  //you also can use flashWrite to write splash screen to address 0, or user font to flash memory, the user font address is different
  //depending on the module equipt flash chip or not
  mydisp.flashWrite(comAddress, sizeof(commandset), commandset);
  delay(500); //wait done writing
//  mydisp.cleanDrawWindow();
  mydisp.print("Run command set just downloaded");
  //run command set in flash chip
  mydisp.runCommandSet(comAddress); //command set must endded by an extra value of 255
}
void adjust_Backlight(void)
{
  mydisp.undoRotation();  //same with setRotation(0);
  mydisp.cleanDrawWindow(); //clear draw window use the new back ground color
  mydisp.setMode('C');  //set pixels operation mode as "COPY"
  mydisp.drawStr(0, 0, "Adjust the Backlight");
  for (unsigned char i = 0; i <= 100; i += 5)
  {
    mydisp.setPrintPos(9 , 4, _TEXT_);
    mydisp.print(i); //display brightness
    mydisp.print("%");
    mydisp.setBackLight(i);
    delay(250);
  }
}