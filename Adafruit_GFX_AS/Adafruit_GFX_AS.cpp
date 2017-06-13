/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
 
Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Update 13/2/15 by Bodmer:
Font 1 will print the Adafruit GLCD font
drawUnicode renamed drawChar
drawNumber array size increased
Faster font drawing
Update 21/2/15 by Alan Senior:
Faster line drawing algorithm added
Update 27/2/15 added font 8
Speed improvement tweaks 18/3/15
Minor bug fix 19/3/15
New comments 06/4/15
Updated write() function to print fonts via print class. 14/4/15
*/

#include "Adafruit_GFX_AS.h"

#ifdef LOAD_GLCD
  #include "glcdfont.c"
#endif

#ifdef LOAD_FONT2
  #include "Font16.h"
#endif

#ifdef LOAD_FONT4
#include "Font32.h"
#endif

#ifdef LOAD_FONT6
#include "Font64.h"
#endif

#ifdef LOAD_FONT7
  #include "Font7s.h"
#endif

#ifdef LOAD_FONT8
  #include "Font72.h"
#endif

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

Adafruit_GFX_AS::Adafruit_GFX_AS(int16_t w, int16_t h):
  WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textfont = 1;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = true;
}

// Draw a circle outline
void Adafruit_GFX_AS::drawCircle(int16_t x0, int16_t y0, int16_t r,
    uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
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

void Adafruit_GFX_AS::drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
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

void Adafruit_GFX_AS::fillCircle(int16_t x0, int16_t y0, int16_t r,
			      uint16_t color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void Adafruit_GFX_AS::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta, uint16_t color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
// This is the basic line drawing algorithm.
// To improve drawing speed it can be subclassed but then
// FastH/V line drawing functions must also be subclassed
void Adafruit_GFX_AS::drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
			    uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void Adafruit_GFX_AS::drawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h,
			    uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void Adafruit_GFX_AS::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint16_t color) {
  // Can update in subclasses to improve performance
  drawLine(x, y, x, y+h-1, color);
}

void Adafruit_GFX_AS::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint16_t color) {
  // Can update in subclasses to improve performance
  drawLine(x, y, x+w-1, y, color);
}

void Adafruit_GFX_AS::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
			    uint16_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void Adafruit_GFX_AS::fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void Adafruit_GFX_AS::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint16_t color) {
  // smarter version
  drawFastHLine(x+r  , y    , w-2*r, color); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x    , y+r  , h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void Adafruit_GFX_AS::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint16_t color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void Adafruit_GFX_AS::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

/* This was an attempt to speed up triangle drawing using Bresenham method, but not as good as hoped.... 
   future improvements pending. It works but uses goto statements!*/

// Fill a triangle - Bresenham method - not used! Bigger code and only a few percent faster!
void Adafruit_GFX_AS::fillTriangle(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, uint16_t c) {
	int16_t t1x,t2x,y,minx,maxx,t1xp,t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int16_t signx1,signx2,dx1,dy1,dx2,dy2;
	uint16_t e1,e2;
    // Sort vertices
	if (y1>y2) { swap(y1,y2); swap(x1,x2); }
	if (y1>y3) { swap(y1,y3); swap(x1,x3); }
	if (y2>y3) { swap(y2,y3); swap(x2,x3); }

	t1x=t2x=x1; y=y1;   // Starting points

	dx1 = x2 - x1; if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = y2 - y1;
 
	dx2 = x3 - x1; if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
	dy2 = y3 - y1;
	
	if (dy1 > dx1) {   // swap values
        swap(dx1,dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
        swap(dy2,dx2);
		changed2 = true;
	}
	
	e2 = dx2>>1;
    // Flat top, just process the second half
    if(y1==y2) goto next;
    e1 = dx1>>1;
	
	for (uint16_t i = 0; i < dx1;) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
        // process first line until y value is about to change
		while(i<dx1) {
			i++;			
			e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   if (changed1) t1xp=signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
	// Move line
	next1:
        // process second line until y value is about to change
		while (1) {
			e2 += dy2;		
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp=signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	drawFastHLine(minx, y, maxx-minx, c);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y==y2) break;
		
   }
	next:
	// Second half
	dx1 = x3 - x2; if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = y3 - y2;
	t1x=x2;
 
	if (dy1 > dx1) {   // swap values
        swap(dy1,dx1);
		changed1 = true;
	} else changed1=false;
	
	e1 = dx1>>1;
	
	for (uint16_t i = 0; i<=dx1; i++) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
	    // process first line until y value is about to change
		while(i<dx1) {
    		e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if(i<dx1) i++;
		}
	next3:
        // process second line until y value is about to change
		while (t2x!=x3) {
			e2 += dy2;
	   	   	while (e2 >= dx2) {
				e2 -= dx2;
				if(changed2) t2xp=signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}	   	   
	next4:

		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	drawFastHLine(minx, y, maxx-minx, c);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>y3) return;
	}
}

/*
// Fill a triangle - original Adafruit function works well and code footprint is small
void Adafruit_GFX_AS::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, uint16_t color) {

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

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
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
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}
*/
void Adafruit_GFX_AS::drawBitmap(int16_t x, int16_t y,
			      const uint8_t *bitmap, int16_t w, int16_t h,
			      uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
	drawPixel(x+i, y+j, color);
      }
    }
  }
}

