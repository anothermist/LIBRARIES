/*
 Show all the fonts.
 
 Only font sizes 2, 4, 6 and 7 are implemented in the Adafruit_GFX_AS library.
 
 This examples uses the hardware SPI only. Non-hardware SPI
 is just too slow (~8 times slower!)
 
 Alan Senior 10/1/2015
 
 Colours:
 
 code	color
 0x0000	Black
 0xFFFF	White
 0xBDF7	Light Gray
 0x7BEF	Dark Gray
 0xF800	Red
 0xFFE0	Yellow
 0xFBE0	Orange
 0x79E0	Brown
 0x7E0	Green
 0x7FF	Cyan
 0x1F	Blue
 0xF81F	Pink
 
 */

#define sclk 13  // Don't change
#define mosi 11  // Don't change
#define cs   9
#define dc   8
#define rst  7  // you can also connect this to the Arduino reset

#include <Adafruit_GFX_AS.h>    // Core graphics library
#include <Adafruit_QDTech_AS.h> // Hardware-specific library
#include <SPI.h>

Adafruit_QDTech_AS tft = Adafruit_QDTech_AS(cs, dc, rst);       // Invoke custom library

unsigned long targetTime = 0;
byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11;

void setup(void) {
  tft.init();
  tft.setRotation(1);
}

void loop() {

    tft.fillScreen(QDTech_BLACK);
    tft.setTextColor(QDTech_GREEN);

    tft.drawString(" !\"#$%&'()*+,-./0123456",0,0,2);
    tft.drawString("789:;<=>?@ABCDEFGHIJKL",0,16,2);
    tft.drawString("MNOPQRSTUVWXYZ[\\]^_`",0,32,2);
    tft.drawString("abcdefghijklmnopqrstuvw",0,48,2);
    int xpos=0;
    xpos+=tft.drawString("xyz{|}~",0,64,2);
    tft.drawChar(127,xpos,64,2);
    delay(4000);
    
    tft.fillScreen(QDTech_BLACK);
    tft.setTextColor(QDTech_GREEN);

    tft.drawString(" !\"#$%&'()*+,-.",0,0,4);
    tft.drawString("/0123456789:;",0,26,4);
    tft.drawString("<=>?@ABCDE",0,52,4);
    tft.drawString("FGHIJKLMNO",0,78,4);
    tft.drawString("PQRSTUVWX",0,104,4);
    delay(4000);
    tft.fillScreen(QDTech_BLACK);
    tft.drawString("YZ[\\]^_`abc",0,0,4);
    tft.drawString("defghijklmno",0,26,4);
    tft.drawString("pqrstuvwxyz",0,52,4);
    xpos=0;
    xpos+=tft.drawString("{|}~",0,78,4);
    tft.drawUnicode(127,xpos,78,4);
    delay(4000);
    
    tft.fillScreen(QDTech_BLACK);
    tft.setTextColor(QDTech_BLUE);

    tft.drawString("012346",0,0,6);
    tft.drawString("6789",0,40,6);
    tft.drawString("apm-:.",0,80,6);
    delay(4000);

    tft.fillScreen(QDTech_BLACK);
    tft.setTextColor(QDTech_RED);

    tft.drawString("0123",0,0,7);
    tft.drawString("4567",0,60,7);
    delay(4000);

    tft.fillScreen(QDTech_BLACK);
    tft.drawString("890:.",0,0,7);
    tft.drawString("",0,60,7);
    delay(4000);
    
    tft.setTextColor(QDTech_MAGENTA, QDTech_WHITE);
    tft.drawCentreString(">>That's all<< ",80,104,4);
    delay(4000);
}





