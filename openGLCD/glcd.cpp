/*
  glcd.cpp - openGLCD library support
  Copyright (c) 2011-2016 Bill Perry
  Copyright (c) 2008,2009,2010 Michael Margolis and Bill Perry

  vi:ts=4
  
 ------------------------------------------------------------------------

  This file is part of the openGLCD library project

  openGLCD is free software: you can redistribute it and/or modify
  it under the terms of version 3 of the GNU General Public License
  as published by the Free Software Foundation.

  openGLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with openGLCD.  If not, see <http://www.gnu.org/licenses/>.

 -----------------------------------------------------------------------

  The glcd class implements high level graphics routines.

  Several of the routines were originally inspired from code written and
  copyright by Fabian Maximilian Thiele.
  You can obtain a copy of his original work here:
  http://www.scienceprog.com/wp-content/uploads/2007/07/glcd_ks0108.zip
 
  
*/

#include "include/glcd.h"

#define BITMAP_FIX // enables a bitmap rendering fix/patch


glcd::glcd()
{
	glcd_Device::Inverted = NON_INVERTED; 
}

/**
 * Initilize library and device hardware
 *
 * @copydetails glcd_Device::Init()
 */

int glcd::Init(glcd_device_mode invert)
{
	return(glcd_Device::Init(invert));  
}		

/**
 * Clear the lcd display
 *
 * @param color
 *
 * Sets all the pixels on the display from \ref glcd::Left "GLCD.Left",
 * \ref glcd::Top "GLCD.Top" to \ref glcd::Right "GLCD.Right",
 * \ref glcd::Bottom "GLCD.Bottom" to the specified color.
 *
 * Color is optional and defaults to ::PIXEL_OFF
 *
 * @note
 * If the display is in ::NON_INVERTED mode, then the color ::PIXEL_OFF or ::WHITE will 
 * turn off all the pixels,
 * If the display is in ::INVERTED mode, then the color ::PIXEL_OFF or ::WHITE will 
 * turn on all the pixels,
 *
 */

void glcd::ClearScreen(uint8_t color)
{
//#define OLDWAY
#ifdef OLDWAY
	this->SetPixels(0,0,GLCD.Width-1,GLCD.Height-1, color);
#else
	for(uint8_t row = 0; row < GLCD.Height; row += 8)
	{
		GotoXY(0, row);
		for(uint8_t col = 0; col < GLCD.Width; col++)
		{
			GLCD.WriteData(color);
		}
	}
#endif
 	CursorToXY(0,0);  // home text position
}

/*
 * Drawline code is based on Bresenham's line algorithm
 * http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *
 * This routine replaces the Teiele's DrawLine() routine which was
 * much larger and much slower.
 *	-- bperrybap
 */
 
/*
 * First, define a few macros to make the DrawLine code below read more like
 * the wikipedia example code.
 */

#define _GLCD_absDiff(x,y) ((x>y) ?  (x-y) : (y-x))
#define _GLCD_swap(a,b) \
do\
{\
uint8_t t;\
	t=a;\
	a=b;\
	b=t;\
} while(0)

/**
 * Draw a line
 *
 * @param x1 a value from 0 to GLCD.Width-1  indicating start x coordinate 
 * @param y1 a value fron 0 to GLCD.Height-1 indicating start y coordinate
 * @param x2 a value from 0 to GLCD.Width-1  indicating end x coordinate 
 * @param y2 a value fron 0 to GLCD.Height-1 indicating end y coordinate
 * @param color
 *
 * Draw a line starting at x1,y2 and ending at x2,y2.
 *
 * Color is optional and defaults to ::PIXEL_ON.
 *
 * @see DrawHLine()
 * @see DrawVLine()
 *
 */

void glcd::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
uint8_t deltax, deltay, x,y, steep;
int8_t error, ystep;

#ifdef XXX
	/*
	 * Rather than fudge up coordinates that are outside of range,
	 * It is better to allow the algorithm to run with them "as is"
	 * so that any pixels on the line that land inside the valid display 
	 * coordinates will be drawn properly.
	 *
	 * If the coordinates are patched, then the line drawn will not
	 * be anything close to the original slope as the endpoint coordinates
	 * will have been altered.
	 * - bperrybap
	 */

	if(x1>=DISPLAY_WIDTH) x1=0;
	if(x2>=DISPLAY_WIDTH) x2=0;
	if(y1>=DISPLAY_HEIGHT) y1=0;
	if(y2>=DISPLAY_HEIGHT) y2=0;
#endif

	steep = _GLCD_absDiff(y1,y2) > _GLCD_absDiff(x1,x2);  

	if ( steep )
	{
		_GLCD_swap(x1, y1);
		_GLCD_swap(x2, y2);
	}

	if (x1 > x2)
	{
		_GLCD_swap(x1, x2);
		_GLCD_swap(y1, y2);
	}

	deltax = x2 - x1;
	deltay =_GLCD_absDiff(y2,y1);  
	error = deltax / 2;
	y = y1;
	if(y1 < y2) ystep = 1;  else ystep = -1;

	for(x = x1; x <= x2; x++)
	{
		if (steep) this->SetDot(y,x, color); else this->SetDot(x,y, color);
		error = error - deltay;
		if (error < 0)
		{
			y = y + ystep;
			error = error + deltax;
		}
	}
}

