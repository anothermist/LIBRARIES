/*
 *This demo code will show you all functions for
 *Digole Graphic LCD adapter
 */
#define SC_W 240  //screen width in pixels
#define SC_H 320  //screen Hight in pixels
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 38400); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
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
#include "DigoleImage.h"

const unsigned char welcomeimage[] PROGMEM = {
  0, 0, 0, 0, 0, 127
    , 0, 8, 1, 2, 0, 127
    , 0, 8, 0, 148, 0, 127
    , 0, 16, 0, 89, 112, 127
    , 3, 144, 0, 16, 144, 127
    , 28, 145, 192, 16, 144, 127
    , 1, 30, 128, 80, 144, 127
    , 9, 49, 3, 208, 144, 127
    , 5, 72, 0, 80, 144, 127
    , 2, 72, 0, 150, 144, 127
    , 3, 8, 0, 152, 208, 127
    , 5, 24, 0, 64, 160, 127
    , 4, 148, 0, 64, 128, 127
    , 8, 36, 0, 128, 128, 127
    , 16, 34, 3, 240, 128, 127
    , 32, 65, 0, 14, 0, 127
    , 0, 129, 128, 1, 252, 127
    , 3, 0, 64, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
};
int ptr;
const char a[] = "disp char array";
const char b = 'Q';
int c = 3456;
String d = "I'm a string";
float pi = 3.1415926535;
double lg10;
const unsigned char fonts[] = {
  6, 10, 18, 51, 120, 123};
const char *fontdir[] = {
  "0\xb0", "90\xb0", "180\xb0", "270\xb0"};
  
void resetpos1(void) //for demo use, reset display position and clean the demo line
{
  mydisp.setPrintPos(0, 0, _TEXT_);
  delay(1500); //delay 2 seconds
  mydisp.println("                "); //display space, use to clear the demo line
  mydisp.setPrintPos(0, 0, _TEXT_);
}
void resetpos(void) //for demo use, reset display position and clean the demo line
{
  mydisp.setPrintPos(0, 1, _TEXT_);
  delay(1500); //delay 2 seconds
  mydisp.println("                "); //display space, use to clear the demo line
  mydisp.setPrintPos(0, 1, _TEXT_);
}

