/***************************************************
  Arduino TFT grahpcs library targetted at the UNO
  and Mega boards.

  This library has been derived from the Adafruit_GFX
  library and the associated driver library. See text
  at the end of this file.

  This is a standalone library that contains the
  hardware driver, the graphics funtions and the
  proportional fonts.

  The fonts are Run Length Encoded to reduce their
  size.

 ****************************************************/

#include <Run_faster.h>

#include <Load_fonts.h>

#ifdef LOAD_GLCD
  #include <Fonts/glcdfont.c>
#endif

#ifdef LOAD_FONT2
  #include <Fonts/Font16.h>
#endif

#ifdef LOAD_FONT4
  #include <Fonts/Font32rle.h>
#endif

#ifdef LOAD_FONT6
  #include <Fonts/Font64rle.h>
#endif

#ifdef LOAD_FONT7
  #include <Fonts/Font7srle.h>
#endif

#ifdef LOAD_FONT8
  #include <Fonts/Font72rle.h>
#endif

#ifndef _TFT_ILI9341H_
#define _TFT_ILI9341H_

#include <Arduino.h>
#include <Print.h>

#include <avr/pgmspace.h>

#define swap(a, b) { int16_t t = a; a = b; b = t; }

//These define the ports and port bits used for the chip select (CS) and data/command (DC) lines
// Mega PORTL bit 2 is CS = pin 48, PORTL bit 1 is DC = pin 47
// UNO etc PORTB bit 2 in CS = pin 10, PORTB bit 1 is DC = pin 9
//
#ifdef MEGA_TFT_ILI9341
  #define TFT_CS_L PORTL &= ~_BV(2)
  #define TFT_DC_C PORTL &= ~_BV(1)

  #define TFT_CS_H PORTL |= _BV(2)
  #define TFT_DC_D PORTL |= _BV(1)
#else
  #define TFT_CS_L PORTB &= ~_BV(2)
  #define TFT_DC_C PORTB &= ~_BV(1)

  #define TFT_CS_H PORTB |= _BV(2)
  #define TFT_DC_D PORTB |= _BV(1)
#endif

// Change the width and height if required (defined in portrait mode)
// or use the constructor to over-ride defaults
#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL  0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

/*
#define ILI9341_PWCTR6  0xFC
*/

// Color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */
#define TFT_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TFT_PINK        0xF81F


class TFT_ILI9341 : public Print {

 public:

  TFT_ILI9341(int8_t _CS, int8_t _DC, int8_t _RST = -1, int16_t _W = ILI9341_TFTWIDTH, int16_t _H = ILI9341_TFTHEIGHT);

  void     init(void),

           drawPixel(int16_t x, int16_t y, uint16_t color),

           drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size),
           setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1),

           pushColor(uint16_t color),
           pushColor(uint16_t color, uint16_t len),

           pushColors(uint16_t *data, uint8_t len),
           pushColors(uint8_t *data, uint8_t len),

           fillScreen(uint16_t color),

           writeBegin(void),
           writeEnd(void),

           spiWait(void),

           drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),

           drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
           drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
           fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),

           setRotation(uint8_t r),
           invertDisplay(boolean i),

           drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
           drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color),
           fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
           fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color),

           drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
           fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),

           drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color),

           setCursor(int16_t x, int16_t y),
           setCursor(int16_t x, int16_t y, uint8_t font),
           setTextColor(uint16_t c),
           setTextColor(uint16_t c, uint16_t bg),
           setTextSize(uint8_t s),
           setTextFont(uint8_t f),
           setTextWrap(boolean w),

           spiwrite(uint8_t),
           writecommand(uint8_t c),
           writedata(uint8_t d),
           commandList(uint8_t *addr);

  uint8_t  spiread(void),
           getRotation(void);

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  int16_t  drawChar(unsigned int uniCode, int x, int y, int size),
           drawNumber(long long_num,int poX, int poY, int size),
           drawFloat(float floatNumber,int decimal,int poX, int poY, int size),

           drawString(char *string, int poX, int poY, int size),
           drawCentreString(char *string, int dX, int poY, int size),
           drawRightString(char *string, int dX, int poY, int size),

           height(void),
           width(void);

void spiWrite16(uint16_t data, int16_t count);

  virtual  size_t write(uint8_t);

 private:

  uint8_t  tabcolor;

  boolean  hwSPI;

  uint8_t  mySPCR;

  volatile uint8_t *mosiport, *clkport, *dcport, *rsport, *csport;

  int8_t   _cs, _dc, _rst, _mosi, _miso, _sclk;
 
  uint8_t  mosipinmask, clkpinmask, cspinmask, dcpinmask;


 protected:

  int16_t  _width, _height, // Display w/h as modified by current rotation
           cursor_x, cursor_y;

  uint16_t textcolor, textbgcolor;

  uint8_t  textfont,
           textsize,
           rotation;

  boolean  wrap; // If set, 'wrap' text at right edge of display

};

#endif

/***************************************************

  ORIGINAL LIBRARY HEADER

  This is our library for the Adafruit  ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  Updated with new functions by Bodmer 14/4/15
 ****************************************************/
