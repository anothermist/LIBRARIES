/***************************************************
  This is based on the library for the Adafruit R61581 Breakout
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  Heavily adapted for the cheap R61581B 16 bit
  parallel port 3.5" 400 x 240 (advertised as 400 x 270!)
  Mega board from Ali Express
 ****************************************************/

#ifndef _ADAFRUIT_R61581_AS_H
#define _ADAFRUIT_R61581_AS_H

 #include "Arduino.h"
 #include "Print.h"

#include <Adafruit_GFX_AS.h>

#include <avr/pgmspace.h>

// Toggle lines Low and High with minimal code
// Use write strobe (low pulse, 2 clocks) at end of transaction.
// There appears to be some +ve edge crosstalk coupling to line

#define WR_L PORTG&=~_BV(2)
#define WR_H PORTG|=_BV(2)
#define WR_STB PORTG&=~_BV(2);PORTG|=_BV(2)
#define CS_L PORTG&=~_BV(1)
#define CS_H PORTG|=_BV(1)
#define RS_L PORTD&=~_BV(7)
#define RS_H PORTD|=_BV(7)

#define R61581D 0xD
#define R61581B 0xB

#define R61581_TFTWIDTH  240
#define R61581_TFTHEIGHT 400
//#define R61581_TFTWIDTH  320
//#define R61581_TFTHEIGHT 240

#define R61581_NOP     0x00
#define R61581_SWRESET 0x01
#define R61581_RDDID   0x04
#define R61581_RDDST   0x09

#define R61581_RDPOWMODE  0x0A
#define R61581_RDMADCTL  0x0B
#define R61581_RDCOLMOD  0x0C
#define R61581_RDDIM  0x0D
#define R61581_RDDSDR  0x0F

#define R61581_SLPIN   0x10
#define R61581_SLPOUT  0x11
#define R61581B_PTLON   0x12
#define R61581B_NORON   0x13

#define R61581_INVOFF  0x20
#define R61581_INVON   0x21
#define R61581_DISPOFF 0x28
#define R61581_DISPON  0x29

#define R61581_CASET   0x2A
#define R61581_PASET   0x2B
#define R61581_RAMWR   0x2C
#define R61581_RAMRD   0x2E

#define R61581B_PTLAR   0x30
#define R61581_TEON  0x35
#define R61581_TEARLINE  0x44
#define R61581_MADCTL  0x36 // Changed from 0x36
#define R61581_COLMOD  0x3A

#define R61581_SETOSC 0xB0
#define R61581_SETPWR1 0xB1
#define R61581B_SETDISPLAY 0xB2
#define R61581_SETRGB 0xB3
#define R61581D_SETCOM  0xB6

#define R61581B_SETDISPMODE  0xB4
#define R61581D_SETCYC  0xB4
#define R61581B_SETOTP 0xB7
#define R61581D_SETC 0xB9

#define R61581B_SET_PANEL_DRIVING 0xC0
#define R61581D_SETSTBA 0xC0
#define R61581B_SETDGC  0xC1
#define R61581B_SETID  0xC3
#define R61581B_SETDDB  0xC4
#define R61581B_SETDISPLAYFRAME 0xC5
#define R61581B_GAMMASET 0xC8
#define R61581B_SETCABC  0xC9
#define R61581_SETPANEL  0xCC


#define R61581B_SETPOWER 0xD0
#define R61581B_SETVCOM 0xD1
#define R61581B_SETPWRNORMAL 0xD2

#define R61581B_RDID1   0xDA
#define R61581B_RDID2   0xDB
#define R61581B_RDID3   0xDC
#define R61581B_RDID4   0xDD

#define R61581D_SETGAMMA 0xE0

#define R61581B_SETGAMMA 0xC8
#define R61581B_SETPANELRELATED  0xE9



// Color definitions
#define R61581_BLACK   0x0000
#define R61581_BLUE    0x001F
#define R61581_RED     0xF800
#define R61581_GREEN   0x07E0
#define R61581_CYAN    0x07FF
#define R61581_MAGENTA 0xF81F
#define R61581_YELLOW  0xFFE0  
#define R61581_WHITE   0xFFFF


class Adafruit_R61581_AS : public Adafruit_GFX_AS {

 public:

  Adafruit_R61581_AS();

  void     begin(uint8_t),
           drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size),
           setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
           pushColor(uint16_t color),
           pushColors(uint16_t *data, uint8_t len),
           pushColors(uint8_t *data, uint8_t len),
           pushColorz(uint16_t color, uint16_t len),
           fillScreen(uint16_t color),
           drawPixel(int16_t x, int16_t y, uint16_t color),
           drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
           fillTriangle(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, uint16_t c),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
           setRotation(uint8_t r),
           invertDisplay(boolean i);

  int      drawChar(unsigned int uniCode, int x, int y, int size);

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  void     writecommand(uint16_t c),
           writedata(uint16_t d);

 private:
  int8_t  _cs, _rs, _rst, _wr, _fcs;
};

#endif
