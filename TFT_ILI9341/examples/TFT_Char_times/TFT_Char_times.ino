/*
Font draw speed test
Needs fonts 4 and 6
 */

// These are the connections for the Mega
#define sclk 52  // Don't change
#define mosi 51  // Don't change
#define cs   47
#define dc   48
#define rst  44  // you can also connect this to the Arduino reset

#include <TFT_ILI9341.h> // Hardware-specific library
#include <SPI.h>

TFT_ILI9341 tft = TFT_ILI9341(cs, dc, rst);       // Invoke custom library

unsigned long drawTime = 0;

void setup(void) {
  tft.init();
  tft.setRotation(1);

}

void loop() {

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  drawTime = millis();

  for (int i = 0; i < 1000; i++) {
    tft.drawNumber(i, 100, 80, 6);
  }

  drawTime = millis() - drawTime;

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawFloat(drawTime / 1000.0, 2, 100, 140, 4);

  delay(4000);
}