/**
 * Draw a rectangle of given width and height
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color
 *
 * Draw a rectangle with the specified width and height.
 * The upper left corner at x,y and the lower right
 * corner at x+width-1, y+height-1.\n
 * The left and right edges of the rectangle are at x and x+width-1.\n
 * The top and bottom edges of the rectangle ar at y and y+height -1.\n
 *
 * The length of the horizontal sides will be width pixels
 * The length of the vertical sides will be height pixels
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see FillRect()
 * @see InvertRect()
 * @see DrawRoundRect()
 */

void glcd::DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	DrawHLine(x, y, width, color);				// top
	DrawHLine(x, y+height-1, width, color);		// bottom
	DrawVLine(x, y, height, color);				// left
	DrawVLine(x+width-1, y, height, color);		// right
}

/**
 * Fill a Rectangle
 * 
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color
 *
 * Fill a rectanglular area of the specified width and height.
 *
 * The resulting rectangle covers an area @em width pixels wide by @em height pixels 
 * tall starting from the pixel at x,y. 
 *
 * The upper left corner at x,y and the lower right corner at x+width-1,y+width-1.
 *
 *
 * The length of the horizontal sides will be width pixels
 * The length of the vertical sides will be height pixels
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawRect()
 * @see InvertRect()
 */

void glcd::FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	this->SetPixels(x,y,x+width-1,y+height-1,color);
}

/**
 * Draw a rectangle with rounded corners
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param radius radius of rounded corners
 * @param color 
 *
 * Draw a rectangle the same as DrawRect() but with rounded corners.
 * Radius is a value from 1 to half the smaller of height or width of the rectangle.
 * The upper left corner at x,y and the lower right
 * corner at x+width-1, y+width-1.\n
 * The left and right edges of the rectangle are at x and x+width-1.\n
 * The top and bottom edges of the rectangle ar at y and y+height-1.\n
 *
 * @see DrawRect() FillRoundRect
 */

void glcd::DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color)
{
  	int16_t tSwitch; 
	uint8_t x1 = 0, y1 = radius;
  	tSwitch = 3 - 2 * radius;
	
	while (x1 <= y1)
	{
		// upper left corner
		this->SetDot(x+radius - x1, y+radius - y1, color); // upper half
		this->SetDot(x+radius - y1, y+radius - x1, color); // lower half

		
		// upper right corner
		this->SetDot(x+width-radius-1 + x1, y+radius - y1, color); // upper half
		this->SetDot(x+width-radius-1 + y1, y+radius - x1, color); // lower half

		// lower right corner
		this->SetDot(x+width-radius-1 + x1, y+height-radius-1 + y1, color); // lower half
		this->SetDot(x+width-radius-1 + y1, y+height-radius-1 + x1, color); // upper half

		// lower left corner
		this->SetDot(x+radius - x1, y+height-radius-1 + y1, color); // lower half
		this->SetDot(x+radius - y1, y+height-radius-1 + x1, color); // upper half

		if (tSwitch < 0)
		{
			tSwitch += (4 * x1 + 6);
		}
		else
		{
			tSwitch += (4 * (x1 - y1) + 10);
			y1--;
		}
		x1++;
	}
	  	
	this->DrawHLine(x+radius, y, width-(2*radius), color);			// top
	this->DrawHLine(x+radius, y+height-1, width-(2*radius), color);	// bottom
	this->DrawVLine(x, y+radius, height-(2*radius), color);			// left
	this->DrawVLine(x+width-1, y+radius, height-(2*radius), color);	// right
}
/**
 * Fill a rectangle with rounded corners
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param radius radius of rounded corners
 * @param color 
 *
 * Fill a rectanglular area with rounded corners with the same outline
 * as DrawRoundRect()\n
 * Radius is a value from 1 to half the smaller of height or width of the rectangle.
 *
 * @see DrawRoundRect DrawRect() FillRect()
 * 
 */

/*
 * Filling rounded rectangles kind of sucks as there is no good way to do it,
 * The code below draws vertical lines between the upper and lower corners.
 * Then fills in the remaining rectangle.
 * -- bperrybap
 */
