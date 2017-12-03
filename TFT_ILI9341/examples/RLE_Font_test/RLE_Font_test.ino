/*
 Show all the fonts.

 Make sure all the required fonts are loaded by editting the
 Load_fonts.h file in the TFT_ILI9341 library folder.

 This sketch uses the GLCD and fonts 2, 4, 6, 7, 8

 If using an UNO or Mega (ATmega328 or ATmega2560 processor) then for best
 performance use the F_AS_T option found in the Run_faster.h file within the
 TFT_ILI9341 library folder. When using the F_AS_T option:

   On ATmega328 based boards, pin 10 and pin 9 must be used for the TFT
   chip select (CS) and Data/command (DC) signal lines.

   On ATmega2560 based boards, pin 47 and pin 48 must be used for the TFT
   chip select (CS) and Data/command (DC) signal lines.

 When using the F_AS_T option with the UNO comment out the MEGA_TFT_ILI9341
 #define in the Run_faster.h file.

 When using the F_AS_T option with the MEGA the MEGA_TFT_ILI9341
 #define in the Run_faster.h file must not be commented out.

 This example uses the hardware SPI only.

 */

// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1000


// Choose which board to use from the following two pin sets:

// Connections for UNO and Atmega328 based boards
#define sclk 13  // Don't change, this is the hardware SPI SCLK line
#define mosi 11  // Don't change, this is the hardware SPI MOSI line
#define cs   10  // Chip select for TFT display, don't change when using F_AS_T
#define dc   9   // Data/command line, don't change when using F_AS_T
#define rst  7   // Reset, you could connect this to the Arduino reset pin

// Connections for the MEGA and ATmega2560 based boards
//#define sclk 52  // Don't change, this is the hardware SPI SCLK line
//#define mosi 51  // Don't change, this is the hardware SPI MOSI line
//#define cs   47  // TFT chip select line, don't change when using F_AS_T
//#define dc   48   // TFT data/command line, don't change when using F_AS_T
//#define rst  44   // you could alternatively connect this to the Arduino reset



#include <TFT_ILI9341.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

TFT_ILI9341 tft = TFT_ILI9341(cs, dc, rst);  // Invoke library

unsigned long targetTime = 0; // Used for testing draw times

void setup(void) {
  tft.init();
  tft.setRotation(1);
}

void loop() {
  targetTime = millis();

  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  int xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  tft.drawChar(127, xpos, 64, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 26, 4);
  tft.drawString("<=>?@ABCDE", 0, 52, 4);
  tft.drawString("FGHIJKLMNO", 0, 78, 4);
  tft.drawString("PQRSTUVWX", 0, 104, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 26, 4);
  tft.drawString("pqrstuvwxyz", 0, 52, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 78, 4);
  tft.drawChar(127, xpos, 78, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);

  tft.drawString("012345", 0, 0, 6);
  tft.drawString("6789", 0, 40, 6);
  tft.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 72, 8);
  delay(WAIT);;

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 72, 8);
  delay(WAIT);;

  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
  xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 128, 2);
  tft.drawChar(127, xpos, 128, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 52, 4);
  tft.drawString("<=>?@ABCDE", 0, 104, 4);
  tft.drawString("FGHIJKLMNO", 0, 156, 4);
  tft.drawString("PQRSTUVWX", 0, 208, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 52, 4);
  tft.drawString("pqrstuvwxyz", 0, 104, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 156, 4);
  tft.drawChar(127, xpos, 156, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);

  tft.drawString("01234", 0, 0, 6);
  tft.drawString("56789", 0, 80, 6);
  tft.drawString("apm-:.", 0, 160, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 144, 8);
  delay(WAIT);;

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 144, 8);
  delay(WAIT);;

  tft.setTextColor(TFT_MAGENTA, TFT_BROWN);

  tft.drawNumber(millis() - targetTime, 0, 180, 4);
  delay(WAIT);;

  targetTime = millis();

  tft.setTextSize(1);
  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  tft.drawChar(127, xpos, 64, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 26, 4);
  tft.drawString("<=>?@ABCDE", 0, 52, 4);
  tft.drawString("FGHIJKLMNO", 0, 78, 4);
  tft.drawString("PQRSTUVWX", 0, 104, 4);

  delay(WAIT);
  tft.fillScreen(TFT_BROWN);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 26, 4);
  tft.drawString("pqrstuvwxyz", 0, 52, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 78, 4);
  tft.drawChar(127, xpos, 78, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_BLUE);

  tft.drawString("012345", 0, 0, 6);
  tft.drawString("6789", 0, 40, 6);
  tft.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_RED);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_YELLOW);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 72, 8);
  delay(WAIT);

  tft.fillScreen(TFT_BLUE);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 72, 8);
  delay(WAIT);

  tft.setTextSize(2);
  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
  xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 128, 2);
  tft.drawChar(127, xpos, 128, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 52, 4);
  tft.drawString("<=>?@ABCDE", 0, 104, 4);
  tft.drawString("FGHIJKLMNO", 0, 156, 4);
  tft.drawString("PQRSTUVWX", 0, 208, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BROWN);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 52, 4);
  tft.drawString("pqrstuvwxyz", 0, 104, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 156, 4);
  tft.drawChar(127, xpos, 156, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_BLUE);

  tft.drawString("01234", 0, 0, 6);
  tft.drawString("56789", 0, 80, 6);
  tft.drawString("apm-:.", 0, 160, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_RED);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 144, 8);
  delay(WAIT);;

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 144, 8);
  delay(WAIT);;

  tft.setTextColor(TFT_BROWN, TFT_MAGENTA);

  tft.drawNumber(millis() - targetTime, 0, 180, 4);
  delay(WAIT);;
}

