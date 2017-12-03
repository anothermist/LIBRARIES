// Spiro
// Rainbow patern generator
// Alan Senior 22/2/15

#include "SPI.h"

#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library

// For the Adafruit shield, these are the default.
#define TFT_CS         9                  
#define TFT_DC         10                
#define TFT_RST        8 

#define ILI9341_GREY 0x7BEF
Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST); // Use hardware SPI

unsigned long runTime = 0;

float sx = 0, sy = 0;
int xx0 = 0;
int xx1 = 0;
int yy0 = 0; 
int yy1 = 0;

void setup()
{
  randomSeed(analogRead(12));
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  delay(10);
  digitalWrite(7, HIGH);
  // Setup the LCD
  tft.begin();
  tft.setRotation(3);
}

void loop()
{
  runTime = millis();

  tft.fillScreen(ILI9341_BLACK);
  int n = random(2, 19), r = random(20, 100), colour = 0; //rainbow();
  
  for (long i = 0; i < (360 * n); i++) {
    sx = cos((i / n - 90) * 0.0174532925);
    sy = sin((i / n - 90) * 0.0174532925);
    xx0 = sx * (120 - r) + 159;
    yy0 = sy * (120 - r) + 119;


    sy = cos(((i % 360) - 90) * 0.0174532925);
    sx = sin(((i % 360) - 90) * 0.0174532925);
    xx1 = sx * r + xx0;
    yy1 = sy * r + yy0;
    tft.drawPixel(xx1, yy1, rainbow(i % 128)); //colour);
  }
  
  r = random(20, 100);//r = r / random(2,4);
  for (long i = 0; i < (360 * n); i++) {
    sx = cos((i / n - 90) * 0.0174532925);
    sy = sin((i / n - 90) * 0.0174532925);
    xx0 = sx * (120 - r) + 159;
    yy0 = sy * (120 - r) + 119;


    sy = cos(((i % 360) - 90) * 0.0174532925);
    sx = sin(((i % 360) - 90) * 0.0174532925);
    xx1 = sx * r + xx0;
    yy1 = sy * r + yy0;
    tft.drawPixel(xx1, yy1, rainbow(i % 128)); //colour);
  }


  delay(2000);
}

unsigned int rainbow(int value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to red = blue
  //int value = random (128);
  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}


