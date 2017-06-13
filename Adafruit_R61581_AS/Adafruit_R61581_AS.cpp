/***************************************************
  Original written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_R61581_AS.h"

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


// Constructor when using hardware 16bit.
Adafruit_R61581_AS::Adafruit_R61581_AS() : Adafruit_GFX_AS(R61581_TFTWIDTH, R61581_TFTHEIGHT) {

  _cs   = 40; //PORT G bit _BV(1)
  _rs   = 38; //PORT D bit _BV(7)
  _rst  = 41; //PORT G bit _BV(0)
  _wr  = 39;  //PORT G bit _BV(2)
  _fcs = 44;  //FLASH chip select?
}

void Adafruit_R61581_AS::writecommand(uint16_t c) {
CS_L;
RS_L;
PORTA = 0;
PORTC = c;
WR_STB;
RS_H;
CS_H;
}

void Adafruit_R61581_AS::writedata(uint16_t c) {
CS_L;
PORTA = c>>8;
PORTC = c;
WR_STB;
CS_H;
}

void Adafruit_R61581_AS::begin(uint8_t type) {
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, LOW);

  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);

  pinMode(_fcs, OUTPUT);
  digitalWrite(_fcs, HIGH); // Stop line floating

  DDRA = 0xFF; // Set direction for the 2 8 bit data ports
  DDRC = 0xFF;

  // toggle RST low to reset
    digitalWrite(_rst, HIGH);
    delay(10);
    digitalWrite(_rst, LOW);
    delay(10);
    digitalWrite(_rst, HIGH);
    delay(100);

// Configure display

// ILI9327 based set

    //writecommand(0xE9);
    //writedata(0x20);

    writecommand(0x11);
    delay(20);

    //writecommand(0xB0);
    //writedata(0x00);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x01);
    writedata(0x08);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x71);
    writedata(0x19);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x44);

    writecommand(0x36);
    writedata(0x48);

    writecommand(0x3A);
    writedata(0x05);

    writecommand(0xC0);
    writedata(0x00); // Bit 0 is SS = shift direction
    writedata(0x35); //Number of lines
    writedata(0x00); //Scan start
    writedata(0x00);
    writedata(0x01);
    writedata(0x02);

    writecommand(0xC1);
    writedata(0x10);
    writedata(0x10);
    writedata(0x02);
    writedata(0x02);

    //writecommand(0xC2);
    //writedata(0x10);
    //writedata(0x10);
    //writedata(0x02);
    //writedata(0x02);

     writecommand(0xC3);
     writedata(0x00);
     writedata(0x00);
     writedata(0x40); // Back porch (top)
     writedata(0x40); // Front porch (bottom)

    writecommand(0xC5);
    writedata(0x04);

    writecommand(0xC8);
    writedata(0x04);
    writedata(0x67);
    writedata(0x35);
    writedata(0x04);
    writedata(0x08);
    writedata(0x06);
    writedata(0x24);
    writedata(0x01);
    writedata(0x37);
    writedata(0x40);
    writedata(0x03);
    writedata(0x10);
    writedata(0x08);
    writedata(0x80);
    writedata(0x00);

    writecommand(0x2A);
    writedata(0x00);
    writedata(0x00);
    writedata(0x00);
    writedata(0xEF);

    writecommand(0x2B);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x8F);

    delay(120);
    writecommand(0x29);
    delay(25);

// End of display configuration

}

// Draw a character - only used for the original Adafruit font
// sub-classed here for convenience
void Adafruit_R61581_AS::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {
#ifdef LOAD_GLCD
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = pgm_read_byte(font+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
#endif
}


void Adafruit_R61581_AS::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1) {
  if (x0>x1) swap(x0,x1);
  if (y0>y1) swap(y0,y1);
  if((x0 < 0) ||(x1 >= _width) || (y0 < 0) || (y1 >= _height)) return;
  CS_L; RS_L;  PORTC = R61581_CASET; WR_STB; RS_H;
  PORTC = x0>>8; WR_STB;
  PORTC = x0; WR_STB;
  PORTC = x1>>8; WR_STB;
  PORTC = x1; WR_STB;
  RS_L; PORTC = R61581_PASET; WR_STB; RS_H;
  PORTC = y0>>8; WR_STB;
  PORTC = y0; WR_STB;
  PORTC = y1>>8; WR_STB;
  PORTC = y1; WR_STB;
  RS_L; PORTC = R61581_RAMWR; WR_STB; RS_H; CS_H;
}

void Adafruit_R61581_AS::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  CS_L; RS_L;  PORTC = R61581_CASET; WR_STB; RS_H;
  PORTA = 0; PORTC = x>>8; WR_STB;
  PORTC = x; WR_STB;
  x++; PORTC = x>>8; WR_STB;
  PORTC = x; WR_STB;
  RS_L; PORTC = R61581_PASET; WR_STB; RS_H;
  PORTC = y>>8; WR_STB;
  PORTC = y; WR_STB;
  y++; PORTA = 0; PORTC = y>>8; WR_STB;
  PORTC = y; WR_STB;
  RS_L; PORTC = R61581_RAMWR; WR_STB; RS_H;
  PORTC = color; PORTA = color>>8; WR_STB; CS_H;
}

// Issues 'raw' an array of 16-bit color values to the LCD; used
// externally by BMP examples.  Assumes that setWindowAddr() has
// previously been set to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).
void Adafruit_R61581_AS::pushColors(uint16_t *data, uint8_t len) {
  uint16_t color;
  CS_L;
  while(len--) {
    color = *data++;
    PORTA    = color; // Don't simplify or merge these
    PORTC    = color >> 8;      // lines
    WR_STB; 
  }
  CS_H;
}

void Adafruit_R61581_AS::pushColors(uint8_t *data, uint8_t len) {
  CS_L;
  while(len--) {
    PORTA    = *data++;
    PORTC    = *data++;
    WR_STB; 
  }
  CS_H;
}

void Adafruit_R61581_AS::pushColorz(uint16_t color, uint16_t len) {
  CS_L;
  PORTA    = color;
  PORTC    = color >> 8;
  while(len--) { WR_L;WR_H;}
  CS_H;
}


// Bresenham's algorithm - thx wikipedia - speed enhanced by Bodmer this uses
// the eficient FastH/V Line draw routine for segments of 2 pixels or more
void Adafruit_R61581_AS::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {

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


  int16_t err = dx>>1, ystep=-1, xs=x0, dlen=0;
  if (y0 < y1) ystep = 1;

  // Split into steep and not steep for FastH/V separation
  if(steep) {
    for (; x0<=x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen==1) drawPixel(y0, xs, color);
        else drawFastVLine(y0, xs, dlen, color);
        dlen = 0; y0 += ystep; xs=x0+1;
      }
    }
    if(dlen) drawFastVLine(y0, xs, dlen, color);
  }
  else
  {
    for (; x0<=x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen==1) drawPixel(xs, y0, color);
        else drawFastHLine(xs, y0, dlen, color);
        dlen = 0; y0 += ystep; xs=x0+1;
      }
    }
    if(dlen) drawFastHLine(xs, y0, dlen, color);
  }
}


// Fill a triangle - Bresenham method - not used! Bigger code and only a few percent faster
// but unlike Adafruit one it does not overflow so easily on large triangles!

void Adafruit_R61581_AS::fillTriangle(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3, uint16_t c) {
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

void Adafruit_R61581_AS::drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;

  if((y+h-1) >= _height) 
    h = _height-y;

  CS_L;
  RS_L;  PORTC = R61581_CASET; WR_STB; RS_H;
  PORTA = 0; PORTC = x>>8; WR_STB;
  PORTC = x; WR_STB;
  PORTC = x>>8; WR_STB;
  PORTC = x; WR_STB;
  RS_L; PORTC = R61581_PASET; WR_STB; RS_H;
  PORTC = y>>8; WR_STB;
  PORTC = y; WR_STB;
  y+=h-1; PORTC = y>>8; WR_STB;
  PORTC = y; WR_STB;
  RS_L; PORTC = R61581_RAMWR; WR_STB; RS_H;

  PORTC = color;
  PORTA = color>>8;

  while (h--) { WR_L;WR_H;}
  CS_H;
  
}

void Adafruit_R61581_AS::drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;

  CS_L;
  RS_L;  PORTC = R61581_CASET; WR_STB; RS_H;
  PORTA = 0; PORTC = x>>8; WR_STB;
  PORTC = x; WR_STB;
  x+=w-1; PORTC = x>>8; WR_STB;
  PORTC = x; WR_STB;
  RS_L; PORTC = R61581_PASET; WR_STB; RS_H;
  PORTC = y>>8; WR_STB;
  PORTC = y; WR_STB;
  PORTC = y>>8; WR_STB;
  PORTC = y; WR_STB;
  RS_L; PORTC = R61581_RAMWR; WR_STB; RS_H;

  PORTC = color;
  PORTA = color>>8;
    while(w--){ WR_L;WR_H;}
  CS_H;
  
}

void Adafruit_R61581_AS::fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}

// fill a rectangle
void Adafruit_R61581_AS::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);
  
  
  PORTA = color>>8;
  PORTC = color;
  CS_L;
  while(h--) {
    for(x=w; x>0; x--) { WR_L;WR_H;}
  }
  CS_H;
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_R61581_AS::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}



#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08

void Adafruit_R61581_AS::setRotation(uint8_t m) {

  writecommand(R61581_MADCTL);
  rotation = m % 4;
  switch (rotation) {
   case 0: // Portrait
    writedata(MADCTL_BGR | 0x02);
    writecommand(0xC0);
    writedata(0x00); // Bit 0 is SS = shift direction
    writedata(0x35); //Number of lines
    writedata(0x00); //Scan start
    writedata(0x00);
    writedata(0x01);
    writedata(0x02);
     _width  = R61581_TFTWIDTH;
     _height = R61581_TFTHEIGHT;
     break;
   case 1: // Landscape (Portrait + 90)
    writedata(MADCTL_BGR | 0x20);
    writecommand(0xC0);
    writedata(0x00); // Bit 0 is SS = shift direction
    writedata(0x35); //Number of lines
    writedata(0x00); //Scan start
    writedata(0x00);
    writedata(0x01);
    writedata(0x02);
     _width  = R61581_TFTHEIGHT;
     _height = R61581_TFTWIDTH;
     break;
   case 2: // Inverter portrait
    writedata( MADCTL_BGR);
    writecommand(0xC0);
    writedata(0x04); // Bit 0 is SS = shift direction
    writedata(0x35); //Number of lines
    writedata(0x08); //Scan starts 32 lines in
    writedata(0x00);
    writedata(0x01);
    writedata(0x02);
     _width  = R61581_TFTWIDTH;
     _height = R61581_TFTHEIGHT;
    break;
  case 3: // Inverted landscape
    writedata(MADCTL_BGR | 0x60);
    writecommand(0xC0);
    writedata(0x04); // Bit 0 is SS = shift direction
    writedata(0x35); //Number of lines
    writedata(0x08); //Scan starts 32 lines in
    writedata(0x00);
    writedata(0x01);
    writedata(0x02);
     _width  = R61581_TFTHEIGHT;
     _height = R61581_TFTWIDTH;
     break;
  }
}


void Adafruit_R61581_AS::invertDisplay(boolean i) {
  writecommand(i ? R61581_INVON : R61581_INVOFF);
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a unicode onto the screen
***************************************************************************************/
int Adafruit_R61581_AS::drawChar(unsigned int uniCode, int x, int y, int size)
{
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int flash_address = 0; // 16 bit address OK for Arduino if font files <60K

  int8_t gap = 0;
  uniCode -= 32; // Not using info from font files at the moment

  switch(size) {
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

  int t = textcolor;
  int b = textbgcolor;

  
  // Code to write 16 bits{PORTA = t>>8; PORTC = t; WR_STB;}
  CS_L;
  for(int i=0; i<height; i++)
  {
    for (int k = 0;k < w; k++)
    { 
    line = pgm_read_byte(flash_address+w*i+k);
    pX = x + k*8;
    if(line&0x80) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x40) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x20) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x10) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x8) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x4) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x2) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    if(line&0x1) {PORTA = t>>8; PORTC = t; WR_STB;}
    else {PORTA = b>>8; PORTC = b; WR_STB;}
    }
    pY+=textsize;
  }
  CS_H;
}

return (width+gap)*textsize;        // x +
}