void glcd::FillRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color)
{
  	int16_t tSwitch; 
	uint8_t x1 = 0, y1 = radius;
  	tSwitch = 3 - 2 * radius;
	
	// center block
	// filling center block first makes it apear to fill faster
	FillRect(x+radius, y, width-2*radius, height, color);

	while (x1 <= y1)
	{
		// left side
		DrawLine(
			x+radius - x1, y+radius - y1,			// upper left corner upper half
			x+radius - x1, y+height-radius-1 + y1,	// lower left corner lower half
			color);
		DrawLine(
			x+radius - y1, y+radius - x1,			// upper left corner lower half
			x+radius - y1, y+height-radius-1 + x1,	// lower left corner upper half
			color);

		// right side
		DrawLine(
			x+width-radius-1 + x1, y+radius - y1,			// upper right corner upper half
			x+width-radius-1 + x1, y+height-radius-1 + y1, // lower right corner lower half
			color);
		DrawLine(
			x+width-radius-1 + y1, y+radius - x1,			// upper right corner lower half
			x+width-radius-1 + y1, y+height-radius-1 + x1,	// lower right corner upper half
			color);

	    if (tSwitch < 0) {
	    	tSwitch += (4 * x1 + 6);
	    } else {
	    	tSwitch += (4 * (x1 - y1) + 10);
	    	y1--;
	    }
	    x1++;
	}
}

/**
 * Invert a Rectangular area
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width in pixels of the rectangular area
 * @param height height in pixels of the rectangular area
 *
 * Invert the pixels in the rectanglular area of the specified width and height.
 * ::BLACK pixels become ::WHITE and ::WHITE pixels become ::BLACK.
 *
 * The upper left corner at x,y and the lower right
 * corner at x+width-1,y+width-1.
 *
 * The length of the horizontal sides will be width pixels
 * The length of the vertical sides will be height pixels
 *
 * @see DrawRect()
 * @see FillRect()
 */


void glcd::InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	uint8_t mask, pageOffset, h, i, data, tmpData;
	
	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset) {
		mask >>= (8-height);
		h = height;
	} else {
		h = 8-pageOffset;
	}
	mask <<= pageOffset;
	
	/*
	 * First do the fractional pages at the top of the region
	 */
	glcd_Device::GotoXY(x, y);
	for(i=0; i<width; i++) {
		data = this->ReadData();
		tmpData = ~data;
		data = (tmpData & mask) | (data & ~mask);
		this->WriteData(data);
	}
	
	/*
	 * Now do the full pages
	 */
	while(h+8 <= height)
	{
		h += 8;
		y += 8;
		glcd_Device::GotoXY(x, y);
		
		for(i=0; i<width; i++) {
			data = this->ReadData();
			this->WriteData(~data);
		}
	}
	
	/*
	 * Now do the fractional pages aat the bottom of the region
	 */
	if(h < height) {
		mask = ~(0xFF << (height-h));
		glcd_Device::GotoXY(x, y+8);
		
		for(i=0; i<width; i++) {
			data = this->ReadData();
			tmpData = ~data;
			data = (tmpData & mask) | (data & ~mask);
			this->WriteData(data);
		}
	}
}
/**
 * Draw a triangle
 *
 * @param x1 the x coordinate of point 1
 * @param y1 the y coordinate of point 1
 * @param x2 the x coordinate of point 2
 * @param y2 the y coordinate of point 2
 * @param x3 the x coordinate of point 3
 * @param y3 the y coordinate of point 3
 * @param color
 *
 * Draw a triangle defined by the 3 points
 *
 * color is an optional parameter indicating pixel color, default is ::::PIXEL_ON
 *
 * @see FillTriangle()
 */

void glcd::DrawTriangle(uint8_t x1, uint8_t y1,
				uint8_t x2, uint8_t y2,
				uint8_t x3, uint8_t y3, uint8_t color)
{
  DrawLine(x1, y1, x2, y2, color);
  DrawLine(x2, y2, x3, y3, color);
  DrawLine(x3, y3, x1, y1, color);
}

/**
 * Fill a triangle
 *
 * @param x1 the x coordinate of point 1
 * @param y1 the y coordinate of point 1
 * @param x2 the x coordinate of point 2
 * @param y2 the y coordinate of point 2
 * @param x3 the x coordinate of point 3
 * @param y3 the y coordinate of point 3
 * @param color
 *
 * Fill a triangle formed by the 3 specified points.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @warning:
 * This routine currently uses floating point, which will
 * pull in the floating point support code.
 *
 * @see DrawTriangle()
 */


/*
 * Code adapted from the code on a blog by Fahad Uddin
 * (He has approved it's use in this library, under GPL licensing)
 * Designation: Computer Information and Systems Engineer
 * Email: engg_fahd@hotmail.com
 * http://mycsnippets.blogspot.com/2010/12/program-to-fill-triangle-in-c-language.html
 * 
 * Note: This code requires floating point. If floating point is not used
 * the calculations are not precise enough to create the proper line lengths to
 * fully fill the tiangular area.
 *	--- bperrybap
 */

