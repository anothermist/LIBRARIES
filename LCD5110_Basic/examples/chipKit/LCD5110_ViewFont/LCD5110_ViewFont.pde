// LCD5110_ViewFont 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the included full font.
//
// This program requires a Nokia 5110 LCD module.
//
// It is assumed that the LCD module is connected to
// the following pins.
//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12
//

#include <LCD5110_Basic.h>

LCD5110 myGLCD(8,9,10,11,12);

extern unsigned char SmallFont[];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
}

void loop()
{
  myGLCD.clrScr();
  myGLCD.print("Upper case:", LEFT, 0);
  myGLCD.print("ABCDEFGHIJKLM", CENTER, 16);
  myGLCD.print("NOPQRSTUVWXYZ", CENTER, 24);
  delay (5000);
  
  myGLCD.clrScr();
  myGLCD.print("Lower case:", LEFT, 0);
  myGLCD.print("abcdefghijklm", CENTER, 16);
  myGLCD.print("nopqrstuvwxyz", CENTER, 24);
  delay (5000);
  
  myGLCD.clrScr();
  myGLCD.print("Numbers:", LEFT, 0);
  myGLCD.print("0123456789", CENTER, 16);
  delay (5000);
  
  myGLCD.clrScr();
  myGLCD.print("Special:", LEFT, 0);
  myGLCD.print("!\"#$%&'()*+,-.", CENTER, 16);
  myGLCD.print("/:;<=>?@[\\]^_`", CENTER, 24);
  myGLCD.print("{|}~", CENTER, 32);
  delay (5000);
}
