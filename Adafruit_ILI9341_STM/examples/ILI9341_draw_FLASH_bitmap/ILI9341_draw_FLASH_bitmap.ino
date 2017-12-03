// Icons are stored in tab ^ above this line (flash_icons.h)

// Option: change IDE compiler option to -O2 as per Instructable to boost speed
// http://www.instructables.com/id/Arduino-IDE-16x-compiler-optimisations-faster-code/
// this example then uses 85% of UNO FLASH and 52% of RAM for dynamic storage

// MS Paint can be used to create and save images in 24bit BMP format

// Images for FLASH images are created with UTFT library tool:
// (libraries\UTFT\Tools\ImageConverter565.exe)

// Convert to .c format then copy into a new tab, instructions here:
// (###### website ######)

// We need this header file to use FLASH as storage with PROGMEM directive


// This is the new tab name
#include "flash_icons.h"
#include "SPI.h"

#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library

// For the Adafruit shield, these are the default.
#define TFT_CS         9                  
#define TFT_DC         10                
#define TFT_RST        8 

// These are the pins I use on an UNO, may need changing for your setup

// Use hardware SPI lines
#define _sclk 13
#define _miso 12 // Needed for SD card, but does not need to be connected to TFT
#define _mosi 11 // Master Out Slave In to send commands/data to TFT and SD card

// TFT chip select and data/command line
#define _cs 10
#define _dc 9

// SD card chip select
#define _sdcs 8

// TFT reset line, can be connected to Arduino reset
#define _rst 7

Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST); // Use hardware SPI


uint32_t drawTime = 0; // Global variable to save draw times for testing

/***************************************************************************************
** Function name:           setup
** Descriptions:            To set things up
***************************************************************************************/

void setup()
{
  Serial.begin(38400); // For debug messages

  tft.begin(); // Initialise the display (various parameters configured)

  Serial.println(F("Here we go..."));

  // Set font foreground and background colours
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

/***************************************************************************************
** Function name:           loop
** Descriptions:            Infinite loop
***************************************************************************************/
void loop()
{
  // Landscape mode
  tft.setRotation(1);

  // Blank screen
  tft.fillScreen(random(0xFFFF));
  
  // Set font foreground and background colours
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  
  // Draw centred text on screen
  tft.drawCentreString("Draw icons from FLASH!", 160, 0, 4);
  
  // Plot icon at position x,y
  // Icon width and height must be correct or image will not look right!
  int x = 122, y = 70, w = 75, h = 82;
  drawIcon(Inst_bot ,x ,y ,w ,h );

  delay(4000);


  // Blank screen
  tft.fillScreen(random(0xFFFF));
  
  // Draws an array of the same icon on the screen
  for (int x = 40; x < 270; x += 52) {
    for (int y = 20; y < 190; y += 52) {
      // We dont need varaibles here but w and h are width and height (25 and 25)
      int w = 25, h = 25;
      // Plot the iconAB at position x,y
      drawIcon(iconAB, x, y, w, h);
    }// y
  }// x

  delay(4000);
  
// End of loop
}


/***************************************************************************************
** Function name:           drawIcon
** Descriptions:            draw a 565 format 16 bit raw image stores in FLASH
***************************************************************************************/

// Size of temporary buffer, this could be avoided but we want to use pushColors()
#define BUFF_SIZE 20 

void drawIcon(const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  drawTime = millis(); // Save current time for performance evaluation

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
  // Line for performance testing
  drawTime = millis() - drawTime;
}