void glcd::FillTriangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t x3,uint8_t y3, uint8_t color)
{
uint8_t sl,sx1,sx2;
double m1,m2,m3;
	if(y2>y3)
	{
		_GLCD_swap(x2,x3);
		_GLCD_swap(y2,y3);
	}
	if(y1>y2)
	{
		_GLCD_swap(x1,x2);
		_GLCD_swap(y1,y2);
	}
	m1=(double)(x1-x2)/(y1-y2);
	m2=(double)(x2-x3)/(y2-y3);
	m3=(double)(x3-x1)/(y3-y1);
	for(sl=y1;sl<=y2;sl++)
	{
		sx1= m1*(sl-y1)+x1;
		sx2= m3*(sl-y1)+x1;
		if(sx1>sx2)
			_GLCD_swap(sx1,sx2);
		DrawLine(sx1, sl, sx2, sl, color);
	}
	for(sl=y2;sl<=y3;sl++)
	{
		sx1= m2*(sl-y3)+x3;
		sx2= m3*(sl-y1)+x1;
		if(sx1>sx2)
			_GLCD_swap(sx1,sx2);
		DrawLine(sx1, sl, sx2, sl, color);
	}
}

/**
 * Set LCD Display mode
 *
 * @param invert Inverted mode
 *
 * Sets the graphical state mode for the entire LCD display 
 *
 * In "normal" or @b ::NON_INVERTED mode, pixels are turned on for ::PIXEL_ON or ::BLACK
 * and turned off for ::PIXEL_OFF or ::WHITE.
 *
 * In "inverted" or @b ::INVERTED mode, pixels are turned off for ::PIXEL_ON or ::BLACK
 * and turned on for ::PIXEL_OFF or ::WHITE.
 *
 */

void glcd::SetDisplayMode(glcd_device_mode invert)	// was named SetInverted
{

	if(this->Inverted != invert)
	{
		this->InvertRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		this->Inverted = invert;
	}
}

/**
 * Draw a glcd bitmap image
 *
 * @param bitmap a ponter to the bitmap data
 * @param x the x coordinate of the upper left corner of the bitmap
 * @param y the y coordinate of the upper left corner of the bitmap
 * @param color
 *
 * Draw a bitmap image with the upper left corner at location x,y
 * The bitmap data is assumed to be in program memory.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawBitmapXBM()
 */

void glcd::DrawBitmap(Image_t bitmap, uint8_t x, uint8_t y, uint8_t color)
{
uint8_t width, height;
uint8_t i, j;

  width = ReadPgmData(bitmap++); 
  height = ReadPgmData(bitmap++);

#ifdef BITMAP_FIX // temporary ifdef just to show what changes if a new 
				// bit rendering routine is written.
							
  /*
   * In the absence of a new/better/proper bitmap rendering routine,
   * this routine will provide a temporary fix for drawing bitmaps that are
   * are non multiples of 8 pixels in height and start on non LCD page Y addresses.
   *
   * Note: nomally a routine like this should not have knowledge of how
   *	   how the lower level write routine works. But in this case it does.
   *
   *	Currently, the low level WriteData() routine ORs in the pixels when data spans
   *	a LCD memory page. So.....
   *
   * In order to ensure that the bitmap data is written to the pixels *exactly* as it
   * it defined in the bitmap data, i.e every black pixels is black and every white
   * pixel is white,...
   *
   * If height or y coordinate is not on a page boundary, clear the background first
   *	Techincally, this could be done all the time and it wouldn't hurt, it
   *	would just suck up a few more cycles.
   */
  if( (y & 7) || (height & 7))
  {
  	this->FillRect(x, y, width, height, PIXEL_OFF);
  }
  for(j = 0; j < ((height+7) / 8); j++)
#else
  for(j = 0; j < height / 8; j++)
#endif
  {
	glcd_Device::GotoXY(x, y + (j*8) );
	for(i = 0; i < width; i++)
	{
		 uint8_t displayData = ReadPgmData(bitmap++);
	   	 if(color == BLACK)
			this->WriteData(displayData);
		 else
		    this->WriteData(~displayData);
	}
  }
}


/**
 * Draw a glcd bitmap image in x11 XBM bitmap data format
 *
 * @param bitmapxbm a ponter to the glcd XBM bitmap data
 * @param x the x coordinate of the upper left corner of the bitmap
 * @param y the y coordinate of the upper left corner of the bitmap
 * @param color
 *
 * Draw a x11 XBM bitmap image with the upper left corner at location x,y
 * The glcd xbm bitmap data format consists of 1 byte of width followed by 1 byte of height followed
 * by the x11 xbm pixel data bytes.
 * The bitmap data is assumed to be in program memory.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawBitmapXBM_P()
 * @see DrawBitmap()
 */

void glcd::DrawBitmapXBM(ImageXBM_t bitmapxbm, uint8_t x, uint8_t y, uint8_t color)
{
uint8_t width, height;
uint8_t bg_color;
uint8_t *xbmbits;

	
	xbmbits = (uint8_t *) bitmapxbm;

	width = ReadPgmData(xbmbits++); 
	height = ReadPgmData(xbmbits++);

	if(color == BLACK)
		bg_color = WHITE;
	else
		bg_color = BLACK;
	DrawBitmapXBM_P(width, height, xbmbits, x, y, color, bg_color);
}

