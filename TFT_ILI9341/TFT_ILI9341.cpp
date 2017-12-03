/***************************************************
  Arduino TFT graphics library targetted at the UNO
  and Mega boards.

  This library has been derived from the Adafruit_GFX
  library and the associated driver library. See text
  at the end of this file.

  This is a standalone library that contains the
  hardware driver, the graphics funtions and the
  proportional fonts.

  The fonts are Run Length Encoded to reduce their
  size.

  13/6/15 Merged the TFT_GFX and TFT_driver libraries
  14/6/15 Posted as version 10 on Instructables
  29/6/15 Updated to work with F_AS_T mode on Mega
  1/7/15  Minor bug fix

  To do: Proportional fonts are rendered in whole
  byte widths, could change to true character width

 ****************************************************/

#include "TFT_ILI9341.h"

#include <avr/pgmspace.h>
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
TFT_ILI9341::TFT_ILI9341(int8_t cs, int8_t dc, int8_t rst, int16_t w, int16_t h)
{
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  _mosi  = _sclk = 0;

  _width    = w;
  _height   = h;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textfont = 1;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = true;
  //hwSPI     = true;
}

void TFT_ILI9341::spiwrite(uint8_t c)
{
  uint8_t backupSPCR = SPCR;
  SPCR = mySPCR;
  SPDR = c;
  while (!(SPSR & _BV(SPIF)));
  SPCR = backupSPCR;
}


void TFT_ILI9341::writecommand(uint8_t c)
{
  *dcport &=  ~dcpinmask;
  *csport &= ~cspinmask;
  spiwrite(c);
  *csport |= cspinmask;
}


void TFT_ILI9341::writedata(uint8_t c)
{
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
  spiwrite(c);
  *csport |= cspinmask;
}

void TFT_ILI9341::writeBegin()
{
#ifdef F_AS_T
  //TFT_DC_D;
  //TFT_CS_L;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#endif
}

void TFT_ILI9341::writeEnd() {
#ifdef F_AS_T
  TFT_CS_H;
#else
  *csport |= cspinmask;
#endif
}


// If the SPI library has transaction support, these functions
// establish settings and protect from interference from other
// libraries.  Otherwise, they simply do nothing.

#ifdef SPI_HAS_TRANSACTION
static inline void spi_begin(void) __attribute__((always_inline));

static inline void spi_begin(void) {
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
}

static inline void spi_end(void) __attribute__((always_inline));

static inline void spi_end(void) {
  SPI.endTransaction();
}

#else
#define spi_begin()
#define spi_end()
#endif


void TFT_ILI9341::init(void)
{
  if (_rst > 0) {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, LOW);
  }

  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);
  csport    = portOutputRegister(digitalPinToPort(_cs));
  cspinmask = digitalPinToBitMask(_cs);
  dcport    = portOutputRegister(digitalPinToPort(_dc));
  dcpinmask = digitalPinToBitMask(_dc);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  mySPCR = SPCR;

  // toggle RST low to reset
  if (_rst > 0) {
    digitalWrite(_rst, HIGH);
    delay(5);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(150);
  }

  //if(cmdList) commandList(cmdList);

  spi_begin();
  writecommand(0xEF);
  writedata(0x03);
  writedata(0x80);
  writedata(0x02);

  writecommand(0xCF);
  writedata(0x00);
  writedata(0XC1);
  writedata(0X30);

  writecommand(0xED);
  writedata(0x64);
  writedata(0x03);
  writedata(0X12);
  writedata(0X81);

  writecommand(0xE8);
  writedata(0x85);
  writedata(0x00);
  writedata(0x78);

  writecommand(0xCB);
  writedata(0x39);
  writedata(0x2C);
  writedata(0x00);
  writedata(0x34);
  writedata(0x02);

  writecommand(0xF7);
  writedata(0x20);

  writecommand(0xEA);
  writedata(0x00);
  writedata(0x00);

  writecommand(ILI9341_PWCTR1);    //Power control
  writedata(0x23);   //VRH[5:0]

  writecommand(ILI9341_PWCTR2);    //Power control
  writedata(0x10);   //SAP[2:0];BT[3:0]

  writecommand(ILI9341_VMCTR1);    //VCM control
  writedata(0x3e);
  writedata(0x28);

  writecommand(ILI9341_VMCTR2);    //VCM control2
  writedata(0x86);  //--

  writecommand(ILI9341_MADCTL);    // Memory Access Control
  writedata(0x48);

  writecommand(ILI9341_PIXFMT);
  writedata(0x55);

  writecommand(ILI9341_FRMCTR1);
  writedata(0x00);
  writedata(0x18);

  writecommand(ILI9341_DFUNCTR);    // Display Function Control
  writedata(0x08);
  writedata(0x82);
  writedata(0x27);

  writecommand(0xF2);    // 3Gamma Function Disable
  writedata(0x00);

  writecommand(ILI9341_GAMMASET);    //Gamma curve selected
  writedata(0x01);

  writecommand(ILI9341_GMCTRP1);    //Set Gamma
  writedata(0x0F);
  writedata(0x31);
  writedata(0x2B);
  writedata(0x0C);
  writedata(0x0E);
  writedata(0x08);
  writedata(0x4E);
  writedata(0xF1);
  writedata(0x37);
  writedata(0x07);
  writedata(0x10);
  writedata(0x03);
  writedata(0x0E);
  writedata(0x09);
  writedata(0x00);

  writecommand(ILI9341_GMCTRN1);    //Set Gamma
  writedata(0x00);
  writedata(0x0E);
  writedata(0x14);
  writedata(0x03);
  writedata(0x11);
  writedata(0x07);
  writedata(0x31);
  writedata(0xC1);
  writedata(0x48);
  writedata(0x08);
  writedata(0x0F);
  writedata(0x0C);
  writedata(0x31);
  writedata(0x36);
  writedata(0x0F);

  writecommand(ILI9341_SLPOUT);    //Exit Sleep
  spi_end();
  delay(120);
  spi_begin();
  writecommand(ILI9341_DISPON);    //Display on
  spi_end();

}