#if ARDUINO >= 100
size_t Adafruit_GFX_AS::write(uint8_t uniCode) {
#else
void Adafruit_GFX_AS::write(uint8_t uniCode) {
#endif

  unsigned int width = 0;
  unsigned int height = 0;

  switch(textfont) {
  #ifdef LOAD_GLCD
    case 1:
      width = 6;
      height = 8;
      break;
  #endif

  #ifdef LOAD_FONT2
    case 2:
      width = pgm_read_byte(widtbl_f16+uniCode-32) + 1;
      height = chr_hgt_f16;
      break;
  #endif

//    case 3:
//      width = pgm_read_byte(widtbl_f24+uniCode-32);
//      height = chr_hgt_f24;
//      break;

  #ifdef LOAD_FONT4
    case 4:
      width = pgm_read_byte(widtbl_f32+uniCode-32) - 3;
      height = chr_hgt_f32;
      break;
  #endif

//    case 5:
//      width = pgm_read_byte(widtbl_f48+uniCode-32);
//      height = chr_hgt_f48;
//      break;

  #ifdef LOAD_FONT6
    case 6:
      width = pgm_read_byte(widtbl_f64+uniCode-32) - 3;
      height = chr_hgt_f64;
      break;
  #endif

  #ifdef LOAD_FONT7
    case 7:
      width = pgm_read_byte(widtbl_f7s+uniCode-32) + 2;
      height = chr_hgt_f7s;
      break;
  #endif

  #ifdef LOAD_FONT8
    case 8:
      width = pgm_read_byte(widtbl_f72+uniCode-32) + 2;
      height = chr_hgt_f72;
      break;
  #endif

    default:
    #if ARDUINO >= 100
      return 0;
    #endif
  }

  if (uniCode == '\n') {
    cursor_y += textsize*height;
    cursor_x  = 0;
  } else if (uniCode == '\r') {
    // skip
  } else {
    if (cursor_x+width*textsize > _width) {
      cursor_y += textsize*height;
      cursor_x = 0;
    }
    cursor_x += drawChar(uniCode, cursor_x, cursor_y, textfont);
  }

#if ARDUINO >= 100
  return 1;
#endif
}

// Draw a character - only used for the original Adafruit font
void Adafruit_GFX_AS::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {
#ifdef LOAD_GLCD
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

if (size == 1) {
  if (bg == color) {
    for (int8_t i=0; i<6; i++ ) {
      uint8_t line;
      if (i == 5) line = 0x0;
      else line = pgm_read_byte(font+(c*5)+i);

      for (int8_t j = 0; j<8; j++) {
        if (line & 0x1) drawPixel(x+i, y+j, color);
        line >>= 1;
      }
    }
  }
  else {
      uint8_t mask = 0x1;
      uint8_t line1,line2,line3,line4,line5;
      setAddrWindow(x, y, x+5, y+7);
      writeBegin();
      line1 = pgm_read_byte(font+(c*5)+0);
      line2 = pgm_read_byte(font+(c*5)+1);
      line3 = pgm_read_byte(font+(c*5)+2);
      line4 = pgm_read_byte(font+(c*5)+3);
      line5 = pgm_read_byte(font+(c*5)+4);

      for (int8_t j = 0; j<8; j++) {
        if (line1 & mask) {while(!(SPSR&_BV(SPIF)));SPDR=color>>8;while(!(SPSR&_BV(SPIF)));SPDR=color;}
        else {while(!(SPSR&_BV(SPIF)));SPDR=bg>>8;while(!(SPSR&_BV(SPIF)));SPDR=bg;}
        if (line2 & mask) {while(!(SPSR&_BV(SPIF)));SPDR=color>>8;while(!(SPSR&_BV(SPIF)));SPDR=color;}
        else {while(!(SPSR&_BV(SPIF)));SPDR=bg>>8;while(!(SPSR&_BV(SPIF)));SPDR=bg;}
        if (line3 & mask) {while(!(SPSR&_BV(SPIF)));SPDR=color>>8;while(!(SPSR&_BV(SPIF)));SPDR=color;}
        else {while(!(SPSR&_BV(SPIF)));SPDR=bg>>8;while(!(SPSR&_BV(SPIF)));SPDR=bg;}
        if (line4 & mask) {while(!(SPSR&_BV(SPIF)));SPDR=color>>8;while(!(SPSR&_BV(SPIF)));SPDR=color;}
        else {while(!(SPSR&_BV(SPIF)));SPDR=bg>>8;while(!(SPSR&_BV(SPIF)));SPDR=bg;}
        if (line5 & mask) {while(!(SPSR&_BV(SPIF)));SPDR=color>>8;while(!(SPSR&_BV(SPIF)));SPDR=color;}
        else {while(!(SPSR&_BV(SPIF)));SPDR=bg>>8;while(!(SPSR&_BV(SPIF)));SPDR=bg;}
        while(!(SPSR&_BV(SPIF)));SPDR=bg>>8;while(!(SPSR&_BV(SPIF)));SPDR=bg;
        mask <<= 1;
      }
    while(!(SPSR&_BV(SPIF)));
    writeEnd();
  }

} else {
  if (bg == color) {
    for (int8_t i=0; i<6; i++ ) {
      uint8_t line;
      if (i == 5) line = 0x0;
      else line = pgm_read_byte(font+(c*5)+i);

      for (int8_t j = 0; j<8; j++) {
        if (line & 0x1) fillRect(x+(i*size), y+(j*size), size, size, color);
        line >>= 1;
      }
    }
  }
  else {
    for (int8_t i=0; i<6; i++ ) {
      uint8_t line;
      if (i == 5) line = 0x0;
      else line = pgm_read_byte(font+(c*5)+i);

      for (int8_t j = 0; j<8; j++) {
        if (line & 0x1) fillRect(x+(i*size), y+(j*size), size, size, color);
        else fillRect(x+i*size, y+j*size, size, size, bg);
        line >>= 1;
      }
    }
  }
}
#endif
}

void Adafruit_GFX_AS::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void Adafruit_GFX_AS::setCursor(int16_t x, int16_t y, uint8_t font) {
  textfont = font;
  cursor_x = x;
  cursor_y = y;
}

void Adafruit_GFX_AS::setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void Adafruit_GFX_AS::setTextFont(uint8_t f) {
  textfont = (f > 0) ? f : 1;
}

void Adafruit_GFX_AS::setTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void Adafruit_GFX_AS::setTextColor(uint16_t c, uint16_t b) {
  textcolor   = c;
  textbgcolor = b; 
}

void Adafruit_GFX_AS::setTextWrap(boolean w) {
  wrap = w;
}

uint8_t Adafruit_GFX_AS::getRotation(void) {
  return rotation;
}

void Adafruit_GFX_AS::setRotation(uint8_t x) {
  rotation = (x & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}

// Return the size of the display (per current rotation)
int16_t Adafruit_GFX_AS::width(void) {
  return _width;
}
 
int16_t Adafruit_GFX_AS::height(void) {
  return _height;
}

void Adafruit_GFX_AS::invertDisplay(boolean i) {
  // Do nothing, must be subclassed if supported
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a unicode onto the screen
***************************************************************************************/
int Adafruit_GFX_AS::drawChar(unsigned int uniCode, int x, int y, int font)
{
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int flash_address = 0; // 16 bit address OK for Arduino if font files <60K

  int8_t gap = 0;
  uniCode -= 32; // Not using info from font files at the moment

  switch(font) {
  #ifdef LOAD_GLCD
    case 1:
      drawChar(x, y,uniCode+32,textcolor, textbgcolor, textsize);
      return 6*textsize;
  #endif

  #ifdef LOAD_FONT2
    case 2:
      flash_address = pgm_read_word(&chrtbl_f16[uniCode]);
      width = pgm_read_byte(widtbl_f16+uniCode);
      height = chr_hgt_f16;
      gap = 1;
      break;
  #endif

//    case 3:
//      flash_address = pgm_read_word(&chrtbl_f24[uniCode]);
//      width = pgm_read_byte(widtbl_f24+uniCode);
//      height = chr_hgt_f24;
//      gap = 0;
//      break;

  #ifdef LOAD_FONT4
    case 4:
      flash_address = pgm_read_word(&chrtbl_f32[uniCode]);
      width = pgm_read_byte(widtbl_f32+uniCode);
      height = chr_hgt_f32;
      gap = -3;
      break;
  #endif

//    case 5:
//      flash_address = pgm_read_word(&chrtbl_f48[uniCode]);
//      width = pgm_read_byte(widtbl_f48+uniCode);
//      height = chr_hgt_f48;
//      gap = -3;
//      break;

  #ifdef LOAD_FONT6
    case 6:
      flash_address = pgm_read_word(&chrtbl_f64[uniCode]);
      width = pgm_read_byte(widtbl_f64+uniCode);
      height = chr_hgt_f64;
      gap = -3;
      break;
  #endif

  #ifdef LOAD_FONT7
    case 7:
      flash_address = pgm_read_word(&chrtbl_f7s[uniCode]);
      width = pgm_read_byte(widtbl_f7s+uniCode);
      height = chr_hgt_f7s;
      gap = 2;
      break;
  #endif

  #ifdef LOAD_FONT8
    case 8:
      flash_address = pgm_read_word(&chrtbl_f72[uniCode]);
      width = pgm_read_byte(widtbl_f72+uniCode);
      height = chr_hgt_f72;
      gap = 2;
      break;
  #endif

    default:
      return 0;
  }

if (x+(width+gap)*textsize >= _width) return (width+gap)*textsize ;

int w = (width+7)/8;
int pX      = 0;
int pY      = y;
byte line = 0;

if (textcolor == textbgcolor | textsize != 1) {
  for(int i=0; i<height; i++)
  {
    if (textcolor != textbgcolor) {
      if (textsize == 1) drawFastHLine(x, pY, width+gap, textbgcolor);
      else fillRect(x, pY, (width+gap)*textsize, textsize, textbgcolor);
    }
    for (int k = 0;k < w; k++)
    { 
      line = pgm_read_byte(flash_address+w*i+k);
      if(line) {
        if (textsize==1){
          pX = x + k*8;
          if(line & 0x80) drawPixel(pX, pY, textcolor);
          if(line & 0x40) drawPixel(pX+1, pY, textcolor);
          if(line & 0x20) drawPixel(pX+2, pY, textcolor);
          if(line & 0x10) drawPixel(pX+3, pY, textcolor);
          if(line & 0x8) drawPixel(pX+4, pY, textcolor);
          if(line & 0x4) drawPixel(pX+5, pY, textcolor);
          if(line & 0x2) drawPixel(pX+6, pY, textcolor);
          if(line & 0x1) drawPixel(pX+7, pY, textcolor);
        }
        else {
          pX = x + k*8*textsize;
          if(line & 0x80) fillRect(pX, pY, textsize, textsize, textcolor);
          if(line & 0x40) fillRect(pX+textsize, pY, textsize, textsize, textcolor);
          if(line & 0x20) fillRect(pX+2*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x10) fillRect(pX+3*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x8) fillRect(pX+4*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x4) fillRect(pX+5*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x2) fillRect(pX+6*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x1) fillRect(pX+7*textsize, pY, textsize, textsize, textcolor);
        }
      }
    }
    pY+=textsize;
  }
}
else
// Faster drawing of characters and background using block write
{
  setAddrWindow(x, y, (x+w*8)-1, y+height-1);

  writeBegin();

  int t = textcolor;
  int b = textbgcolor;

  for(int i=0; i<height; i++)
  {
    for (int k = 0;k < w; k++)
    { 
    line = pgm_read_byte(flash_address+w*i+k);
    pX = x + k*8;
    if(line&0x80) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x40) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x20) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x10) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x8) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x4) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x2) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    if(line&0x1) {while(!(SPSR&_BV(SPIF)));SPDR=t>>8;while(!(SPSR&_BV(SPIF)));SPDR=t;}
    else {while(!(SPSR&_BV(SPIF)));SPDR=b>>8;while(!(SPSR&_BV(SPIF)));SPDR=b;}
    }
    pY+=textsize;
  }
  while(!(SPSR&_BV(SPIF)));
  writeEnd();
}