void setup() {
//  delay(10);  //waiting for display model ready
  mydisp.begin();
  mydisp.clearScreen(); //CLear screen
  /*----------Special function for color OLED ------------*/
  mydisp.print("Due to the limitation of memory on Arduino, we only can draw small size of color image here");
  delay(2000);
  mydisp.clearScreen(); //CLear screen
  mydisp.print("Shut down whole module after 1 second");
  delay(1000);
  mydisp.write('D');
  mydisp.write('N');
  mydisp.write('A');
  mydisp.write('L');
  mydisp.write('L');
  delay(2000);
  resetpos1();
  mydisp.clearScreen();
  mydisp.print("Adjust the Backlight");
  delay(1000);
  for(unsigned char i=0;i<=100;i+=5)
  {
    mydisp.setPrintPos(9 , 4, _TEXT_);
    mydisp.print(i); //display brightness
    mydisp.print("%");
    mydisp.setBackLight(i);
    delay(250);
  }
  delay(2000);
  mydisp.setBackLight(70);  //set brightness of backlight as 70%
  mydisp.clearScreen();
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
  mydisp.print("Draw 256 color image");
  mydisp.drawBitmap256((SC_W-120)/2,SC_H/2,120,30,digole256);  //use our image convert tool to convert, www.digole.com/tools
  delay(2000);
  mydisp.clearScreen();
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
  mydisp.print("Draw 262K color image");
  mydisp.drawBitmap262K((SC_W-70)/2,SC_H/2,70,43,eyes);
  delay(2000);
  /*----------for text LCD adapter and graphic LCD adapter ------------*/
  resetpos1();
  mydisp.clearScreen();
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
//  for(unsigned int i=0;i<65535;i++)
//  {
//    mydisp.Print::print(random(255));
//  }
  //mydisp.displayConfig(1);    //set config display ON, 0=off
  //mydisp.setI2CAddress(0x29);  //this function only working when you connect using I2C, from 1 to 127
  //delay(1000);
  //mydisp.setLCDColRow(16,2);  //set LCD Col and Row, only time set up is OK
mydisp.displayConfig(0);
mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
  mydisp.drawStr(2, 0, "Demo TEXT now"); //display string at: x=4, y=0
  //Test print function
  mydisp.setColor(random(254)+1);
  mydisp.setPrintPos(0, 1, _TEXT_);
  mydisp.print(a); // display a char array
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("display a char:");
  mydisp.setColor(random(254)+1);
  mydisp.print(b); //display a char
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("int as DEC:");
  mydisp.setColor(random(254)+1);
  mydisp.print(c); //display 3456 in Dec
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("as HEX:");
  mydisp.setColor(random(254)+1);
  mydisp.print(c, HEX); //display 3456 in Hex
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("as OCT:");
  mydisp.setColor(random(254)+1);
  mydisp.print(c, OCT); //display 3456 in Oct
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("BIN:");
  mydisp.setColor(random(254)+1);
  mydisp.print(c, BIN); //display 3456 in Bin
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("float pi=");
  mydisp.setColor(random(254)+1);
  mydisp.print(pi); //display float of PI
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("Pi6=");
  mydisp.setColor(random(254)+1);
  mydisp.print(pi, 6); //display PI in 6 Accuracy
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("Pi*3=");
  mydisp.setColor(random(254)+1);
  mydisp.print(pi * 3, 6); //display PI time 3 in 6 Accuracy
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print("lg5=");
  mydisp.setColor(random(254)+1);
  mydisp.print(log(5), 8); //display log(5) in 8 Accuracy
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.print(d); //display String object
  resetpos();
  for (uint8_t j = 0; j < 2; j++) //making "Hello" string moving
  {
    for (uint8_t i = 0; i < 10; i++) //move string to right
    {
      mydisp.setPrintPos(i, 1, _TEXT_);
      mydisp.setColor(random(254)+1);
      mydisp.print(" Hello ");
      delay(100); //delay 100ms
    }
    for (uint8_t i = 0; i < 10; i++) //move string to left
    {
      mydisp.setPrintPos(9 - i, 1, _TEXT_);
      mydisp.setColor(random(254)+1);
      mydisp.print(" Hello ");
      delay(100);
    }
  }
  mydisp.setColor(random(254)+1);
  mydisp.print("Enjoy it!");
  mydisp.enableCursor(); //enable cursor
  /*---------for Graphic LCD adapter only-------*/
  mydisp.disableCursor(); //enable cursor
  resetpos();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 1, "Negative/flash");
  mydisp.setMode('~');
  for (uint8_t i = 0; i < 10; i++) {
    mydisp.drawBox(0, 13, 110, 13);
    delay(500);
  }
  resetpos();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "Draw image in 4 dir");
  //display image in 4 different directions, use setRot90();setRot180();setRot270();undoRotation();setRotation(dir);
  for (uint8_t i = 0; i < 4; i++) {
    mydisp.setRotation(i);
    mydisp.setColor(random(254)+1);
    mydisp.drawBitmap(12, 12, 41, 21, welcomeimage);
  }
  mydisp.undoRotation();
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "draw a box");
  mydisp.setColor(random(254)+1);
  mydisp.drawBox((SC_W-30)/2, SC_H/2, 30, 40);
  //test drawing a circle
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "draw a circle and a pie");
  mydisp.setColor(random(254)+1);
  mydisp.drawCircle(31, 31, 30);
  mydisp.setColor(random(254)+1);
  mydisp.drawCircle(31, 31, 14, 1);
  //test drawing a Frame
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "draw a Frame");
  mydisp.setColor(random(254)+1);
  mydisp.drawFrame(20, 15, SC_W/2, SC_H/2);
  //Clear an area use set color =0
  resetpos1();
  mydisp.clearScreen();
  mydisp.drawStr(0, 0, "Clear an area by using set color=0");
  mydisp.setColor(random(254)+1);
  mydisp.drawCircle(35, 70, 35, 1);
  mydisp.setColor(random(254)+1);
  mydisp.drawBox(80, 40, 80, 60);
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
  mydisp.setColor(0);
  mydisp.drawCircle(110, 70, 25, 1);
  mydisp.drawBox(20, 50, 40, 30);
  //test drawing Pixels
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "draw Pixels");
  for (uint8_t i = 0; i < 20; i++) {
    mydisp.setColor(random(254)+1);
    mydisp.drawPixel(random(SC_W-1), 12 + random(SC_H-13));
  }
  //test drawing Lines
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "draw Lines");
  for (uint8_t i = 0; i < 10; i++) {
    mydisp.setColor(random(254)+1);
    mydisp.drawLine(random(SC_W-1), 12 + random(SC_H-13), random(SC_W-1), 12 + random(SC_H-13));
  }
  //test drawing Lines to
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "draw Lines to");
  mydisp.setColor(random(254)+1);
  mydisp.drawLine(random(SC_W-1), 12 + random(SC_H-13), random(SC_W-1), 12 + random(SC_H-13));
  for (uint8_t i = 0; i < 10; i++) {
    mydisp.setColor(random(254)+1);
    mydisp.drawLineTo(random(SC_W-1), 12 + random(SC_H-13));
  }
  //test differnt font
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "default font");
  mydisp.setFont(fonts[0]);
  mydisp.nextTextLine();
  mydisp.setColor(random(254)+1);
  mydisp.print("Font 6");
  mydisp.setFont(fonts[1]);
  mydisp.nextTextLine();
  mydisp.setColor(random(254)+1);
  mydisp.print("Font 10");
  mydisp.setFont(fonts[2]);
  mydisp.nextTextLine();
  mydisp.setColor(random(254)+1);
  mydisp.print("Font 18");
  mydisp.setFont(fonts[3]);
  mydisp.nextTextLine();
  mydisp.setColor(random(254)+1);
  mydisp.print("Font 51");
  resetpos1();
  mydisp.clearScreen();
  mydisp.setFont(fonts[4]);
  mydisp.setPrintPos(0, 0, _TEXT_);
  mydisp.setColor(random(254)+1);
  mydisp.print("Font 120");
  mydisp.setFont(fonts[5]); //font 123 only have chars of space and 1 to 9
  mydisp.setColor(random(254)+1);
  mydisp.nextTextLine();
  mydisp.print("123");
  resetpos1();
  mydisp.clearScreen();
  //User font using number: 200,201,202,203,only available after upload to adapter
  mydisp.setFont(10);
  mydisp.setColor(random(254)+1);
  mydisp.drawStr(0, 0, "User font using:200,201,202,203,available after upload to adapter,16Kb space.");
  //move area on screen
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.print("move area on screen");
  mydisp.setColor(random(254)+1);
  mydisp.drawCircle(31, 31, 14, 1);
  delay(2000);
  mydisp.moveArea(31, 31, 15, 15, 5, 8);
  //set display mode, affect all graphic functions, the available mode: "|"-or,"&"-and,"^"-xor,"!"-not,"C"-copy
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.print("use xor or not mode to show a Circle Animation");
  mydisp.setMode('^');
  for (uint8_t i = 1; i < 31; i++) {
    mydisp.setColor(random(254)+1);
    mydisp.drawCircle(31, 31, i); //draw a circle
    delay(50);
    mydisp.drawCircle(31, 31, i); //disappear the previos draw
  }
  //this section show how to use enhanced TEXT position functions:
  //setTextPosBack(),setTextPosOffset() and setTextPosAbs() to make some thing fun
  resetpos1();
  mydisp.clearScreen();
  mydisp.setMode('|');
  mydisp.setColor(random(254)+1);
  mydisp.print("Bold D");
  mydisp.setFont(120);
  mydisp.nextTextLine();
  mydisp.setColor(random(254)+1);
  mydisp.print('D');
  mydisp.setTextPosBack(); //set text position back
  delay(2000);
  mydisp.setTextPosOffset(2, 0); //move text position to x+2
  mydisp.setColor(random(254)+1);
  mydisp.print('D'); //display D again, under or mode, it will bold 'D'
  //use setTextPosAbs() to make a Animation letter
  resetpos1();
  mydisp.clearScreen();
  mydisp.setColor(random(254)+1);
  mydisp.print("make > animation");
  mydisp.setFont(51);
  mydisp.setMode('C');
  for (uint8_t i = 0; i < SC_W-20; i++) {
    mydisp.setTextPosAbs(i, 45);
    mydisp.setColor(random(254)+1);
    mydisp.print('>');
    delay(5);
  }
  //show font in different direction
  resetpos1();
  mydisp.clearScreen();
  mydisp.setFont(fonts[1]);
  mydisp.setColor(random(254)+1);
  mydisp.print("show font in different direction");
  resetpos1();
  mydisp.clearScreen();
  for (uint8_t i = 0; i < 4; i++) {
    mydisp.setRotation(i);
    mydisp.setFont(fonts[2]);
    mydisp.setPrintPos(0, 0);
    for (uint8_t j = 0; j < 3; j++) {
      mydisp.setFont(fonts[2 - j]);
      if (j != 0)
        mydisp.nextTextLine();
      mydisp.setColor(random(254)+1);
      mydisp.print(fontdir[i]);
    }
  }
  resetpos1();
  mydisp.clearScreen();
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
  mydisp.setColor(random(254)+1);
  mydisp.drawHLine(0, SC_H/2-1, SC_W-1); //draw horizontal LiNe
  mydisp.setColor(random(254)+1);
  mydisp.setPrintPos(0, SC_H/2-1, 1); //Set Graphic position
  for (int i = 1; i <= SC_W-1; i = i + 6) //this loop will draw sin graph
  {
    mydisp.setColor(random(254)+1);
    mydisp.drawLineTo(i, (SC_H/2 - (float) (sin(i * 3.14 / (SC_W/2-1))*(SC_H/2))));
  }
}

void loop() {
}