/**
 * Draw a x11 XBM bitmap image
 *
 * @param width pixel width of the image
 * @param height pixel height of the image
 * @param xbmbits a ponter to the XBM bitmap pixel data
 * @param x the x coordinate of the upper left corner of the bitmap
 * @param y the y coordinate of the upper left corner of the bitmap
 * @param fg_color foreground color
 * @param bg_color background color
 *
 * Draw a x11 XBM bitmap image with the upper left corner at location x,y
 * The xbm bitmap pixel data format is the same as the X11 bitmap pixel data.
 * The bitmap data is assumed to be in program memory.
 *
 * @note All parameters are mandatory
 *
 * @see DrawBitmapXBM()
 * @see DrawBitmap()
 */


void glcd::DrawBitmapXBM_P(uint8_t width, uint8_t height, uint8_t *xbmbits, 
			uint8_t x, uint8_t y, uint8_t fg_color, uint8_t bg_color)
{
uint8_t xbmx, xbmy;
uint8_t xbmdata = 0; // FIXME? assignment unneeded, but eliminates a stupid warning

	/*
	 * Traverse through the XBM data byte by byte and plot pixel by pixel
	 */
	for(xbmy = 0; xbmy < height; xbmy++)
	{
		for(xbmx = 0; xbmx < width; xbmx++)
		{
			if(!(xbmx & 7))	// read the flash data only once per byte
				xbmdata = ReadPgmData(xbmbits++);

			if(xbmdata & _BV((xbmx & 7)))
				this->SetDot(x+xbmx, y+xbmy, fg_color); // XBM 1 bits are fg color
			else
				this->SetDot(x+xbmx, y+xbmy, bg_color); // XBM 0 bits are bg color
		}
	}

}

/**
 * Draw a Vertical Line
 *
 * @param x a value from 0 to GLCD.Width-1
 * @param y a value from 0 to GLCD.Height-1
 * @param height a value from 1 to GLCD.Height-y
 * @param color color of line
 *
 * Draw a vertical line of width pixels starting at x,y
 * Resulting line is height pixels.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawLine()
 * @see DrawHLine()
 */
 

void glcd::DrawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color)
{
	this->SetPixels(x,y,x,y+height-1,color);
}
	
/**
 * Draw a Horizontal Line
 *
 * @param x a value from 0 to GLCD.Width-1
 * @param y a value from 0 to GLCD.Height-1
 * @param width a value from 1 to GLCD.Width-x
 * @param color
 *
 * Draw a horizontal line of width pixels starting at x,y
 * Resulting line is width pixels.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawLine()
 * @see DrawVLine()
 */

void glcd::DrawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color)
{
	this->SetPixels(x,y, x+width-1, y, color);
}

/**
 * Draw a Circle
 *
 * @param xCenter X coordinate of the center of the circle
 * @param yCenter Y coordinate of the center of the circle
 * @param radius radius of circle
 * @param color
 *
 * Draw a circle of the given radius extending out radius pixels
 * from the center pixel.
 * The circle will fit inside a rectanglular area bounded by
 * x-radius,y-radius and x+radius,y+radius
 *
 * The diameter of the circle is radius*2 +1 pixels.
 *
 * Color is optional and defaults to ::PIXEL_ON.
 *
 * @see FillCircle()
 * @see FillEllipse()
 * @see DrawEllipse()
 */
void glcd::DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color)
{
	this->DrawRoundRect(xCenter-radius, yCenter-radius, 2*radius+1, 2*radius+1, radius, color);
}

/**
 * Draw a Filled in a Circle
 *
 * @param xCenter X coordinate of the center of the circle
 * @param yCenter Y coordinate of the center of the circle
 * @param radius radius of circle
 * @param color
 *
 * Draw a filled in circle of the given radius extending out from
 * the center pixel.
 *
 * See DrawCircle() for the full details on sizing.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawCircle()
 * @see DrawEllipse()
 * @see FillEllipse()
 *
 */

void glcd::FillCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color)
{
/*
 * Circle fill Code is merely a modification of the midpoint
 * circle algorithem which is an adaption of Bresenham's line algorithm
 * http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 * http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *
 * While it looks different it is the same calculation that is in
 * DrawRoundRect()
 *
 * Note: FillCircle()
 *		could be modified to work like DrawCircle() by using
 *		a DrawRoundRect() function.
 * 		DrawRoundRect() first plots the cicrular quadrants and then draws
 *		the horizontal and verticl lines between them to create
 *		a rounded rectangle.
 *		
 *		For fills of rounded rectangles, this type of circle fill algorithm
 *		would fill the upper and lower quadrants on each side of the rounded
 *		rectangle by connecting them vertically.
 *		When that was done, a filled rectangle would be neded need to fill the 
 *		space between them.
 *		There really isn't an easy way to fill a rounded rectangle.
 *
 *		For now, it is limited to circles.
 *
 * 			--- bperrybap
 */

int f = 1 - radius;
int ddF_x = 1;
int ddF_y = -2 * radius;
uint8_t x = 0;
uint8_t y = radius;
 
	/*
	 * Fill in the center between the two halves
	 */
	DrawLine(xCenter, yCenter-radius, xCenter, yCenter+radius, color);
 
	while(x < y)
	{
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		/*
		 * Now draw vertical lines between the points on the circle rather than
		 * draw the points of the circle. This draws lines between the 
		 * perimeter points on the upper and lower quadrants of the 2 halves of the circle.
		 */

		DrawLine(xCenter+x, yCenter+y, xCenter+x, yCenter-y, color);
		DrawLine(xCenter-x, yCenter+y, xCenter-x, yCenter-y, color);
		DrawLine(xCenter+y, yCenter+x, y+xCenter, yCenter-x, color);
		DrawLine(xCenter-y, yCenter+x, xCenter-y, yCenter-x, color);
  	}
}