return (width+gap)*textsize;        // x +
}

/***************************************************************************************
** Function name:           Fast character drawing support functions
***************************************************************************************/

void Adafruit_GFX_AS::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  // Do nothing, MUST be subclassed
}

void Adafruit_GFX_AS::writeBegin()
{
  // Do nothing, MUST be subclassed
} 

void Adafruit_GFX_AS::writeEnd()
{
  // Do nothing, MUST be subclassed
} 


/***************************************************************************************
** Function name:           drawNumber unsigned with size
** Description:             draw a long integer
***************************************************************************************/
int Adafruit_GFX_AS::drawNumber(long long_num,int poX, int poY, int font)
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
int Adafruit_GFX_AS::drawString(char *string, int poX, int poY, int font)
{
    int sumX = 0;

    while(*string)
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
int Adafruit_GFX_AS::drawCentreString(char *string, int dX, int poY, int font)
{
    int sumX = 0;
    int len = 0;
    char *pointer = string;
    char ascii;

    while(*pointer)
    {
        ascii = *pointer;

#ifdef LOAD_GLCD
        if (font==1)len += 6;
#endif
#ifdef LOAD_FONT2
        if (font==2)len += 1+pgm_read_byte(widtbl_f16+ascii-32);
#endif
        //if (font==3)len += 1+pgm_read_byte(widtbl_f48+ascii-32)/2;
#ifdef LOAD_FONT4
        if (font==4)len += pgm_read_byte(widtbl_f32+ascii-32)-3;
#endif
        //if (font==5) len += pgm_read_byte(widtbl_f48+ascii-32)-3;
#ifdef LOAD_FONT6
        if (font==6) len += pgm_read_byte(widtbl_f64+ascii-32)-3;
#endif
#ifdef LOAD_FONT7
        if (font==7) len += pgm_read_byte(widtbl_f7s+ascii-32)+2;
#endif
#ifdef LOAD_FONT8
        if (font==8) len += pgm_read_byte(widtbl_f72+ascii-32)+2;
#endif

        *pointer++;
    }
    len = len*textsize;
    int poX = dX - len/2;

    if (poX < 0) poX = 0;

    while(*string)
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
int Adafruit_GFX_AS::drawRightString(char *string, int dX, int poY, int font)
{
    int sumX = 0;
    int len = 0;
    char *pointer = string;
    char ascii;

    while(*pointer)
    {
        ascii = *pointer;

#ifdef LOAD_GLCD
        if (font==1)len += 6;
#endif
#ifdef LOAD_FONT2
        if (font==2)len += 1+pgm_read_byte(widtbl_f16+ascii-32);
#endif
        //if (font==3)len += 1+pgm_read_byte(widtbl_f48+ascii-32)/2;
#ifdef LOAD_FONT4
        if (font==4)len += pgm_read_byte(widtbl_f32+ascii-32)-3;
#endif
        //if (font==5) len += pgm_read_byte(widtbl_f48+ascii-32)-3;
#ifdef LOAD_FONT6
        if (font==6) len += pgm_read_byte(widtbl_f64+ascii-32)-3;
#endif
#ifdef LOAD_FONT7
        if (font==7) len += pgm_read_byte(widtbl_f7s+ascii-32)+2;
#endif
#ifdef LOAD_FONT8
        if (font==8) len += pgm_read_byte(widtbl_f72+ascii-32)+2;
#endif

        *pointer++;
    }
    
    len = len*textsize;
    int poX = dX - len;

    if (poX < 0) poX = 0;

    while(*string)
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
int Adafruit_GFX_AS::drawFloat(float floatNumber, int decimal, int poX, int poY, int font)
{
    unsigned long temp=0;
    float decy=0.0;
    float rounding = 0.5;
    
    float eep = 0.000001;
    
    int sumX    = 0;
    int xPlus   = 0;
    
    if(floatNumber-0.0 < eep)       // floatNumber < 0
    {
        xPlus = drawChar('-',poX, poY, font);
        floatNumber = -floatNumber;

        poX  += xPlus; 
        sumX += xPlus;
    }
    
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    
    floatNumber += rounding;

    temp = (long)floatNumber;
    
    
    xPlus = drawNumber(temp,poX, poY, font);

    poX  += xPlus; 
    sumX += xPlus;

    if(decimal>0)
    {
        xPlus = drawChar('.',poX, poY, font);
        poX += xPlus;                            /* Move cursor right            */
        sumX += xPlus;
    }
    else
    {
        return sumX;
    }
    
    decy = floatNumber - temp;
    for(unsigned char i=0; i<decimal; i++)                                      
    {
        decy *= 10;                                /* for the next decimal         */
        temp = decy;                               /* get the decimal              */
        xPlus = drawNumber(temp,poX, poY, font);
        
        poX += xPlus;                              /* Move cursor right            */
        sumX += xPlus;
        decy -= temp;
    }
    return sumX;
}
