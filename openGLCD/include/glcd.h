/*
  glcd.h - openGLCD library support
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


#ifndef	GLCD_H
#define GLCD_H

#include <inttypes.h>
#include "openGLCD_Config.h"
#include "include/gText.h" 

/*
 * Note that while for now all these typedefs are the same they
 * may not be in the future since each is used to point to a different type of data
 */
typedef const uint8_t* Image_t; // a glcd format bitmap (includes width & height)
typedef const uint8_t* ImageXBM_t; // a "xbm" format bitmap (includes width & height)

// the first two bytes of bitmap data are the width and height
#define bitmapWidth(bitmap)  (*bitmap)  
#define bitmapHeight(bitmap)  (*(bitmap+1))  


/**
 * @class glcd
 * @brief Functions for GLCD object
 */
class glcd : public gText  
{
  private:
  public:
	glcd();
	
/** @name CONTROL FUNCTIONS
 * The following control functions are available
 */
/*@{*/
	// Control functions
	int Init(glcd_device_mode invert = NON_INVERTED);
	void SetDisplayMode(glcd_device_mode mode); //NON_INVERTED or INVERTED,   was SetInverted(uint8_t invert);
#ifdef DOXYGEN
	/*
	 * Define functions to get them to show up properly
	 * in doxygen
	 */
	void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void SetPixels(uint8_t x, uint8_t y,uint8_t x1, uint8_t y1, uint8_t color);
	uint8_t ReadData(void);
    void WriteData(uint8_t data); 
	int OnBacklight(void); 
	int OffBacklight(void); 
	int SetBacklight(int val); 
	int On(void);
	int Off(void);
	int OnDisplay(void);
	int OffDisplay(void);
#else
	using glcd_Device::SetDot;
	using glcd_Device::SetPixels;
	using glcd_Device::ReadData; 
	using glcd_Device::WriteData; 
	using glcd_Device::OnBacklight; 
	using glcd_Device::OffBacklight; 
	using glcd_Device::SetBacklight; 
	using glcd_Device::On;
	using glcd_Device::Off;
	using glcd_Device::OnDisplay;
	using glcd_Device::OffDisplay;
#endif

	void GotoXY(uint8_t x, uint8_t y);  // overrride for GotoXY in device class
/*@}*/
	
/** @name DRAWING FUNCTIONS
 * The following graphic functions are available
 */
/*@{*/
	// Graphic Functions
	void ClearScreen(uint8_t color = PIXEL_OFF);
	void DrawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color= PIXEL_ON);
	void DrawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color= PIXEL_ON);
	void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color= PIXEL_ON);
	void DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color= PIXEL_ON);
	void FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color= PIXEL_ON);
	void DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color= PIXEL_ON);
	void FillRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color= PIXEL_ON);
	void InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t color=PIXEL_ON);
	void FillTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t color=PIXEL_ON);
	void DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color= PIXEL_ON);	
	void FillCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color= PIXEL_ON);	
	void DrawEllipse(uint8_t xCenter, uint8_t yCenter, uint8_t xRadius, uint8_t yRadius, uint8_t color= PIXEL_ON);	
	void FillEllipse(uint8_t xCenter, uint8_t yCenter, uint8_t xRadius, uint8_t yRadius, uint8_t color= PIXEL_ON);	
	void DrawHBarGraph(uint8_t x, uint8_t y, int width, int height, uint8_t border, int minval, int maxval, int curval);
	void DrawVBarGraph(uint8_t x, uint8_t y, int width, int height, uint8_t border, int minval, int maxval, int curval);
	void DrawBitmap(Image_t bitmap, uint8_t x, uint8_t y, uint8_t color= PIXEL_ON);
	void DrawBitmapXBM(ImageXBM_t bitmapxbm, uint8_t x, uint8_t y, uint8_t color= PIXEL_ON);
	void DrawBitmapXBM_P(uint8_t width, uint8_t height, uint8_t *xbmbits, uint8_t x, uint8_t y, 
		uint8_t fg_color, uint8_t bg_color);


/*@}*/

	//Device Properties - these are read only constants	 
	static const uint8_t Width = DISPLAY_WIDTH; 	/**< Display width in pixels */
	static const uint8_t Height = DISPLAY_HEIGHT;	/**< Display height in pixels */
	static const uint8_t Left = 0;					/**< Left most pixel x coordinate on Display (0 zero)*/
	static const uint8_t Right = DISPLAY_WIDTH-1;	/**< Right most pixel x coordinate on Display (equals Width -1)*/
	static const uint8_t Top = 0;					/**< Top most pixel y coordinate on Display (0 zero)*/
	static const uint8_t Bottom = DISPLAY_HEIGHT-1; /**< Bottom most pixel y coordinate on Display (equals Height -1)*/
	static const uint8_t CenterX = DISPLAY_WIDTH/2;	/**< Horizontal center pixel on Display (equals Width/2)*/
	static const uint8_t CenterY = DISPLAY_HEIGHT/2;/**< Vertical center pixel on Display (equals Height/2)*/
	
};

extern glcd GLCD;   
#endif
