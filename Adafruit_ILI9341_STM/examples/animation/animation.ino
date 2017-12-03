// http://gifmaker.me/exploder/

#include "data.h"
#include "SPI.h"

#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library

// For the Adafruit shield, these are the default.
#define TFT_CS         9
#define TFT_DC         10
#define TFT_RST        8
#define DLYTIME 10
Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST); // Use hardware SPI

byte x = 0;
byte z = 0;
boolean zflip = 0;
void setup() {
  // put your setup code here, to run once:
  tft.begin(); // Initialise the display (various parameters configured)
  tft.setRotation(1);
  tft.fillScreen(ILI9341_WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:


 tft.setAddrWindow(0, 0, 319, 239);

  for (int y = 0; y < 320 ; y++)
  {


    drawIcon(tmp[x] , 320 - y - 42 , 100 + z , 42 , 43 );
    tft.drawLine(320 - y , 100 + z, 320 - y , 100 + 44 + z, ILI9341_WHITE);

    delay(DLYTIME);
    x++;
    if (x > 6)
      x = 0;
    if (zflip == 0)
    {
      z++;
      if (z > 60)
        zflip = 1;
    }
    else
    {
      z--;
      if (z == 0)
        zflip = 0;
    }

 
  }
 tft.fillScreen(ILI9341_WHITE);
}

#define BUFF_SIZE 20

void drawIcon(const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)


  // Set up a window the right size to stream pixels into
  tft.setAddrWindow(x, y, x + width - 1, y + height - 1);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;
  // Fill and send the buffers
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    tft.pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    tft.pushColors(pix_buffer, np);
  }

}