/*
 * Internal ellipse routine to plot points on perimeter or connet the points when filling
 */
	
void static _Plot4EllipsePoints(long CX,long  CY, long X, long Y, uint8_t color, uint8_t fill)
{

	if(fill)
	{ // to fill rather than draw a line, plot between the points
		GLCD.DrawLine(CX+X, CY+Y, CX-X, CY+Y, color);
		GLCD.DrawLine(CX-X, CY-Y, CX+X, CY-Y, color);
	}
	else
	{
		GLCD.SetDot(CX+X, CY+Y, color); //{point in quadrant 1}
		GLCD.SetDot(CX-X, CY+Y, color); //{point in quadrant 2}
		GLCD.SetDot(CX-X, CY-Y, color); //{point in quadrant 3}
		GLCD.SetDot(CX+X, CY-Y, color); //{point in quadrant 4}
	}
}

/*
 * Internal ellipse routine
 */

void static _DrawEllipse(long CX, long  CY, long XRadius,long YRadius, uint8_t color, uint8_t fill)
{
// ported the algorithm by John Kennedy found at
// http://homepage.smc.edu/kennedy_john/belipse.pdf
//

  long X, Y;
  long XChange, YChange;
  long EllipseError;
  long TwoASquare,TwoBSquare;
  long StoppingX, StoppingY;
  TwoASquare = 2*XRadius*XRadius;
  TwoBSquare = 2*YRadius*YRadius;
  X = XRadius;
  Y = 0;
  XChange = YRadius*YRadius*(1-2*XRadius);
  YChange = XRadius*XRadius;
  EllipseError = 0;
  StoppingX = TwoBSquare*XRadius;
  StoppingY = 0;

  while ( StoppingX >=StoppingY ) //first set of points,y'>-1
  {
    _Plot4EllipsePoints(CX,CY,X,Y,color, fill);
    Y++;
    StoppingY=StoppingY+ TwoASquare;
    EllipseError = EllipseError+ YChange;
    YChange=YChange+TwoASquare;
    if ((2*EllipseError + XChange) > 0 ) {
      X--;
      StoppingX=StoppingX- TwoBSquare;
      EllipseError=EllipseError+ XChange;
      XChange=XChange+TwoBSquare;
    }
  }
  //{ first point set is done; start the 2nd set of points }

  Y = YRadius;
  X = 0;
  YChange = XRadius*XRadius*(1-2*YRadius);
  XChange = YRadius*YRadius;
  EllipseError = 0;
  StoppingY = TwoASquare*YRadius;
  StoppingX = 0;
  while ( StoppingY >=StoppingX ) //{2nd set of points, y'< -1}
  {
    _Plot4EllipsePoints(CX,CY,X,Y,color, fill);
    X++;
    StoppingX=StoppingX + TwoBSquare;
    EllipseError=EllipseError+ XChange;
    XChange=XChange+TwoBSquare;
    if ((2*EllipseError + YChange) > 0 ) {
      Y--;
      StoppingY=StoppingY- TwoASquare;
      EllipseError=EllipseError+ YChange;
      YChange=YChange+TwoASquare;
    }
  }
};

/**
 * Draw an ellipse
 *
 * @param xCenter X coordinate of the center of the ellipse
 * @param yCenter Y coordinate of the center of the ellipse
 * @param xRadius horizontal radius of ellipse
 * @param yRadius vertical radius of ellipse
 * @param color
 *
 * Draw an ellipse of the 2 given radi extending out from
 * the center pixel.
 * The ellipse will fit inside a rectanglular area bounded by
 * x-radius,y-radius and x+radius,y+radius
 * Final ellipse dimensions will be xRadius*2 +1 by yRadius*2 +1 pixels
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawCircle()
 * @see FillCircle()
 * @see FillEllipse()
 */
void glcd::DrawEllipse(uint8_t xCenter, uint8_t  yCenter, uint8_t xRadius,uint8_t yRadius, uint8_t color)
{
	_DrawEllipse(xCenter, yCenter, xRadius,yRadius, color, 0); // no fill
}