// Draw a circle outline
void TFT_ILI9341::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0 + r, color);
  drawPixel(x0  , y0 - r, color);
  drawPixel(x0 + r, y0  , color);
  drawPixel(x0 - r, y0  , color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void TFT_ILI9341::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void TFT_ILI9341::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  drawFastVLine(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void TFT_ILI9341::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
      drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
      drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
    }
  }
}


void TFT_ILI9341::fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color);
}

// Draw a rectangle
void TFT_ILI9341::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

// Draw a rounded rectangle
void TFT_ILI9341::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  drawFastHLine(x + r  , y    , w - 2 * r, color); // Top
  drawFastHLine(x + r  , y + h - 1, w - 2 * r, color); // Bottom
  drawFastVLine(x    , y + r  , h - 2 * r, color); // Left
  drawFastVLine(x + w - 1, y + r  , h - 2 * r, color); // Right
  // draw four corners
  drawCircleHelper(x + r    , y + r    , r, 1, color);
  drawCircleHelper(x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(x + r    , y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void TFT_ILI9341::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  fillRect(x + r, y, w - 2 * r, h, color);

  // draw four corners
  fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  fillCircleHelper(x + r    , y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle
void TFT_ILI9341::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}


// Fill a triangle - original Adafruit function works well and code footprint is small
void TFT_ILI9341::fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)      a = x1;
    else if (x1 > b) b = x1;
    if (x2 < a)      a = x2;
    else if (x2 > b) b = x2;
    drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1,
  sa   = 0,
  sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) last = y1;  // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b) swap(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b) swap(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }
}

void TFT_ILI9341::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        drawPixel(x + i, y + j, color);
      }
    }
  }
}


void TFT_ILI9341::setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

void TFT_ILI9341::setCursor(int16_t x, int16_t y, uint8_t font)
{
  textfont = font;
  cursor_x = x;
  cursor_y = y;
}

void TFT_ILI9341::setTextSize(uint8_t s)
{
  textsize = (s > 0) ? s : 1;
}

void TFT_ILI9341::setTextFont(uint8_t f)
{
  textfont = (f > 0) ? f : 1;
}

void TFT_ILI9341::setTextColor(uint16_t c)
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void TFT_ILI9341::setTextColor(uint16_t c, uint16_t b)
{
  textcolor   = c;
  textbgcolor = b;
}

void TFT_ILI9341::setTextWrap(boolean w)
{
  wrap = w;
}

uint8_t TFT_ILI9341::getRotation(void)
{
  return rotation;
}


// Return the size of the display (per current rotation)
int16_t TFT_ILI9341::width(void)
{
  return _width;
}

int16_t TFT_ILI9341::height(void)
{
  return _height;
}


