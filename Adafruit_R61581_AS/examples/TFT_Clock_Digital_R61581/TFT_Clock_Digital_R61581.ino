/*
 An example digital clock using a TFT LCD screen to show the time.
 Demonstrates use of the font printing routines. (Time updates but date does not.)
 
 For a more accurate clock, it would be better to use the RTClib library.
 But this is just a demo. 
 
 This examples uses the hardware SPI only. Non-hardware SPI
 is just too slow (~8 times slower!)
 
 Based on clock sketch by Gilchrist 6/2/2014 1.0

A few colour codes:
 
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

#include "Adafruit_GFX_AS.h"
#include "Adafruit_R61581_AS.h"

#define R61581_GREY 0x5AEB

Adafruit_R61581_AS tft = Adafruit_R61581_AS();      // Invoke custom library

uint32_t targetTime = 0;                    // for next 1 second timeout
uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

byte omm = 99;
boolean initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

void setup(void) {
  tft.begin(R61581B);
  tft.setRotation(0);
  tft.fillScreen(R61581_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(R61581_YELLOW, R61581_BLACK);

  targetTime = millis() + 1000; 
}

void loop() {
  if (targetTime < millis()) {
    targetTime = millis()+1000;
    ss++;              // Advance second
    if (ss==60) {
      ss=0;
      omm = mm;
      mm++;            // Advance minute
      if(mm>59) {
        mm=0;
        hh++;          // Advance hour
        if (hh>23) {
          hh=0;
        }
      }
    }

    if (ss==0 || initial) {
      initial = 0;
      tft.setTextColor(R61581_GREEN, R61581_BLACK);
      tft.setCursor (50, 52);
      tft.print(__DATE__); // This uses the standard ADAFruit small font

      tft.setTextColor(R61581_BLUE, R61581_BLACK);
      tft.drawCentreString("It is windy",160,48,2); // Next size up font 2

      tft.setTextColor(0xF81F, R61581_BLACK); // Pink
      tft.drawCentreString("12.34",120,91,6); // Large font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 . : a p m
    }

    // Update digital time
    byte xpos = 50;
    byte ypos = 0;
    if (omm != mm) { // Only redraw every minute to minimise flicker
      // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
      tft.setTextColor(0x39C4, R61581_BLACK);  // Leave a 7 segment ghost image, comment out next line!
      //tft.setTextColor(R61581_BLACK, R61581_BLACK); // Set font colour to back to wipe image
      // Font 7 is to show a pseudo 7 segment display.
      // Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
      tft.drawString("88:88",xpos,ypos,7); // Overwrite the text to clear it
      tft.setTextColor(0xFBE0); // Orange
      omm = mm;

      if (hh<10) xpos+= tft.drawChar('0',xpos,ypos,7);
      xpos+= tft.drawNumber(hh,xpos,ypos,7);
      xcolon=xpos;
      xpos+= tft.drawChar(':',xpos,ypos,7);
      if (mm<10) xpos+= tft.drawChar('0',xpos,ypos,7);
      tft.drawNumber(mm,xpos,ypos,7);
    }

    if (ss%2) { // Flash the colon
      tft.setTextColor(0x39C4, R61581_BLACK);
      xpos+= tft.drawChar(':',xcolon,ypos,7);
      tft.setTextColor(0xFBE0, R61581_BLACK);
    }
    else {
      tft.drawChar(':',xcolon,ypos,7);
      colour = random(0xFFFF);
      // Erase the text with a rectangle
      tft.fillRect (0, 64, 240, 20, R61581_BLACK);
      tft.setTextColor(colour);
      tft.drawRightString("Colour:",120,64,4); // Right justified string drawing to x position 120
      String scolour = String(colour,HEX);
      scolour.toUpperCase();
      char buffer[20];
      scolour.toCharArray(buffer,20);
      tft.drawString(buffer,128,64,4);
    }
  }
}

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}