/**
 * Draw a filled in ellipse
 *
 * @param xCenter X coordinate of the center of the ellipse
 * @param yCenter Y coordinate of the center of the ellipse
 * @param xRadius horizontal radius of ellipse
 * @param yRadius vertical radius of ellipse
 * @param color
 *
 * Draw a filled in ellipse of the 2 given radi extending out from
 * the center pixel.
 * Final ellipse dimensions will be xRadius*2 +1 by yRadius*2 +1 pixels
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * @see DrawCircle()
 * @see FillCircle()
 * @see FillEllipse()
 */
void glcd::FillEllipse(uint8_t xCenter, uint8_t  yCenter, uint8_t xRadius, uint8_t yRadius, uint8_t color)
{
	_DrawEllipse(xCenter, yCenter, xRadius, yRadius, color, 1); // filled
}

/**
 * Draw a Horizontal BarGraph
 *
 * @param x X coordinate of the corner of the bargraph
 * @param y Y coordinate of the corner of the bargraph
 * @param width Width in pixels of bargraph, including border pixels
 * @param height Height in pixels of bargraph, including border pixels
 * @param border Border pixels around bargraph (use zero for no border)
 * @param minval Minimum value of the bargraph
 * @param maxval Maximum value of the bargraph
 * @param curval Current value of the bargraph
 *
 * Draw a horizontal bargraph.\n
 * If @em width is positive, the graph will be drawn to the right of the x coordinate
 * and the graph will advance from left to right as @em curval increases towards @em maxval. \n
 * If @em width is negative, the graph will be drawn to the left of the x coordinate
 * and the graph will advance from right to left as @em curval increases towards @em maxval. \n
 * If @em height is positive, the graph will be drawn below the y coordinate.\n
 * If @em height is negative, the graph will be drawn above the y coordinate.
 *
 * @b Examples:
 *
 * @code
 *
 * // Draw Horizontal bar graph:
 * // 10 pixels from left edge of display 
 * // 21 pixels from top
 * // (GLCD.Width -20) pixels wide advancing to right of x
 * // 5 pixels in height droping down from y
 * // 1 pixel border,
 * // with a range of 0 to BARMAXVAL
 * GLCD.DrawHBarGraph(GLCD.Left+10, GLCD.Top+21, GLCD.Width-2*10, 5, 1, 0, BARMAXVAL, curval);
 *
 *
 * // Draw Horizontal bar graph:
 * // 10 pixels from right edge of display 
 * // 6 pixels from bottom
 * // (GLCD.Width -20) pixels wide advancing to left of x
 * // 5 pixels in height rising up from y
 * // 1 pixel border,
 * // with a range of 0 to BARMAXVAL
 * GLCD.DrawHBarGraph(GLCD.Right-10, GLCD.Bottom-6, -(GLCD.Width-2*10), -5, 1, 0, BARMAXVAL, curval);
 *
 * @endcode

 * @note
 * While the behavior of @em height may at first seem backwards, it is defined with
 * respect to the coordinate system.\n
 * The y value in the coordinate system increases as the pixel location moves down
 * on the display.
 *
 * @see DrawVBarGraph()
 */
void glcd::DrawHBarGraph(uint8_t x, uint8_t y, int width, int height, uint8_t border, 
	int minval, int maxval, int curval)
{
unsigned int busypix; // pixels in "busy" part
uint8_t gulx, guly; // bargraph upper left corner coordinates
uint8_t glrx, glry; // bargraph lower right corner coordinates

	/*
	 * set up border and graph coordinates
	 * x,y will be adjusted to upper left corner of border
	 * gulx,guly will be set to upper left corner of graph (sans border)
	 * glrx,glry will be set to lower right corner of graph (sans border)
	 */

	if(height < 0)
	{
		glry = y - border;
		y = y + height + 1;
		guly = y + border;
	}
	else
	{
		// y is ok
		guly = y + border;
		glry = y + height -1 - border;
	}

	if(width < 0)
	{
		glrx = x - border;
		x = x + width +1;
		gulx = x + border;
	}
	else
	{
		// x is ok
		gulx = x + border;
		glrx = x + width - 1 - border;
	}

	/*
	 * Draw border pixels
	 */

	if(border)
	{
		for(uint8_t i = 0; i < border; i++)
		{
			this->DrawRect(x+i, y+i, abs(width)-2*i, abs(height)-2*i);
		}
	}

	/*
	 * Draw "busy" and "not busy" bargraph pixels
	 */

	busypix = map(curval, minval, maxval, 0, abs(width) -2*border);

	if(width < 0)
	{
		// bar advances to left
		if(busypix)
			this->SetPixels(glrx-busypix+1, guly, glrx, glry, PIXEL_ON);
		if(curval < maxval)
			this->SetPixels(gulx, guly, glrx-busypix, glry, PIXEL_OFF);
	}
	else
	{
		// bar advances to right
		if(busypix)
			this->SetPixels(gulx, guly, gulx+busypix-1, glry, PIXEL_ON);
		if(curval < maxval)
			this->SetPixels(gulx+busypix+1, guly, glrx, glry, PIXEL_OFF);
	}
}