// Draw a character - only used for the original Adafruit font
void TFT_ILI9341::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
#ifdef LOAD_GLCD
  if ((x >= _width)            || // Clip right
      (y >= _height)           || // Clip bottom
      ((x + 6 * size - 1) < 0) || // Clip left
      ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i = 0; i < 6; i++ ) {
    uint8_t line;
    if (i == 5)
      line = 0x0;
    else
      line = pgm_read_byte(font + (c * 5) + i);
    for (int8_t j = 0; j < 8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x + i, y + j, color);
        else {  // big size
          fillRect(x + (i * size), y + (j * size), size, size, color);
        }
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x + i, y + j, bg);
        else {  // big size
          fillRect(x + i * size, y + j * size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
#endif
}

void TFT_ILI9341::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
#ifdef F_AS_T
  // Column addr set
  TFT_DC_C;
  TFT_CS_L;
  SPDR = ILI9341_CASET;
  spiWait();

  TFT_DC_D;
  SPDR = x0 >> 8; spiWait();
  SPDR = x0; spiWait();
  SPDR = x1 >> 8; spiWait();
  SPDR = x1; spiWait();

  // Row addr set
  TFT_DC_C;
  //TFT_CS_L;
  SPDR = ILI9341_PASET; spiWait();

  TFT_DC_D;
  SPDR = y0 >> 8; spiWait();
  SPDR = y0; spiWait();
  SPDR = y1 >> 8; spiWait();
  SPDR = y1; spiWait();

  // write to RAM
  TFT_DC_C;
  //TFT_CS_L;
  SPDR = ILI9341_RAMWR; spiWait();

  //CS, HIGH;
  //TFT_CS_H;
  TFT_DC_D;
#else
  writecommand(ILI9341_CASET); // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND

  writecommand(ILI9341_PASET); // Row addr set
  writedata(y0 >> 8);
  writedata(y0);     // YSTART
  writedata(y1 >> 8);
  writedata(y1);     // YEND

  writecommand(ILI9341_RAMWR); // write to RAM
#endif
}

void TFT_ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

#ifdef F_AS_T
  // Column addr set
  TFT_DC_C;
  TFT_CS_L;
  SPDR = ILI9341_CASET;
  spiWait();

  TFT_DC_D;
  SPDR = x >> 8; spiWait();
  SPDR = x; x++; spiWait();
  SPDR = x >> 8; spiWait();
  SPDR = x; spiWait();

  // Row addr set
  TFT_DC_C;
  //TFT_CS_L;
  SPDR = ILI9341_PASET; spiWait();

  TFT_DC_D;
  SPDR = y >> 8; spiWait();
  SPDR = y; y++; spiWait();
  SPDR = y >> 8; spiWait();
  SPDR = y; spiWait();

  // write to RAM
  TFT_DC_C;
  //TFT_CS_L;
  SPDR = ILI9341_RAMWR; spiWait();

  TFT_DC_D;

  SPDR = color >> 8; spiWait();
  SPDR = color; spiWait();

  //CS, HIGH;
  TFT_CS_H;
  TFT_DC_D;
#else
  spi_begin();
  setAddrWindow(x, y, x + 1, y + 1);

  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;

  spiwrite(color >> 8);
  spiwrite(color);

  *csport |= cspinmask;
  spi_end();
#endif
}

void TFT_ILI9341::pushColor(uint16_t color)
{
  spi_begin();
#ifdef F_AS_T
  TFT_DC_D;
  TFT_CS_L;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#endif
  //uint8_t backupSPCR =SPCR;
  //SPCR = mySPCR;

  SPDR = color >> 8;
  while (!(SPSR & _BV(SPIF)));
  SPDR = color;
  while (!(SPSR & _BV(SPIF)));

  //SPCR = backupSPCR;

#ifdef F_AS_T
  TFT_CS_H;
#else
  *csport |= cspinmask;
#endif

  spi_end();
}

// This is the byte array version for 16 bit raw images.
void TFT_ILI9341::pushColor(uint16_t color, uint16_t len)
{
  spi_begin();

#ifdef F_AS_T
  TFT_DC_D;
  TFT_CS_L;
  spiWrite16(color, len);
  TFT_CS_H;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;

  uint8_t backupSPCR =SPCR;
  SPCR = mySPCR;

  while (len) {
    len--;
    // This order is fast as we loop back during the SPI wait period
    while (!(SPSR & _BV(SPIF)));
    SPDR = color>>8;
    while (!(SPSR & _BV(SPIF)));
    SPDR = color;
  }
  while (!(SPSR & _BV(SPIF)));
  SPCR = backupSPCR;
  *csport |= cspinmask;
#endif

  spi_end();
}

// Sends an array of 16-bit color values to the TFT; used
// externally by BMP examples.  Assumes that setAddrWindow() has
// previously been called to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).
void TFT_ILI9341::pushColors(uint16_t *data, uint8_t len)
{
  uint16_t color;
  spi_begin();
#ifdef F_AS_T
  TFT_DC_D;
  TFT_CS_L;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#endif
  //uint8_t backupSPCR =SPCR;
  //SPCR = mySPCR;

  while (len) {
    len--;
    color = *data++;
    // This order is fast as we loop back & fetch during the SPI wait period!
    while (!(SPSR & _BV(SPIF)));
    SPDR = color >> 8;
    while (!(SPSR & _BV(SPIF)));
    SPDR = color;
  }
  while (!(SPSR & _BV(SPIF)));

  //SPCR = backupSPCR;

#ifdef F_AS_T
  TFT_CS_H;
#else
  *csport |= cspinmask;
#endif

  spi_end();
}