/**
 * Draw a Vertical BarGraph
 *
 * @param x X coordinate of the corner of the bargraph
 * @param y Y coordinate of the corner of the bargraph
 * @param width Width in pixels of bargraph, including border pixels
 * @param height Height in pixels of bargraph, including border pixels
 * @param border Border pixels around bargraph (use zero for no border)
 * @param minval Minimum value of the bargraph
 * @param maxval Maximum value of the bargraph
 * @param curval Current value of the bargraph
 *
 * Draw a vertical bargraph.\n
 * If @em width is positive, the graph will be drawn to the right of the x coordinate \n
 * If @em width is negative, the graph will be drawn to the left of the x coordinate \n
 * If @em height is positive, the graph will be drawn below the y coordinate and
 * and the graph will advance down from the top as @em curval increases towards @em maxval.\n
 * If @em height is negative, the graph will be drawn above the y coordinate and
 * and the graph will advance up from the bottom as @em curval increases towards @em maxval.
 *
 * @b Examples:
 *
 * @code
 * // Draw vertical bar graph:
 * // 10 pixels from left edge of display
 * // 4 pixels up from bottom
 * // 5 pixels wide to right of x,
 * // (HEIGHT - 8) pixels in height advancing up from y
 * // 1 pixel border,
 * // with a range of 0 to BARMAXVAL
 * GLCD.DrawVBarGraph(GLCD.Left+10, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 1, 0, BARMAXVAL, curval);
 *
 * // Draw vertical bar graph:
 * // 10 pixels from right edge of display 
 * // 4 pixels down from top
 * // 5 pixels wide to left of x,
 * // (HEIGHT - 8) pixels in height advancing down from y
 * // 1 pixel border,
 * // with a range of 0 to BARMAXVAL
 * GLCD.DrawVBarGraph(GLCD.Right-10, GLCD.Top+4, -5, (GLCD.Height-2*4), 1, 0, BARMAXVAL, curval);
 *
 * @endcode
 * @note
 * While the behavior of @em height may at first seem backwards, it is defined with
 * respect to the coordinate system.\n
 * The y value in the coordinate system increases as the pixel location moves down
 * on the display.
 *
 * @see DrawHBarGraph()
 */

void glcd::DrawVBarGraph(uint8_t x, uint8_t y, int width, int height, uint8_t border, 
	int minval, int maxval, int curval)
{
unsigned int busypix; // pixels in "busy" part
uint8_t gulx, guly; // bargraph upper left corner coordinates
uint8_t glrx, glry; // bargraph lower right corner coordinates

	/*
	 * set up border and graph coordinates
	 * x,y will be adjusted to upper left corner of border
	 * gulx,guly will be set to upper left corner of bargraph (sans border)
	 * glrx,glry will be set to lower right corner of bargraph (sans border)
	 */

	if(height < 0)
	{
		glry = y - border;
		y = y + height + 1;
		guly = y + border;
	}
	else
	{
		// y is ok
		guly = y + border;
		glry = y + height -1 - border;
	}

	if(width < 0)
	{
		glrx = x - border;
		x = x + width +1;
		gulx = x + border;
	}
	else
	{
		// x is ok
		gulx = x + border;
		glrx = x + width - 1 - border;
	}

	/*
	 * Draw border pixels
	 */

	if(border)
	{
		for(uint8_t i = 0; i < border; i++)
		{
			this->DrawRect(x+i, y+i, abs(width)-2*i, abs(height)-2*i);
		}
	}

	/*
	 * Draw "busy" and "not busy" bargraph pixels
	 */

	busypix = map(curval, minval, maxval, 0, abs(height) -2*border);

	if(height < 0)
	{
		// bar advances up
		if(busypix)
			this->SetPixels(gulx, glry-busypix+1, glrx, glry, PIXEL_ON);
		if(curval < maxval)
			this->SetPixels(gulx, guly, glrx, glry-busypix, PIXEL_OFF);
	}
	else
	{
		// bar advances down
		if(busypix)
			this->SetPixels(gulx, guly, glrx, guly+busypix-1, PIXEL_ON);
		if(curval < maxval)
			this->SetPixels(gulx, guly+busypix+1, glrx, glry, PIXEL_OFF);
	}
}

//
// Font Functions
//

uint8_t ReadPgmData(const uint8_t* ptr)  // note this is a static function
{  // note this is a static function
	return pgm_read_byte(ptr);
}

/*
 * Below here are text wrapper functions
 */

// override GotoXY to also call CursorToxy for backward compability 
// with older v2 ks0108 library
// (older library didn't have seperate x & y for hardware/graphics vs text )

/**
 * Set x,y coordinates for both Graphics and Text
 *
 * @copydetails glcd_Device::GotoXY()
 */
void glcd::GotoXY(uint8_t x, uint8_t y)
{
	glcd_Device::GotoXY(x, y);
  	CursorToXY(x,y); 
} 

// Make one instance for the user
glcd GLCD = glcd();