// This is the byte array version for 16 bit raw images.
void TFT_ILI9341::pushColors(uint8_t *data, uint8_t len)
{
  spi_begin();

#ifdef F_AS_T
  TFT_DC_D;
  TFT_CS_L;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#endif

  //uint8_t backupSPCR =SPCR;
  //SPCR = mySPCR;
  while (len) {
    len--;
    // This order is fast as we loop back during the SPI wait period
    while (!(SPSR & _BV(SPIF)));
    SPDR = *data++;
    while (!(SPSR & _BV(SPIF)));
    SPDR = *data++;
  }
  while (!(SPSR & _BV(SPIF)));
  //SPCR = backupSPCR;

#ifdef F_AS_T
  TFT_CS_H;
#else
  *csport |= cspinmask;
#endif

  spi_end();
}

// Bresenham's algorithm - thx wikipedia - speed enhanced by Bodmer this uses
// the eficient FastH/V Line draw routine for segments of 2 pixels or more
void TFT_ILI9341::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  boolean steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx = x1 - x0, dy = abs(y1 - y0);;


  int16_t err = dx >> 1, ystep = -1, xs = x0, dlen = 0;
  if (y0 < y1) ystep = 1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(y0, xs, color);
        else drawFastVLine(y0, xs, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastVLine(y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(xs, y0, color);
        else drawFastHLine(xs, y0, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastHLine(xs, y0, dlen, color);
  }
}

void TFT_ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;

  if ((y + h - 1) >= _height)
    h = _height - y;

  spi_begin();
  setAddrWindow(x, y, x, y + h - 1);

#ifdef F_AS_T
  //TFT_CS_L;
  //TFT_DC_D;
  spiWrite16(color, h);
//  while (h) {
//    h--;
//    while (!(SPSR & _BV(SPIF)));
//    SPDR = hi;
//    while (!(SPSR & _BV(SPIF)));
//    SPDR = lo;
//  }
//  while (!(SPSR & _BV(SPIF)));
  TFT_CS_H;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
  uint8_t hi = color >> 8, lo = color;
  while (h) {
    h--;
    spiwrite(hi);
    spiwrite(lo);
  }
  *csport |= cspinmask;
#endif
  spi_end();
}


void TFT_ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width)  w = _width - x;

  //spi_begin();
  setAddrWindow(x, y, x + w - 1, y);

#ifdef F_AS_T
  //TFT_CS_L;
  spiWrite16(color, w);
//  while (w) {
//    w--;
//    while (!(SPSR & _BV(SPIF)));
//    SPDR = hi;
//    while (!(SPSR & _BV(SPIF)));
//    SPDR = lo;
//  }
//  while (!(SPSR & _BV(SPIF)));
  TFT_CS_H;
#else
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
  uint8_t hi = color >> 8, lo = color;
  while (w) {
    w--;
    spiwrite(hi);
    spiwrite(lo);
  }
  *csport |= cspinmask;
#endif
  //spi_end();
}

// fill a rectangle
void TFT_ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  // rudimentary clipping (drawChar w/big text requires this)
  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width)  w = _width  - x;
  if ((y + h - 1) >= _height) h = _height - y;

  spi_begin();
  setAddrWindow(x, y, x + w - 1, y + h - 1);

#ifdef F_AS_T
  //TFT_CS_L;
  //TFT_DC_D;
  while (h--) spiWrite16(color, w);
//  for (y = h; y > 0; y--) {
//    for (x = w; x > 0; x--) {
//      while (!(SPSR & _BV(SPIF)));
//      SPDR = hi;
//      while (!(SPSR & _BV(SPIF)));
//      SPDR = lo;
//    }
//  }
//  while (!(SPSR & _BV(SPIF)));
  TFT_CS_H;
#else
  uint8_t hi = color >> 8, lo = color;
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
  for (y = h; y > 0; y--) {
    for (x = w; x > 0; x--) {
      spiwrite(hi);
      spiwrite(lo);
    }
  }
  *csport |= cspinmask;
#endif

  spi_end();
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t TFT_ILI9341::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void TFT_ILI9341::setRotation(uint8_t m)
{
  rotation = m % 4;
  while (!(SPSR & _BV(SPIF))); // We need this here for some reason...
  spi_begin();
  writecommand(ILI9341_MADCTL);
  switch (rotation) {
    case 0:
      writedata(MADCTL_MX | MADCTL_BGR);
      _width  = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 1:
      writedata(MADCTL_MV | MADCTL_BGR);
      _width  = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    case 2:
      writedata(MADCTL_MY | MADCTL_BGR);
      _width  = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 3:
      writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      _width  = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
/*
    case 4:
      writedata(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
      _width  = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 5:
      writedata(MADCTL_MV | MADCTL_MX | MADCTL_BGR);
      _width  = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    case 6:
      writedata(MADCTL_BGR);
      _width  = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 7:
      writedata(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      _width  = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
*/
  }
  spi_end();
}


void TFT_ILI9341::invertDisplay(boolean i)
{
  spi_begin();
  writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
  spi_end();
}

size_t TFT_ILI9341::write(uint8_t uniCode)
{
  if (uniCode == '\r') return 1;
  unsigned int width = 0;
  unsigned int height = 0;

  switch (textfont) {
#ifdef LOAD_GLCD
    case 1:
      width = textsize*6;
      height = 8;
      break;
#endif

#ifdef LOAD_FONT2
    case 2:
      width = pgm_read_byte(widtbl_f16 + uniCode - 32);
      height = chr_hgt_f16;
      break;
#endif

      //    case 3:
      //      width = pgm_read_byte(widtbl_f24+uniCode-32);
      //      height = chr_hgt_f24;
      //      break;

#ifdef LOAD_FONT4
    case 4:
      width = pgm_read_byte(widtbl_f32 + uniCode - 32);
      height = chr_hgt_f32;
      break;
#endif

      //    case 5:
      //      width = pgm_read_byte(widtbl_f48+uniCode-32);
      //      height = chr_hgt_f48;
      //      break;

#ifdef LOAD_FONT6
    case 6:
      width = pgm_read_byte(widtbl_f64 + uniCode - 32);
      height = chr_hgt_f64;
      break;
#endif

#ifdef LOAD_FONT7
    case 7:
      width = pgm_read_byte(widtbl_f7s + uniCode - 32);
      height = chr_hgt_f7s;
      break;
#endif

#ifdef LOAD_FONT8
    case 8:
      width = pgm_read_byte(widtbl_f72 + uniCode - 32);
      height = chr_hgt_f72;
      break;
#endif

    default:
      return 0;
  }

  if (textfont != 1) {
    width = (width + 7) / 8;      // Width in whole bytes for proportional fonts
    width = width * 8 * textsize; // Width is now in pixels
  }

  height = height * textsize;

  if (uniCode == '\n') {
    cursor_y += height;
    cursor_x  = 0;
  }
  else
  {
    if (wrap && (cursor_x + width > _width))
    {
      cursor_y += height;
      cursor_x = 0;
    }
    cursor_x += drawChar(uniCode, cursor_x, cursor_y, textfont);
  }
  return 1;
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a unicode onto the screen
***************************************************************************************/
int TFT_ILI9341::drawChar(unsigned int uniCode, int x, int y, int font)
{
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int flash_address = 0; // 16 bit address OK for Arduino if font files <60K

  uniCode -= 32; // Not using info from font files at the moment

  switch (font) {
#ifdef LOAD_GLCD
    case 1:
      drawChar(x, y, uniCode + 32, textcolor, textbgcolor, textsize);
      return 6 * textsize;
#endif

#ifdef LOAD_FONT2
    case 2:
      flash_address = pgm_read_word(&chrtbl_f16[uniCode]);
      width = pgm_read_byte(widtbl_f16 + uniCode);
      height = chr_hgt_f16;
      break;
#endif

      //    case 3:
      //      flash_address = pgm_read_word(&chrtbl_f24[uniCode]);
      //      width = pgm_read_byte(widtbl_f24+uniCode);
      //      height = chr_hgt_f24;
      //      break;

#ifdef LOAD_FONT4
    case 4:
      flash_address = pgm_read_word(&chrtbl_f32[uniCode]);
      width = pgm_read_byte(widtbl_f32 + uniCode);
      height = chr_hgt_f32;
      break;
#endif

      //    case 5:
      //      flash_address = pgm_read_word(&chrtbl_f48[uniCode]);
      //      width = pgm_read_byte(widtbl_f48+uniCode);
      //      height = chr_hgt_f48;
      //      break;

#ifdef LOAD_FONT6
    case 6:
      flash_address = pgm_read_word(&chrtbl_f64[uniCode]);
      width = pgm_read_byte(widtbl_f64 + uniCode);
      height = chr_hgt_f64;
      break;
#endif

#ifdef LOAD_FONT7
    case 7:
      flash_address = pgm_read_word(&chrtbl_f7s[uniCode]);
      width = pgm_read_byte(widtbl_f7s + uniCode);
      height = chr_hgt_f7s;
      break;
#endif

#ifdef LOAD_FONT8
    case 8:
      flash_address = pgm_read_word(&chrtbl_f72[uniCode]);
      width = pgm_read_byte(widtbl_f72 + uniCode);
      height = chr_hgt_f72;
      break;
#endif

    default:
      return 0;
  }

  int w = width;
  int pX      = 0;
  int pY      = y;
  byte line = 0;
  byte tl = textcolor;
  byte th = textcolor >> 8;
  byte bl = textbgcolor;
  byte bh = textbgcolor >> 8;

  if (font == 2) {
    w = w + 7;
    w = w / 8;
    width++; // Add a gap further of 1 pixel between characters for this font
    if (x + width * textsize >= _width) return width * textsize ;

    if (textcolor == textbgcolor || textsize != 1) {

      for (int i = 0; i < height; i++)
      {
        if (textcolor != textbgcolor) {
          if (textsize == 1) drawFastHLine(x, pY, width, textbgcolor);
          else fillRect(x, pY, width * textsize, textsize, textbgcolor);
        }
        for (int k = 0; k < w; k++)
        {
          line = pgm_read_byte(flash_address + w * i + k);
          if (line) {
            if (textsize == 1) {
              pX = x + k * 8;
              if (line & 0x80) drawPixel(pX, pY, textcolor);
              if (line & 0x40) drawPixel(pX + 1, pY, textcolor);
              if (line & 0x20) drawPixel(pX + 2, pY, textcolor);
              if (line & 0x10) drawPixel(pX + 3, pY, textcolor);
              if (line & 0x08) drawPixel(pX + 4, pY, textcolor);
              if (line & 0x04) drawPixel(pX + 5, pY, textcolor);
              if (line & 0x02) drawPixel(pX + 6, pY, textcolor);
              if (line & 0x01) drawPixel(pX + 7, pY, textcolor);
            }
            else {
              pX = x + k * 8 * textsize;
              if (line & 0x80) fillRect(pX, pY, textsize, textsize, textcolor);
              if (line & 0x40) fillRect(pX + textsize, pY, textsize, textsize, textcolor);
              if (line & 0x20) fillRect(pX + 2 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x10) fillRect(pX + 3 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x08) fillRect(pX + 4 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x04) fillRect(pX + 5 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x02) fillRect(pX + 6 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x01) fillRect(pX + 7 * textsize, pY, textsize, textsize, textcolor);
            }
          }
        }
        pY += textsize;
      }
    }
    else
      // Faster drawing of characters and background using block write
    {
      setAddrWindow(x, y, (x + w * 8) - 1, y + height - 1);

      writeBegin();
      byte mask;
      for (int i = 0; i < height; i++)
      {
        for (int k = 0; k < w; k++)
        {
          line = pgm_read_byte(flash_address + w * i + k);
          pX = x + k * 8;
          mask = 0x80;
          while (mask) {
            if (line & mask) {
              while (!(SPSR & _BV(SPIF)));
              SPDR = th;
              while (!(SPSR & _BV(SPIF)));
              SPDR = tl;
            }
            else {
              while (!(SPSR & _BV(SPIF)));
              SPDR = bh;
              while (!(SPSR & _BV(SPIF)));
              SPDR = bl;
            }
            mask = mask >> 1;
          }
        }
        pY += textsize;
      }
      while (!(SPSR & _BV(SPIF)));
      writeEnd();
    }
  }

  else // Font is not 2 and hence is RLE encoded
  {
    //int wp = w;  // width of character in pixels
    w *= height; // Now w is total number of pixels in the character

    if ((textsize != 1) || (textcolor == textbgcolor)) {
      if (textcolor != textbgcolor) fillRect(x, pY, width * textsize, textsize * height, textbgcolor);
      int px = 0, py = pY, tpy = pY; // To hold character block start and end column and row values
      int pc = 0; // Pixel count
      byte np = textsize * textsize; // Number of pixels in a drawn pixel

      byte tnp = 0; // Temporary copy of np for while loop
      byte ts = textsize - 1; // Temporary copy of textsize
      // Maximum font size is equivalent to 180x180 pixels in area
      // w is number of pixels to plot to fill character block
      while (pc < w)
      {
        line = pgm_read_byte(flash_address);
        flash_address++;
        if (line & 0x80) {
          line &= 0x7F;
          line++;
          if (ts) {
            px = x + textsize * (pc % width); // Keep these px and py calculations outside the loop as they are slow
            py = y + textsize * (pc / width);
          }
          else {
            px = x + pc % width; // Keep these px and py calculations outside the loop as they are slow
            py = y + pc / width;
          }
          while (line--) { // In this case the while(line--) is faster
            pc++; // This is faster than putting pc+=line before while()
            while (!(SPSR & _BV(SPIF)));
            setAddrWindow(px, py, px + ts, py + ts);
            writeBegin();
            if (ts) {
              tnp = np;
              while (tnp--) {
                while (!(SPSR & _BV(SPIF)));
                SPDR = th;
                while (!(SPSR & _BV(SPIF)));
                SPDR = tl;
              }
            }
            else {
              while (!(SPSR & _BV(SPIF)));
              SPDR = th;
              while (!(SPSR & _BV(SPIF)));
              SPDR = tl;
            }
            px += textsize;

            if (px >= (x + width * textsize))
            {
              px = x;
              py += textsize;
            }
          }
        }
        else {
          line++;
          pc += line;
        }
      }
      while (!(SPSR & _BV(SPIF)));
      writeEnd();
    }
    else // Text colour != background && textsize = 1
         // so use faster drawing of characters and background using block write
    {
      setAddrWindow(x, y, x + width - 1, y + height - 1);
      writeBegin();
      // Maximum font size is equivalent to 180x180 pixels in area
      while (w > 0)
      {
        line = pgm_read_byte(flash_address);
        flash_address++;
        if (line & 0x80) {
          line &= 0x7F;
          line++; w -= line;
#ifdef F_AS_T
          spiWrite16(textcolor, line);
#else
          while (line--) {
            while (!(SPSR & _BV(SPIF)));
            SPDR = th;
            while (!(SPSR & _BV(SPIF)));
            SPDR = tl;
          }
#endif
        }
        else {
          line++; w -= line;
#ifdef F_AS_T
          spiWrite16(textbgcolor, line);
#else
          while (line--) {
            while (!(SPSR & _BV(SPIF)));
            SPDR = bh;
            while (!(SPSR & _BV(SPIF)));
            SPDR = bl;
          }
#endif
        }
      }
      while (!(SPSR & _BV(SPIF)));
      writeEnd();
    }
  }
  // End of RLE font rendering
  return width * textsize;    // x +
}

/***************************************************************************************
** Function name:           drawNumber unsigned with size
** Description:             draw a long integer
***************************************************************************************/
int TFT_ILI9341::drawNumber(long long_num, int poX, int poY, int font)
{
  char tmp[12];
  if (long_num < 0) sprintf(tmp, "%li", long_num);
  else sprintf(tmp, "%lu", long_num);
  return drawString(tmp, poX, poY, font);
}

/***************************************************************************************
** Function name:           drawString
** Description :            draw string
***************************************************************************************/
int TFT_ILI9341::drawString(char *string, int poX, int poY, int font)
{
  int sumX = 0;

  while (*string)
  {
    int xPlus = drawChar(*string, poX, poY, font);
    sumX += xPlus;
    *string++;
    poX += xPlus;                            /* Move cursor right       */
  }
  return sumX;
}

/***************************************************************************************
** Function name:           drawCentreString
** Descriptions:            draw string centred on dX
***************************************************************************************/
int TFT_ILI9341::drawCentreString(char *string, int dX, int poY, int font)
{
  int sumX = 0;
  int len = 0;
  char *pointer = string;
  char ascii;

  while (*pointer)
  {
    ascii = *pointer;

#ifdef LOAD_GLCD
    if (font == 1)len += 6;
#endif

#ifdef LOAD_FONT2
    if (font == 2)len += 1 + pgm_read_byte(widtbl_f16 + ascii - 32);
#endif

    //if (font==3)len += 1+pgm_read_byte(widtbl_f48+ascii-32)/2;

#ifdef LOAD_FONT4
    if (font == 4)len += pgm_read_byte(widtbl_f32 + ascii - 32);// - 3;
#endif

    //if (font==5) len += pgm_read_byte(widtbl_f48+ascii-32)-3;

#ifdef LOAD_FONT6
    if (font == 6) len += pgm_read_byte(widtbl_f64 + ascii - 32);// - 3;
#endif

#ifdef LOAD_FONT7
    if (font == 7) len += pgm_read_byte(widtbl_f7s + ascii - 32);// + 2;
#endif

#ifdef LOAD_FONT8
    if (font == 8) len += pgm_read_byte(widtbl_f72 + ascii - 32);// + 2;
#endif

    *pointer++;
  }
  len = len * textsize;
  int poX = dX - len / 2;

  if (poX < 0) poX = 0;

  while (*string)
  {
    int xPlus = drawChar(*string, poX, poY, font);
    sumX += xPlus;
    *string++;
    poX += xPlus;                  /* Move cursor right            */
  }

  return sumX;
}

/***************************************************************************************
** Function name:           drawRightString
** Descriptions:            draw string right justified to dX
***************************************************************************************/
int TFT_ILI9341::drawRightString(char *string, int dX, int poY, int font)
{
  int sumX = 0;
  int len = 0;
  char *pointer = string;
  char ascii;

  while (*pointer)
  {
    ascii = *pointer;

#ifdef LOAD_GLCD
    if (font == 1)len += 6;
#endif

#ifdef LOAD_FONT2
    if (font == 2)len += 1 + pgm_read_byte(widtbl_f16 + ascii - 32);
#endif

    //if (font==3)len += 1+pgm_read_byte(widtbl_f48+ascii-32)/2;

#ifdef LOAD_FONT4
    if (font == 4)len += pgm_read_byte(widtbl_f32 + ascii - 32);// - 3;
#endif

    //if (font==5) len += pgm_read_byte(widtbl_f48+ascii-32)-3;

#ifdef LOAD_FONT6
    if (font == 6) len += pgm_read_byte(widtbl_f64 + ascii - 32);// - 3;
#endif

#ifdef LOAD_FONT7
    if (font == 7) len += pgm_read_byte(widtbl_f7s + ascii - 32);// + 2;
#endif

#ifdef LOAD_FONT8
    if (font == 8) len += pgm_read_byte(widtbl_f72 + ascii - 32);// + 2;
#endif

    *pointer++;
  }

  len = len * textsize;
  int poX = dX - len;

  if (poX < 0) poX = 0;

  while (*string)
  {
    int xPlus = drawChar(*string, poX, poY, font);
    sumX += xPlus;
    *string++;
    poX += xPlus;          /* Move cursor right            */
  }

  return sumX;
}

/***************************************************************************************
** Function name:           drawFloat
** Descriptions:            drawFloat
***************************************************************************************/
int TFT_ILI9341::drawFloat(float floatNumber, int decimal, int poX, int poY, int font)
{
  unsigned long temp = 0;
  float decy = 0.0;
  float rounding = 0.5;

  float eep = 0.000001;

  int sumX    = 0;
  int xPlus   = 0;

  if (floatNumber - 0.0 < eep)    // floatNumber < 0
  {
    xPlus = drawChar('-', poX, poY, font);
    floatNumber = -floatNumber;

    poX  += xPlus;
    sumX += xPlus;
  }

  for (unsigned char i = 0; i < decimal; ++i)
  {
    rounding /= 10.0;
  }

  floatNumber += rounding;

  temp = (long)floatNumber;


  xPlus = drawNumber(temp, poX, poY, font);

  poX  += xPlus;
  sumX += xPlus;

  if (decimal > 0)
  {
    xPlus = drawChar('.', poX, poY, font);
    poX += xPlus;                            /* Move cursor right            */
    sumX += xPlus;
  }
  else
  {
    return sumX;
  }

  decy = floatNumber - temp;
  for (unsigned char i = 0; i < decimal; i++)
  {
    decy *= 10;                                /* for the next decimal         */
    temp = decy;                               /* get the decimal              */
    xPlus = drawNumber(temp, poX, poY, font);

    poX += xPlus;                              /* Move cursor right            */
    sumX += xPlus;
    decy -= temp;
  }
  return sumX;
}


/***************************************************************************************
** Function name:           spiWrite16
** Descriptions:            Delay based assembler loop for fast SPI write
***************************************************************************************/
inline void TFT_ILI9341::spiWrite16(uint16_t data, int16_t count)
{
// We can enter this loop with 0 pixels to draw, so we need to check this
// if(count<1) { Serial.print("#### Less than 1 ####"); Serial.println(count);}

  uint8_t temp;
  __asm__ __volatile__
  (
    "	sbiw	%[count],0\n"			// test count
    //"	brmi	2f\n"					// if < 0 then done
    "	breq	2f\n"					// if == 0 then done

    "1:	out	%[spi],%[hi]\n"		// write SPI data

    "	adiw	r24,0\n"	// 2
    "	adiw	r24,0\n"	// 4
    "	adiw	r24,0\n"	// 6
    "	adiw	r24,0\n"	// 8
    "	adiw	r24,0\n"	// 10
    "	adiw	r24,0\n"	// 12
    "	adiw	r24,0\n"	// 14
    "	adiw	r24,0\n"	// 16
    "	nop	\n"		// 17

    "	out	%[spi],%[lo]\n"			// write SPI data

    "	adiw	r24,0	\n"	// 2
    "	adiw	r24,0	\n"	// 4
    "	adiw	r24,0	\n"	// 6
    "	adiw	r24,0	\n"	// 8
    "	adiw	r24,0	\n"	// 10
    "	adiw	r24,0	\n"	// 12
    "	nop	\n"		// 13

    "	sbiw	%[count],1\n"			// 15 decrement count
    "	brne	1b\n"					// 17 if != 0 then loop

    "2:\n"

    : [temp] "=d" (temp), [count] "+w" (count)
    : [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
    :
  );
}

/***************************************************************************************
** Function name:           spiWait
** Descriptions:            17 cycle delay
***************************************************************************************/
inline void TFT_ILI9341::spiWait(void)
{
  __asm__ __volatile__
  (
    "	adiw	r24,0\n"	// 2
    "	adiw	r24,0\n"	// 4
    "	adiw	r24,0\n"	// 6
    "	adiw	r24,0\n"	// 8
    "	adiw	r24,0\n"	// 10
    "	adiw	r24,0\n"	// 12
    "	adiw	r24,0\n"	// 14
    "	adiw	r24,0\n"	// 16
    "	nop	\n"		// 17
  );
}

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

 ****************************************************/