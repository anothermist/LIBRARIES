/*
  gText.cpp - openGLCD library support - Text output routines
  Copyright (c) 2011-2016  Bill Perry
  Copyright (c) 2009,2010  Bill Perry and Michael Margolis

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

*/

#include "include/gText.h"
#include "include/glcd_errno.h"

#ifndef GLCDCFG_NO_PRINTF
#ifdef __AVR__
extern "C"
{
#include <stdio.h>
}
#else
#include <stdio.h>
#include <stdarg.h> // ARM tools don't include this in stdio.h WTF!
#endif
#endif

static FontCallback	FontRead;	// font callback shared across all instances


#ifdef GLCDCFG_UTF8
/**
 * UTF8 character decoding
 *
 * @param c next character in UTF8 stream to process
 *
 * UTF8 character decode processing
 * For now this is a crude hack that assumes no threading or reentrancy
 * and that the UTF character codes are only 0-255.
 * To go beyond 255 is a BIG deal since the font header currently only supports up to
 * 256 characters. (it is only an 8 bit field)
 * Also by only supporting 0-255 codes, when UTF8 processing is enabled, raw code processing
 * can also be supported for characters above 0x7f with the exception that codes
 * 0xc2 and 0xc3 will be lost.
 *
 * This code can process each byte seperately, or as a single 16bit UTF8 code.
 * This allows using UTF8 wide character codes directly which is what is used
 * when using a single UTF8 literal character.
 *
 * @returns < 0 if processing still on going, otherwise the final decoded character code.
 */

int
gText::UTF8decode(wchar_t c)
{
static uint8_t ucode = 0; // decoded character code

	if((uint16_t)c > (uint8_t)0xff) // support sending UTF8 code as one 16 bit value
	{
		UTF8decode((uint8_t)(c >> 8));
		return(UTF8decode((uint8_t)(c & 0xff)));
	}

	if((c == 0xc2) || (c == 0xc3)) // is it a UTF 80-7ff code point start?
	{
		ucode = (c & 0x1f) << 6;
		return(-1); // still decoding
	}
	else
	{
		if(ucode)
			c = (c & 0x3f) | ucode; // create final code point
	}
	ucode = 0; // done decoding
	return(c);
}
#endif
	   

/**
 * Constructor creates a default gText text area object with no font
 *
 * Constructor creates a text area using the entire display but does not
 * assign any font to it.
 * SelectFont() must be used to select a font for the area before any text can
 * be output to the display.
 *
 * @b Examples:
 * @code
 *	// text area using full display
 *	gText textarea;
 * @endcode
 *
 * @see \ref gText::DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode) "DefineArea()"
 * @see ClearArea()
 * @see SetAreaMode()  ClearAreaMode()
 * @see gTextMode
 */
gText::gText()
{
    this->DefineArea(0,0,DISPLAY_WIDTH -1,DISPLAY_HEIGHT -1, DEFAULT_gTEXTMODE); // this should never fail
}

/**
 * Constructor creates a gText text area object by coordinates
 *
 * @param x1 X coordinate of upper left corner
 * @param y1 Y coordinate of upper left corner
 * @param x2 X coordinate of lower right corner
 * @param y2 Y coordinate of lower right corner
 * @param mode a value from @ref gTextMode
 *
 * Creates a gText area object and then calls
 * \ref DefineArea(predefinedArea selection, Font_t font, gTextMode mode) "DefineArea()"
 * with the same parameters to initalize it.
 *
 * @b Examples:
 * @code
 *	// upper left quardrant
 *	textarea.DefineArea(GLCD.Left, GLCD.Top, GLCD.CenterX, GLCD.CenterY);
 *
 *	// Right half, reverse scroll
 *	textarea.DefineArea(GLCD.CenterX, GLCD.Top, GLCD.Right, GLCD.Bottom, gTextMode_SCROLLDOWN);
 * @endcode
 * @note
 *
 * @see \ref gText::DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode) "DefineArea()"
 * @see ClearArea()
 * @see SetAreaMode()  ClearAreaMode()
 * @see gTextMode
 */

gText::gText(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode) 
{
	this->DefineArea(x1,y1,x2,y2,mode);
}

/**
 * Constructor creates a gText text area object by predefined area without font
 *
 * @param selection a value from @ref predefinedArea
 * @param mode a value from @ref gTextMode
 *
 * Creates a gText area object and then calls
 * \ref DefineArea(predefinedArea selection, gTextMode mode) "DefineArea()"
 * with the same parameters to initalize it.
 * Constructor does not assign any font to it.
 * SelectFont() must be used to select a font for the area before any text can
 * be output to the display.
 *
 * @b Examples:
 * @code
 *	// right side of display
 *	textarea.DefineArea(textAreaRIGHT);
 *
 *	// top half of display
 *	textarea.DefineArea(textAreaTOP);
 *
 *	// upper left quadrant reverse scroll
 *	textarea.DefineArea(textAreaTOPLEFT, gTextMode_SCROLLDOWN);
 * @endcode
 *
 * @see \ref DefineArea(predefinedArea selection, gTextMode mode) "DefineArea()"
 * @see ClearArea()
 * @see SetAreaMode()  ClearAreaMode()
 * @see gTextMode
 */
gText::gText(predefinedArea selection, gTextMode mode)
{
	this->DefineArea(selection,mode);
}

/**
 * Constructor creates a gText text area object by predefined area and font
 *
 * @param selection a value from @ref predefinedArea
 * @param font a font definition
 * @param mode a value from @ref gTextMode
 *
 * Creates a gText area object and then calls
 * \ref DefineArea(predefinedArea selection, Font_t font, gTextMode mode) "DefineArea()"
 * with the same parameters to initalize it.
 *
 * @b Examples:
 * @code
 *	// right side of display
 *	textarea.DefineArea(textAreaRIGHT, System5x7);
 *
 *	// top half of display
 *	textarea.DefineArea(textAreaTOP, System5x7);
 *
 *	// upper left quadrant reverse scroll
 *	textarea.DefineArea(textAreaTOPLEFT, System5x7, gTextMode_SCROLLDOWN);
 * @endcode
 *
 * @see \ref DefineArea(predefinedArea selection, Font_t font, gTextMode mode) "DefineArea()"
 * @see ClearArea()
 * @see SetAreaMode()  ClearAreaMode()
 * @see gTextMode
 */
gText::gText(predefinedArea selection, Font_t font, gTextMode mode)
{
	this->DefineArea(selection,font, mode);
}

/**
 * Constructor creates a gText text area object by columns and rows
 *
 * @param x X coordinate of upper left corner
 * @param y Y coordinate of upper left corner
 * @param columns number of text columns
 * @param rows number of text rows
 * @param font a font definition
 * @param mode a value from @ref gTextMode
 *
 *
 * Creates a gText area object and then calls
 * \ref DefineArea(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode) "DefineArea()"
 * with the same parameters to initalize it.
 *
 * @b Examples:
 * @code
 *	// top of right side of display, 10 columns by 2 lines
 *	gText textarea(GLCD.CenterX, GLCD.Top, 10, 2, System5x7);
 *
 *	// top of right side of display, 10 columns by 2 lines, reverse scroll
 *	gText textarea(GLCD.CenterX, GLCD.Top, 10, 2, System5x7, gTextMode_SCROLLDOWN);
 * @endcode
 *
 * @see \ref DefineArea(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode) "DefineArea()"
 * @see ClearArea()
 * @see SetAreaMode()  ClearAreaMode()
 * @see gTextMode
 */

gText::gText(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode)
{
	this->DefineArea(x,y,columns,rows,font, mode);
}

/**
 * Clear text area with the current font background color
 * and home the cursor to upper left corner of the text area.
 *
 * @see DefineArea()
 */
void gText::ClearArea(void)
{
uint8_t color;

	/*
	 * If no font then no font color has been set yet, so assume the background
	 * is to be cleared using the default font color which is all pixels off.
	 */

	if((this->Font == 0) || this->FontColor == PIXEL_ON )
		color = PIXEL_OFF;
	else
		color = PIXEL_ON;

	/*
	 * fill the area with font background color
	 */
	glcd_Device::SetPixels(this->tarea.x1, this->tarea.y1, 
	this->tarea.x2, this->tarea.y2, color);

	/*
	 * put cursor at home position of text area to ensure we are always inside area.
	 */

	 this->CursorToXY(0,0);
}

/**
 * Define a Text area by columns and rows
 *
 * @param x X coordinate of upper left corner
 * @param y Y coordinate of upper left corner
 * @param columns number of text columns
 * @param rows number of text rows
 * @param font a font definition
 * @param mode a value from @ref gTextMode
 *
 * Define a text area sized to hold columns characters across and rows
 * characters tall.
 * It is properly sized for the specified font.
 * 
 * The area within the newly defined text area is intentionally not cleared.
 *
 * While intended for fixed width fonts, sizing will work for variable
 * width fonts.
 *
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 *
 * x,y is an absolute coordinate and is relateive to the 0,0 origin of the
 * display.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @returns 0 with the desired area defined if all the coordinates are valid,
 * otherwise returns non zero error code with the area set to the full display
 *
 * @b Examples:
 * @code
 *	// top of right side of display, 10 columns by 2 lines
 *	textarea.DefineArea(GLCD.CenterX, GLCD.Top, 10, 2, System5x7);
 *
 *	// top of right side of display, 10 columns by 2 lines, reverse scrolling
 *	textarea.DefineArea(GLCD.CenterX, GLCD.Top, 10, 2, System5x7, gTextMode_SCROLLDOWN);
 * @endcode
 * @note
 * Upon defining the text area, the cursor position for the text area will be
 * set to x,y
 *
 * @see ClearArea()
 * @see SetAreaMode()  ClearAreaMode()
 * @see gTextMode
 * @see \ref gText(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode) "gText()"
 */

uint8_t
gText::DefineArea(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode)
{
uint8_t x2,y2;
uint8_t width, height;

	this->SelectFont(font);

	width = FontRead(font+FONT_FIXED_WIDTH);
	height = FontRead(font+FONT_HEIGHT);

	if(!isNoPadFixedFont(font)) // all fonts except NoPadFixed fonts get pad pixels
	{
		width++;
		height++;
	}

	x2 = x + columns * width -1;
	y2 = y + rows * height -1;

	return this->DefineArea(x, y, x2, y2, mode);
}

/**
 * Define a text area by absolute coordinates
 *
 * @param x1 X coordinate of upper left corner
 * @param y1 Y coordinate of upper left corner
 * @param x2 X coordinate of lower right corner
 * @param y2 Y coordinate of lower right corner
 * @param mode a value from @ref gTextMode
 *
 * Define a text area based on absolute coordinates.
 * The pixel coordinates for the text area are inclusive so x2,y2 is the lower right
 * pixel of the text area.
 *
 * x1,y1 and x2,y2 are an absolute coordinates and are relateive to the 0,0 origin of the
 * display.
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @returns 0 with the desired area defined if all the coordinates are valid,
 * otherwise returns non zero error code with the area set to the full display
 *

 * @b Examples:
 * @code
 *	// upper left quardrant
 *	textarea.DefineArea(GLCD.Left, GLCD.Top, GLCD.CenterX, GLCD.CenterY);
 *
 *	// Right half, reverse scroll
 *	textarea.DefineArea(GLCD.CenterX, GLCD.Top, GLCD.Right, GLCD.Bottom, gTextMode_SCROLLDOWN);
 * @endcode
 * @note
 * Upon creation of the text area, the cursor position for the text area will be set to x1, y1
 *
 * @see ClearArea()
 * @see SetAreaMode()
 * @see ClearAreaMode()
 * @see gTextMode
 * @see \ref gText::DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode) "gText()"
 *
 */

uint8_t
gText::DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode)
{
uint8_t ret = GLCD_ENOERR; // assume call will work

	if(		(x1 >= x2)
		||	(y1 >= y2)
		||	(x1 >= DISPLAY_WIDTH)
		||	(y1 >= DISPLAY_HEIGHT)
		||	(x2 >= DISPLAY_WIDTH)
		||	(y2 >= DISPLAY_WIDTH)
	)
	{
	    // failed sanity check so set defaults and return false 
		this->tarea.x1 = 0;
		this->tarea.y1 = 0;
		this->tarea.x2 = DISPLAY_WIDTH -1;
		this->tarea.y2 = DISPLAY_HEIGHT -1;
		this->tarea.mode = DEFAULT_gTEXTMODE;
		ret = GLCD_EINVAL;
	} 		
	else
	{  
	    this->tarea.x1 = x1; 
	    this->tarea.y1 = y1; 
		this->tarea.x2 = x2; 
	    this->tarea.y2 = y2; 		
		this->tarea.mode = mode; // not yet sanity checked
	}		
	/*
	 * set cursor position for the area
	 */
	this->x = x1;
	this->y = y1;	
	
#ifndef GLCDCFG_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when re defining areas.
	 */
	this->need_scroll = 0;
#endif
    return ret;
}

/**
 * Define a predefined generic text area
 *
 * @param selection a value from @ref predefinedArea
 * @param mode a value from @ref gTextMode
 *
 * Define a text area using a selection form a set of predefined areas
 * without setting or altering the area's font.
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @returns @em 0 with the desired area defined if all the coordinates are valid,
 * otherwise returns non zero error code with the area set to the full display
 *
 * @b Examples:
 * @code
 *	// right side of display
 *	textarea.DefineArea(textAreaRIGHT);
 *
 * // top half of display
 *	textarea.DefineArea(textAreaTOP);
 *
 *	// upper left quadrant reverse scroll
 *	textarea.DefineArea(textAreaTOPLEFT, gTextMode_SCROLLDOWN);
 * @endcode
 *
 * @note
 * Upon defining the text area, the cursor position for the text area will be set to 
 * the upper left coordinate of the given predefined area
 *
 * @see ClearArea()
 * @see SetAreaMode()
 * @see ClearAreaMode()
 * @see predefinedArea
 * @see gTextMode
 * @see \ref gText::gText(predefinedArea selection, Font_t font, gTextMode mode) "gText()"
 *
 */

uint8_t
gText::DefineArea(predefinedArea selection, gTextMode mode)
{
uint8_t x1,y1,x2,y2;
TareaToken tok;

	tok.token = selection;

	x1 =  tok.coord.x1;
	y1 =  tok.coord.y1;
	x2 =  tok.coord.x2;
	y2 =  tok.coord.y2;

	return this->DefineArea(x1,y1,x2,y2, mode);
}

/**
 * Define a predefined generic text area and font
 *
 * @param selection a value from @ref predefinedArea
 * @param font a font definition
 * @param mode a value from @ref gTextMode
 *
 * Define a text area using a selection from a set of predefined areas.
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * @returns @em 0 with the desired area defined if all the coordinates are valid,
 * otherwise returns non zero error code with the area set to the full display
 *
 * @b Examples:
 * @code
 *	// right side of display
 *	textarea.DefineArea(textAreaRIGHT, System5x7);
 *
 *	// top half of display
 *	textarea.DefineArea(textAreaTOP, System5x7);
 *
 *	// upper left quadrant reverse scroll
 *	textarea.DefineArea(textAreaTOPLEFT, System5x7, gTextMode_SCROLLDOWN);
 * @endcode
 *
 * @note
 * Upon defining the text area, the cursor position for the text area will be set to 
 * the upper left coordinate of the given predefined area
 *
 * @see ClearArea()
 * @see SetAreaMode() 
 * @see ClearAreaMode()
 * @see predefinedArea
 * @see gTextMode
 *
 */

uint8_t
gText::DefineArea(predefinedArea selection, Font_t font, gTextMode mode)
{
	this->SelectFont(font);
	return this->DefineArea(selection, mode);
}


/**
 * Get Text Area property
 *
 * @param property a value from @ref gTextProp_t
 * Get Text Area property
 *
 * @returns the value of the specified text area property.
 *
 * @b Examples:
 * @code
 * height = GLCD.GetAreaProp(gTextProp_FontHeight);   // font rendered height
 *   cols = GLCD.GetAreaProp(gTextProp_cols);         // text columns
 *   rows = GLCD.GetAreaProp(gTextProp_rows);         // text rows
 *   cols = textarea.GetAreaProp(gTextProp_cols);     // text columns
 *   rows = textarea.GetAreaProp(gTextProp_rows);     // text rows
 *    x1  = textarea.GetAreaProp(gTextProp_x1);       // upper left x coordinate
 *    y1  = textarea.GetAreaProp(gTextProp_y1);       // upper left y coordinate
 *    x2  = textarea.GetAreaProp(gTextProp_x2);       // lower right x coordinate
 *    y2  = textarea.GetAreaProp(gTextProp_y2);       // lower right y coordinate
 * width  = textarea.GetAreaProp(gTextProp_FontWidth);// font rendered fixed width
 *  minC  = textarea.GetAreaProp(gTextProp_minC);     // minimum font character code
 *  maxC  = textarea.GetAreaProp(gTextProp_maxC);     // maximum font character code
 * @endcode
 *
 * @note
 * The value returned for gTextProp_cols is based on the fixed width 
 * maximum size of the font. 
 * If a variable font is used, the true number of
 * actual columns possible will vary depending on the characters rendered.
 *
 * gTextProp_FontWidth and gTextProp_FontHeight return rendered sizes
 * in pixels which include any inter character padding pixels.
 *
 *
 * @see DefineArea()
 * @see CharHeight()
 * @see CharWidth()
 *
 */

int
gText::GetAreaProp(gTextProp_t property)
{
int rval;

	switch(property)
	{
		case gTextProp_x1:
			rval = this->tarea.x1;
			break;

		case gTextProp_y1:
			rval = this->tarea.y1;
			break;

		case gTextProp_x2:
			rval = this->tarea.x2;
			break;

		case gTextProp_y2:
			rval = this->tarea.y2;
			break;

		case gTextProp_cols:
    		if(this->Font)
			{
				rval = (this->tarea.x2 - this->tarea.x1+1) / GetAreaProp(gTextProp_FontWidth);
			}
			else
			{
				rval = 0;
			}

			break;

		case gTextProp_rows:
    		if(this->Font)
				rval = (this->tarea.y2 - this->tarea.y1+1) / GetAreaProp(gTextProp_FontHeight);
			else
				rval = 0;
			break;

		case gTextProp_FontWidth: // always returns font's rendered max fixed width
			if(this->Font)
			{
				rval = FontRead(this->Font+FONT_FIXED_WIDTH);
				if(isFixedWidthPaddedFont(this->Font))  // there is 1 pixel pad on right
					rval++;
			}
			else
			{
				rval = 0;
			}
			break;

		case gTextProp_FontHeight:
			rval = this->CharHeight(0);
			break;

		case gTextProp_minC:
			return(FontRead(this->Font+FONT_FIRST_CHAR));
			break;

		case gTextProp_maxC:
			return(FontRead(this->Font+FONT_FIRST_CHAR) + FontRead(this->Font+FONT_CHAR_COUNT) -1);
			break;

		default:
			rval = 0;
	}

	return(rval);
}




/*
 * Scroll a pixel region up.
 * 	Area scrolled is defined by x1,y1 through x2,y2 inclusive.
 *  x1,y1 is upper left corder, x2,y2 is lower right corner.
 *
 *	color is the color to be used for the created space along the
 *	bottom.
 *
 *	pixels is the *exact* pixels to scroll. 1 is 1 and 9 is 9 it is
 *  not 1 less or 1 more than what you want. It is *exact*.
 */

void gText::ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, 
	uint8_t pixels, uint8_t color)
{
uint8_t dy;
uint8_t dbyte;
uint8_t sy;
uint8_t sbyte;
uint8_t col;

	/*
	 * Scrolling up more than area height?
	 */
	if(y1 + pixels > y2)
	{
		/*
		 * fill the region with "whitespace" because
		 * it is being totally scrolled out.
		 */
		glcd_Device::SetPixels(x1, y1, x2, y2, color);
		return;
	}

	for(col = x1; col <= x2; col++)
	{
		dy = y1;
		glcd_Device::GotoXY(col, dy & ~7);
		dbyte = glcd_Device::ReadData();


		/*
		 * preserve bits outside/above scroll region
		 */

		dbyte &= (_BV((dy & 7)) - 1);

		sy = dy + pixels;
		glcd_Device::GotoXY(col, sy & ~7);
		sbyte = glcd_Device::ReadData();

		while(sy <= y2)
		{
			if(sbyte & _BV(sy & 7))
			{
				dbyte |= _BV(dy & 7);
			}

			sy++;
			if((sy & 7) == 0)
			{
				/*
				 * If we just crossed over, then we should be done.
				 */
				if(sy < DISPLAY_HEIGHT)
				{
					glcd_Device::GotoXY(col, sy & ~7);
					sbyte = glcd_Device::ReadData();
				}
			}

			if((dy & 7) == 7)
			{
				glcd_Device::GotoXY(col, dy & ~7);	// Should be able to remove this
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy++;
		}

		/*
		 * Handle the new area at the bottom of the region
		 */

		for(uint8_t p = pixels; p; p--)
		{
			if(color == BLACK)
			{
				dbyte |= _BV(dy & 7);
			}
			else
			{
				dbyte &= ~_BV(dy & 7);
			}

			if((dy & 7) == 7)
			{
				glcd_Device::GotoXY(col, dy & ~7); // should be able to remove this.
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy++;
		}

		/*
		 * Flush out the final destination byte
		 */


		if(dy & 7)
		{
			dy--;

			glcd_Device::GotoXY(col, dy & ~7);
			sbyte = glcd_Device::ReadData();
			/*
			 * Preserver bits outside/below region
			 */

			dy++;
			sbyte &= ~(_BV((dy & 7)) - 1);
			dbyte |= sbyte;

			glcd_Device::WriteData(dbyte);
		}
	}

}

#ifndef GLCDCFG_NO_SCROLLDOWN

void gText::ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, 
	uint8_t pixels, uint8_t color)
{
uint8_t dy;
uint8_t dbyte;
uint8_t sy;
uint8_t sbyte;
uint8_t col;

	/*
	 * Scrolling up more than area height?
	 */
	if(y1 + pixels > y2)
	{
		/*
		 * fill the region with "whitespace" because
		 * it is being totally scrolled out.
		 */
		glcd_Device::SetPixels(x1, y1, x2, y2, color);
		return;
	}

	/*
	 * Process region from left to right
	 */
	for(col = x1; col <= x2; col++)
	{
		dy = y2;
		glcd_Device::GotoXY(col, dy & ~7);
		dbyte = glcd_Device::ReadData();

		/*
		 * preserve bits outside/below scroll region
		 */

		dbyte &= ~(_BV(((dy & 7)+1)) - 1);
		sy = dy - pixels;
		glcd_Device::GotoXY(col, sy & ~7);
		sbyte = glcd_Device::ReadData();

		while(sy >= y1)
		{
			if(sbyte & _BV(sy & 7))
			{
				dbyte |= _BV(dy & 7);
			}
			if((dy & 7) == 0)
			{
				glcd_Device::GotoXY(col, dy & ~7);	// Should be able to remove this
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy--;

			if(!sy)
				break; /* if we bottomed out, we are done */
			sy--;
			if((sy & 7) == 7)
			{
				glcd_Device::GotoXY(col, sy & ~7);
				sbyte = glcd_Device::ReadData();
			}

		}

		/*
		 * Handle the new area at the top of the column
		 */

		for(uint8_t p = pixels; p; p--)
		{
			if(color == BLACK)
			{
				dbyte |= _BV(dy & 7);
			}
			else
			{
				dbyte &= ~_BV(dy & 7);
			}

			if((dy & 7) == 0)
			{
				glcd_Device::GotoXY(col, dy & ~7); // should be able to remove this.
				glcd_Device::WriteData(dbyte);
				dbyte = 0;
			}
			dy--;
		}

		dy++; /* point dy back to last destination row */

		/*
		 * Flush out the final destination byte
		 */

		if(dy & 7)
		{
			glcd_Device::GotoXY(col, dy & ~7);
			sbyte = glcd_Device::ReadData();
			/*
			 * Preserve bits outside/above region
			 */

			sbyte &= (_BV((dy & 7)) - 1);
			dbyte |= sbyte;
			glcd_Device::WriteData(dbyte);
		}

	}

}
#endif //GLCDCFG_NO_SCROLLDOWN


/*
 * Handle all special processing characters
 * returns non zero if character was a special character that was handled.
 * returns zero if character was not a special character.
 */
uint8_t gText::SpecialChar(uint8_t c)
{


	if(c == '\n')
	{
		uint8_t height = FontRead(this->Font+FONT_HEIGHT);

		if(isNoPadFixedFont(this->Font))
			height--; // reduce by one since code below assumes padding


#ifdef CONSIDER_FOR_LATER
		/*
		 * Special check for 1 line text areas to avoid complex scrolling
		 * On single line text areas, clear the area and home the cursor.
		 */

		if(this->tarea.y2 == this->tarea.y1 + height)
		{
			this->ClearArea();
		}
#endif

		/*
		 * Erase all pixels remaining to edge of text area.on all wraps
		 * It looks better when using inverted (WHITE) text, on proportional fonts, and
		 * doing WHITE scroll fills.
		 *
		 */


		if(this->x < this->tarea.x2)
			glcd_Device::SetPixels(this->x, this->y, this->tarea.x2, this->y+height, this->FontColor == BLACK ? WHITE : BLACK);

		/*
		 * Check for scroll up vs scroll down (scrollup is normal)
		 */
#ifndef GLCDCFG_NO_SCROLLDOWN
		if(!(this->tarea.mode & SCROLL_DOWN))
#endif
		{

			/*
			 * Normal/up scroll
			 */

			/*
			 * Note this comparison and the pixel calcuation below takes into 
			 * consideration that fonts
			 * are atually 1 pixel taller when rendered. 
			 * This extra pixel is along the bottom for a "gap" between the character below.
			 */
			if(this->y + 2*height >= this->tarea.y2)
			{
#ifndef GLCDCFG_NODEFER_SCROLL
					if(!this->need_scroll)
					{
						this->need_scroll = 1;
						return(1);
					}
#endif

				/*
				 * forumula for pixels to scroll is:
				 *	(assumes "height" is one less than rendered height)
				 *
				 *		pixels = height - ((this->tarea.y2 - this->y)  - height) +1;
				 *
				 *		The forumala below is unchanged 
				 *		But has been re-written/simplified in hopes of better code
				 *
				 */

				uint8_t pixels = 2*height + this->y - this->tarea.y2 +1;
		
				/*
				 * Scroll everything to make room
				 * * NOTE: (FIXME, slight "bug")
				 * When less than the full character height of pixels is scrolled,
				 * There can be an issue with the newly created empty line.
				 * This is because only the # of pixels scrolled will be colored.
				 * What it means is that if the area starts off as white and the text
				 * color is also white, the newly created empty text line after a scroll 
				 * operation will not be colored BLACK for the full height of the character.
				 * The only way to fix this would be alter the code use a "move pixels"
				 * rather than a scroll pixels, and then do a clear to end line immediately
				 * after the move and wrap.
				 *
				 * Currently this only shows up when
				 * there are are less than 2xheight pixels below the current Y coordinate to
				 * the bottom of the text area
				 * and the current background of the pixels below the current text line
				 * matches the text color
				 * and  a wrap was just completed.
				 *
				 * After a full row of text is printed, the issue will resolve itself.
				 * 
				 * 
				 */
				this->ScrollUp(this->tarea.x1, this->tarea.y1, 
					this->tarea.x2, this->tarea.y2, pixels, this->FontColor == BLACK ? WHITE : BLACK);

				this->x = this->tarea.x1;
				this->y = this->tarea.y2 - height;
			}
			else
			{
				/*
				 * Room for simple wrap
				 */

				this->x = this->tarea.x1;
				this->y = this->y+height+1;
			}
		}
#ifndef GLCDCFG_NO_SCROLLDOWN
		else
		{
			/*
			 * Reverse/Down scroll
			 */

			/*
			 * Check for Wrap vs scroll.
			 *
			 * Note this comparison and the pixel calcuation below takes into 
			 * consideration that fonts
			 * are atually 1 pixel taller when rendered. 
			 *
			 */
			if(this->y > this->tarea.y1 + height)
			{
				/*
				 * There is room so just do a simple wrap
				 */
				this->x = this->tarea.x1;
				this->y = this->y - (height+1);
			}
			else
			{
#ifndef GLCDCFG_NODEFER_SCROLL
					if(!this->need_scroll)
					{
						this->need_scroll = 1;
						return(1);
					}
#endif

				/*
				 * Scroll down everything to make room for new line
				 *	(assumes "height" is one less than rendered height)
				 */

				uint8_t pixels = height+1 - (this->tarea.y1 - this->y);

				this->ScrollDown(this->tarea.x1, this->tarea.y1, 
					this->tarea.x2, this->tarea.y2, pixels, this->FontColor == BLACK ? WHITE : BLACK);

				this->x = this->tarea.x1;
				this->y = this->tarea.y1;
			}
		}
#endif
	  return(1);
	}
	return(0);

}

/**
 * output a character
 *
 * @param c the character to output
 *
 * If the character will not fit on the current text line
 * inside the text area,
 * the text position is wrapped to the next line. This might be
 * the next lower or the next higher line depending on the
 * scroll direction.
 *
 * If there is not enough room to fit a full line of new text after
 * wrapping, the entire text area will be scrolled to make room for a new
 * line of text. The scroll direction will be up or down
 * depending on the scroll direction for the text area.
 *
 * If there is an attempt to output a character that has no definition
 * i.e. its width is zero in the font data, then no character will be rendered.
 * This occurs if the font is a "sparse" font, that does not define all the characters
 * within the font range. To save room a "sparse" font will set the width of the
 * undefined characters to zero.
 *
 * @returns 1 if a character was rendered, 0 if a character was not rendered
 *
 * @note
 * UTF8 character encoding is supported only if it is enabled in the library config file.
 * When UTF8 encoding is enabled, PutChar() will support receiving the UTF8 encoding
 * as a single wide UTF8 character or as individual bytes in a multi byte data stream. 
 * If the UTF8 encoding is sent to PutChar() as multiple individul bytes,
 * only the last byte of the multi-byte sequence will actually print a character.
 * All other bytes of the multibyte sequence will silently print nothing as
 * they are used to decode the character code.\n\n
 * For more information on UTF8 and UTF8 font see here:\n
 *  http://en.wikipedia.org/wiki/UTF-8 \n
 *  http://www.fileformat.info/info/charset/UTF-8/list.htm
 *
 * @warning
 * When UTF8 encoding support is enabled, the raw 8 bit character codes 0xc2 and 0xc3
 * can no longer be sent as those are used for UTF8 encoding.
 * 
 * @see Puts()
 * @see Puts_P()
 * @see write()
 * @see writeUTF8()
 */

int 
#ifdef GLCDCFG_UTF8
gText::PutChar(wchar_t c)
#else
gText::PutChar(uint8_t c)
#endif
{
    if(this->Font == 0)
	  return 0; // no font selected

	/*
	 * check for special character processing
	 */

	if(c < 0x20)
	{
		if(SpecialChar((uint8_t)c))
			return 1;
	}
#ifdef GLCDCFG_UTF8
	// must use tmp var since wchar is not guaranteed to be signed.
	int tc  = UTF8decode(c);
	if(tc < 0)
		return(1); // processing still on going, so lie and say we processed it
	c = tc;
#endif

	uint8_t width = 0;
	uint8_t height = FontRead(this->Font+FONT_HEIGHT);
	uint8_t bytes = (height+7)/8; /* calculates height in rounded up bytes */
	
	uint8_t firstChar = FontRead(this->Font+FONT_FIRST_CHAR);
	uint8_t charCount = FontRead(this->Font+FONT_CHAR_COUNT);
	
	uint16_t index = 0;
	uint8_t thielefont;

	if(c < firstChar || c >= (firstChar+charCount))
	{
		return 0; // invalid char
	}
	c-= firstChar;

	if( isFixedWidthFont(this->Font))
	{
		thielefont = 0;
		width = FontRead(this->Font+FONT_FIXED_WIDTH); 
		index = c*bytes*width+FONT_WIDTH_TABLE;
	}
	else
	{
	// variable width font, read width data, to get the index
		thielefont = 1;
		/*
		 * Because there is no table for the offset of where the data
		 * for each character glyph starts, run the table and add up all the
		 * widths of all the characters prior to the character we
		 * need to locate.
		 */
		for(uint8_t i=0; i<c; i++)
		 index += FontRead(this->Font+FONT_WIDTH_TABLE+i);

		/*
		 * Calculate the offset of where the font data
		 * for our character starts.
		 * The index value from above has to be adjusted because
		 * there is potentialy more than 1 byte per column in the glyph,
		 * when the characgter is taller than 8 bits.
		 * To account for this, index has to be multiplied
		 * by the height in bytes because there is one byte of font
		 * data for each vertical 8 pixels.
		 * The index is then adjusted to skip over the font width data
		 * and the font header information.
		 */

	   index = index*bytes+charCount+FONT_WIDTH_TABLE;

		/*
		 * Finally, fetch the width of our character
		 */
	   width = FontRead(this->Font+FONT_WIDTH_TABLE+c);

#ifdef notdef
		/*
		 * Check to see if variable fonts are being rendered in fixed width mode
		 */
		if(this->tarea.mode & gTextMode_VARasFIXED)
		{
			/*
			 * Offset the glyph to center within the fixed width size for the font
			 */
			this->x = this->x + ((FontRead(this->Font+FONT_FIXED_WIDTH) - width) / 2);
		}
#endif
    }
	if(!width) // no glyph definition for character?
		return(0);

#ifndef GLCDCFG_NODEFER_SCROLL
	/*
	 * check for a defered scroll
	 * If there is a deferred scroll,
	 * Fake a newline to complete it.
	 */

	if(this->need_scroll)
	{
		this->PutChar('\n'); // fake a newline to cause wrap/scroll
		this->need_scroll = 0;
	}
#endif
		/*
		 * Check to see if variable fonts are being rendered in fixed width mode
		 */
		if(this->tarea.mode & gTextMode_VARasFIXED)
		{
			/*
			 * Offset the glyph to center within the fixed width size for the font
			 *FIXME: can't really do it like this. It generally works but the code
			 * really needs to slam out bytes to actually write to the skiped over locations
			 * Otherwise in inverted mode, you can get gaps. Also need to think about how
			 * it should work with overstrike.
			 * If this isn't done the when rendering smaller characters on top of
			 * of larger characters, pixels remain from the larger pixels.
			 */
			this->x = this->x + ((FontRead(this->Font+FONT_FIXED_WIDTH) - width) / 2);
		}

	/*
	 * If the character won't fit in the text area,
	 * fake a newline to get the text area to wrap and 
	 * scroll if necessary.
	 * NOTE/WARNING: the below calculation takes into consideration the 1 pixel pad.
	 */
	if(this->x + width - (isNoPadFixedFont(this->Font)) > this->tarea.x2)
	{
		this->PutChar('\n'); // fake a newline to cause wrap/scroll
#ifndef GLCDCFG_NODEFER_SCROLL
		/*
		 * We can't defer a scroll at this point since we need to ouput
		 * a character right now.
		 */
		if(this->need_scroll)
		{
			this->PutChar('\n'); // fake another newline to force wrap/scroll
			this->need_scroll = 0;
		}
#endif
		/*
		 * Check to see if variable fonts are being rendered in fixed width mode
		 */
		if(this->tarea.mode & gTextMode_VARasFIXED)
		{
			/*
			 * Offset the glyph to center within the fixed width size for the font
			 */
			this->x = this->x + ((FontRead(this->Font+FONT_FIXED_WIDTH) - width) / 2);
		}
	}

	// last but not least, draw the character

	/*
	 * Paint font data bits and write them to LCD memory 1 LCD page at a time.
	 * This is very different from simply reading 1 byte of font data
	 * and writing all 8 bits to LCD memory and expecting the write data routine
	 * to fragement the 8 bits across LCD 2 memory pages when necessary.
	 * That method (really doesn't work) and reads and writes the same LCD page 
	 * more than once as well as not do sequential writes to memory.
	 *
	 * This method of rendering while much more complicated, somewhat scrambles the font 
	 * data reads to ensure that all writes to LCD pages are always sequential and a given LCD
	 * memory page is never read or written more than once.
	 * And reads of LCD pages are only done at the top or bottom of the font data rendering
	 * when necessary. 
	 * i.e it ensures the absolute minimum number of LCD page accesses
	 * as well as does the sequential writes as much as possible.
	 *
	 */

	uint8_t pixels = height;
	uint8_t p;
	uint8_t dy;
	uint8_t tfp;
	uint8_t dp;
	uint8_t dbyte;
	uint8_t fdata;

	if(!isNoPadFixedFont(this->Font))
		pixels++; // extra pixel on bottom for spacing on all fonts but NoPadFixed fonts

	for(p = 0; p < pixels;)
	{
		dy = this->y + p;

		/*
		 * Align to proper Column and page in LCD memory
		 */

		glcd_Device::GotoXY(this->x, (dy & ~7));

		uint16_t page = p/8 * width; // page must be 16 bit to prevent overflow

		for(uint8_t j=0; j<width; j++) /* each column of font data */
		{
			
			/*
			 * Fetch proper byte of font data.
			 * Note:
			 * This code "cheats" to add the horizontal space/pixel row
			 * below the font.
			 * It essentially creates a font pixel of 0/off when the pixels are
			 * out of the defined pixel map.
			 *
			 * fake a fondata read read when we are on the very last
			 * bottom "pixel". This lets the loop logic continue to run
			 * with the extra fake pixel. If the loop is not the
			 * the last pixel the pixel will come from the actual
			 * font data, but that is ok as it is 0 padded.
			 *
			 */

			if(p >= height)
			{
				/*
				 * fake a font data read for padding below character.
				 */
				fdata = 0;
			}
			else
			{
				fdata = FontRead(this->Font+index+page+j);

				/*
				 * Have to shift font data because Thiele shifted residual
				 * font bits the wrong direction for LCD memory.
				 *
				 * The real solution to this is to fix the variable width font format to
				 * not shift the residual bits the wrong direction!!!!
				 */
				if(thielefont && (height - (p&~7)) < 8)
				{
					fdata >>= 8 - (height & 7);
				}
			}

			if(this->FontColor == WHITE)
				fdata ^= 0xff;	/* inverted data for "white" font color	*/


			/*
			 * Check to see if a quick full byte write of font
			 * data can be done.
			 */

			if(!(dy & 7) && !(p & 7) && ((pixels -p) >= 8))
			{
				/*
				 * destination pixel is on a page boundary
				 * Font data is on byte boundary
				 * And there are 8 or more pixels left
				 * to paint so a full byte write can be done.
				 */
					
					if(this->tarea.mode & gTextMode_OVERSTRIKE)
					{
					
						uint8_t tdata = glcd_Device::ReadData();
						fdata |= tdata;
					}
					glcd_Device::WriteData(fdata);
					continue;
			}
			else
			{
					/*
					 * No, so must fetch byte from LCD memory.
					 */
					dbyte = glcd_Device::ReadData();
			}

			/*
			 * At this point there is either not a full page of data
			 * left to be painted  or the font data spans multiple font
			 * data bytes. (or both) So, the font data bits will be painted
			 * into a byte and then written to the LCD memory.page.
			 */


			tfp = p;		/* font pixel bit position 		*/
			dp = dy & 7;	/* data byte pixel bit position */

			/*
			 * paint bits until we hit bottom of page/byte
			 * or run out of pixels to paint.
			 */
			while((dp <= 7) && (tfp) < pixels)
			{
				if(fdata & _BV(tfp & 7))
				{
					dbyte |= _BV(dp);
				}
				else
				{
					dbyte &= ~_BV(dp);
				}

				/*
				 * Check for crossing font data bytes
				 */
				if((tfp & 7)== 7)
				{
					fdata = FontRead(this->Font+index+page+j+width);

					/*
					 * Have to shift font data because Thiele shifted residual
					 * font bits the wrong direction for LCD memory.
					 *
					 * Note: the 8+1 is there vs 8 because we are fetching the font
					 * data byte for the next pixel, but tfp has not yet incremented yet
					 * so it is one less then it should be. We add 1 to the 8
					 * to account for this.
					 */

					if((thielefont) && ((height - tfp) < (8+1)))
					{
						fdata >>= (8 - (height & 7));
					}

					if(this->FontColor == WHITE)
						fdata ^= 0xff;	/* inverted data for "white" color	*/
				}
				tfp++;
				dp++;
			}

			if(this->tarea.mode & gTextMode_OVERSTRIKE)
			{
				uint8_t tdata = glcd_Device::ReadData();
				dbyte |= tdata;
			}
			/*
			 * Now flush out the painted byte.
			 */
			glcd_Device::WriteData(dbyte);
		}

		/*
		 * now create a horizontal gap (vertical line of pixels) between characters.
		 * Since this gap is "white space", the pixels painted are oposite of the
		 * font color.
		 *
		 * Since full LCD pages are being written, there are 4 combinations of filling
		 * in the this gap page.
		 *	- pixels start at bit 0 and go down less than 8 bits
		 *	- pixels don't start at 0 but go down through bit 7
		 *	- pixels don't start at 0 and don't go down through bit 7 (fonts shorter than 6 hi)
		 *	- pixels start at bit 0 and go down through bit 7 (full byte)
		 *
		 * The code below creates a mask of the bits that should not be painted.
		 *
		 * Then it is easy to paint the desired bits since if the color is WHITE,
		 * the paint bits are set, and if the coloer is not WHITE the paint bits are stripped.
		 * and the paint bits are the inverse of the desired bits mask.
		 */


	  if(!isNoPadFixedFont(this->Font))
	  { // extra pixel on right for spacing on all fonts but NoPadFixed fonts
		
		if((dy & 7) || (pixels - p < 8))
		{
		uint8_t mask = 0;

			dbyte = glcd_Device::ReadData();

			if(dy & 7)
				mask |= _BV(dy & 7) -1;

			if((pixels-p) < 8)
				mask |= ~(_BV(pixels - p) -1);


			if(this->FontColor == WHITE)
				dbyte |= ~mask;	
			else
				dbyte &= mask;

		}
		else
		{
			if(this->FontColor == WHITE)
				dbyte = 0xff;
			else
				dbyte = 0;
		}
		if(this->tarea.mode & gTextMode_OVERSTRIKE)
		{
 			/* not all pixels set, fetch byte from lcd for the OR*/
			if(dbyte != 0xff)
			{
				uint8_t tdata = glcd_Device::ReadData();
				dbyte |= tdata;
			}
		}
		glcd_Device::WriteData(dbyte);

	  }
		/*
		 * advance the font pixel for the pixels
		 * just painted.
		 */

		p += 8 - (dy & 7);
	}


	/*
	 * Since this rendering code always starts off with a GotoXY() it really isn't necessary
	 * to do a real GotoXY() to set the h/w location after rendering a character.
	 * We can get away with only setting the s/w version of X & Y.
	 *
	 * Since y didn't change while rendering, it is still correct.
	 * But update x for the pixels rendered.
	 *
	 */

	this->x += width; // pixels rendered in character glyph

	if(this->tarea.mode & gTextMode_VARasFIXED)
	{
		/*
		 * Correct pixel position to skip over the remaining "gap" pixels
		 * after the variable rendering to reserve the pixels up to the "fixed" width
		 * of the variable font.
		 * FIXME: Actually will have to write pixels out to ensure overstrike works correctly
		 */
		uint8_t fixedwidth = FontRead(this->Font+FONT_FIXED_WIDTH);
		this->x += fixedwidth - width - ((fixedwidth - width) / 2);
	}
	if(!isNoPadFixedFont(this->Font))
		this->x++; // skip over pad pixel we rendered

	return 1; // valid char
}


/**
 * output a character string
 *
 * @param str pointer to a null terminated character string.
 *
 * Outputs all the characters in the string to the text area. 
 *
 * @b Examples:
 * @code
 * GLCD.Puts("Hello World");
 * textarea.Puts("Hello World");
 * @endcode
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts_P()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */


void gText::Puts(const char *str)
{
    while(*str)
	{
        this->PutChar((uint8_t)*str);
        str++;
    }
}


/**
 * output a String class string
 *
 * @param str String class string
 *
 * Outputs all the characters in the string to the text area. 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts_P()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */
void gText::Puts(const String &str)
{
	for (unsigned int i = 0; i < str.length(); i++)
	{
		write(str[i]);
	}
}

/**
 * output an Arduino Flash program memory character string
 *
 * @param str pointer to a null terminated character string stored in program memory declared using F() macro
 *
 * Outputs all the characters in the string to the text area. 
 *
 * @b Examples:
 * @code
 * GLCD.Puts(F("Hello World"));
 * textarea.Puts(F("Hello World"));
 * @endcode
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */
void gText::Puts(const __FlashStringHelper *str)
{
	Puts_P((PGM_P)str);
}

/**
 * output a program memory character string
 *
 * @param str pointer to a null terminated character string stored in program memory
 *
 * Outputs all the characters in the string to the text area. 
 *
 * @b Examples:
 * @code
 * GLCD.Puts_P(PSTR("Hello World"));
 * textarea.Puts_P(PSTR("Hello World"));
 * @endcode
 *
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts()
 * @see DrawString()
 * @see DrawString_P()
 * @see write()
 */

void gText::Puts_P(PGM_P str)
{
uint8_t c;

    while((c = pgm_read_byte(str)) != 0)
	{
        this->PutChar(c);
        str++;
    }
}



/*
 * Internal private function to set XY position for DrawString()
 *
 * slen is rendered width in pixels of string to be positioned
 */
void gText::DrawStringSetXY(int slen, int hpos, int vpos, eraseLine_t erase)
{
uint8_t llen; // line length in pixels
uint8_t fx, fy; // x & y pixel formatting positions

	llen = this->tarea.x2 - this->tarea.x1 + 1;

	/*
	 * There is no real good way to deal with strings that are wider than
	 * the text area.
	 * To handle this we will fudge up the sting length to match the width of the text area.
	 * This will cause a string that is too wide when using center or right alignment
	 * to start in column 0 
	 * and in all cases to run to the right of the line and wrap if line wrapping is enabled
	 */

	if(slen > llen)
		slen = llen;

	switch(hpos)
	{
		case gTextfmt_left:
			fx = 0;
			break;
		case gTextfmt_center:
			fx = llen/2 - slen/2;	// center horizontally
			break;
		case gTextfmt_right:
			fx = llen - slen;		// Right adjust
			break;
		case gTextfmt_current:
			fx = this->x - this->tarea.x1; // need relative X position not absolute
			break;
		default:
			if(hpos < 0) // negative values are text columns
			{
				CursorTo(-hpos, 0); // used to get x postion (proper x/y postion will get set below)
				fx = this->x - this->tarea.x1; // need relative X position not absolute
			}
			else
			{
				fx = (uint8_t) hpos;
			}
	}
	switch(vpos)
	{
		case gTextfmt_top:
			fy = 0;
			break;
		case gTextfmt_center:
			// The +1 is to round up for centering i.e. even heights use lower pixel on display
			// the -1 is to pick a lower pixel to be center for even sized font heights
			fy = (this->tarea.y2 - this->tarea.y1+1) / 2 - (this->CharHeight(0) -1)/2;
			break;
		case gTextfmt_bottom:
			fy = (this->tarea.y2 -this->tarea.y1) - this->CharHeight(0) +1; // use default char height of font
			break;
		case gTextfmt_current:
			fy = this-> y - this->tarea.y1; // need relative Y position not absolute
			break;
		default:
			if(vpos < 0) // negative values are text rows
			{
				CursorTo(0, -vpos); // used to get y position (proper x/y position will get set below)
				fy = this-> y - this->tarea.y1; // need relative Y position not absolute
			}
			else
			{
				fy = (uint8_t) vpos;
			}
	}

	this->CursorToXY(fx,fy);
	this->EraseTextLine(erase);
}

/*
 * NOTE: the description below is used for ALL Drawstring() functions
 */
/**
 * Output a NULL terminated character string with horizontal and vertical positioning
 *
 * @param str character string
 * @param hpos specifies the horizontal positioning.
 * @param vpos specifies the vertical positioning.
 * @param erase optional parameter that specifies which of the remaing pixels on the text line will be erased
 * before the characters are rendered. It is one of the values from @ref eraseLine_t and defaults to @ref eraseNONE
 * 
 * Outputs the characters in the string to the text area. 
 *
 * hpos/vpos can specify:
 * - an x/y pixel position relative to the upper left corner of the text area
 * - a formatting type from @ref gTextfmt_t
 * - the type gTextfmt_col(col) to specify a text column relative to the left of the text area
 * - the type gTextfmt_row(row) to specify a text row/line relative the top of the text area
 *
 * When hpos & vpos are not a formatting type, 
 * the value is a zero based pixel coordinate relative to the upper left corner of the text area.
 * 
 * @b Examples:
 * @code
 *	GLCD.DrawString("Hello", 20, 10, eraseFULL_LINE);		 	// print at pixel coordinate x=20, y=10 relative to top left corner of text area
 *	GLCD.DrawString("Hello", gTextfmt_left, gTextfmt_row(2), eraseFULL_LINE);		// left adjust on row=2 (3rd line from top) 
 *	GLCD.DrawString("Hello", gTextfmt_col(20), gTextfmt_row(2), eraseFULL_LINE); 	// text column=20, row=2
 *	GLCD.DrawString("Hello", gTextfmt_col(col), gTextfmt_row(row), eraseFULL_LINE); 	// text column=col, row=row
 *	GLCD.DrawString("Hello", gTextfmt_center, gTextfmt_center);					// center on display
 *	GLCD.DrawString("Hello", gTextfmt_center, gTextfmt_current);				// center on the current line of display
 *	GLCD.DrawString("Hello", gTextfmt_center, gTextfmt_bottom);					// center on bottom line of display
 *	GLCD.DrawString("Hello", gTextfmt_right, gTextfmt_top);						// right adjust on top line of display
 *	GLCD.DrawString(F("Hello"), gTextfmt_right, gTextfmt_top);					// right adjust on top line of display
 *	GLCD.DrawString_P(PSTR("Hello"), gTextfmt_right, gTextfmt_top);				// right adjust on top line of display
 *	textarea.DrawString(F("Hello"), gTextfmt_right, gTextfmt_top);				// right adjust on top line of display
 * @endcode
 * 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 *
 * @note
 * Because DrawString() allows alignment at the pixel level in addition to the row and column, it
 * is possible that the renderered text does not line up with 
 * a normal text area row and column. This is likely to occur when using formatting type @ref gTextfmt_center
 *
 * @see PutChar()
 * @see Puts()
 * @see Puts_P()
 * @see DrawString_P()
 * @see write()
 */

void gText::DrawString(const char *str, int hpos, int vpos, eraseLine_t erase)
{
uint8_t slen;

	slen = StringWidth(str);
	DrawStringSetXY(slen, hpos, vpos, erase);
	this->Puts(str);
}

/**
 * Output a String class string with horizontal and vertical positioning
 *
 * @copydetails gText::DrawString(const char *str, int hpos, int vpos, eraseLine_t erase)
 *
 */
void gText::DrawString(String &str, int hpos, int vpos, eraseLine_t erase)
{
	DrawString(str, hpos, vpos, erase);
}

/**
 * Output a program memory character string with horizontal and vertical positioning
 *
 * @copydetails gText::DrawString(const char *str, int hpos, int vpos, eraseLine_t erase)
 */

void gText::DrawString_P(PGM_P str, int hpos, int vpos, eraseLine_t erase)
{
uint8_t slen;

	slen = StringWidth_P(str);
	DrawStringSetXY(slen, hpos, vpos, erase);
	this->Puts_P(str);
}

#if ARDUINO > 100 || defined(F)
/**
 * Output an Arduino FLASH string with horizontal and vertical positioning
 *
 * @copydetails gText::DrawString(const char *str, int hpos, int vpos, eraseLine_t erase)
 */

void gText::DrawString(const __FlashStringHelper *str, int hpos, int vpos, eraseLine_t erase)

{
	DrawString_P((PGM_P)str, hpos, vpos, erase);
}
#endif

#if defined(flashStr)
/**
 * Output an GLCD FLASH string with horizontal and vertical positioning
 *
 * @copydetails gText::DrawString(const char *str, int hpos, int vpos, eraseLine_t erase)
 */

void gText::DrawString(FLASHSTRING str, int hpos, int vpos, eraseLine_t erase)

{
	DrawString_P((PGM_P)str, hpos, vpos, erase);
}
#endif


/**
 * Positions cursor to a character based column and row.
 *
 * @param column specifies the horizontal position 
 * @param row  specifies the vertical position
 *
 *	Column and Row are zero based character positions
 *	and are relative the the upper left corner of the
 *	text area base on the size of the currently selected font.
 *
 * This is intended for fixed width fonts
 *
 * @b Examples:
 * @code
 * GLCD.CursorTo(0,0);     // position cursor to leftmost column, on top line of default text area
 * GLCD.CursorTo(5,0);     // position cursor to column 5 , on top line of default text area
 * textarea.CursorTo(10,2);// position cursor to column 10, line 2 in text area
 * textarea.CursorTo(0,2); // position cursor to leftmost column, line 2 in text area
 * textarea.CursorTo(0,0); // position cursor to leftmost column, on top line of text area
 * @endcode
 *
 * @note
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 *
 * @warning
 * If the column and row parameters create a position that is outside the bounds
 * of the text area, then the position will be set to the home position of the
 * text area.
 *
 * @see CursorToXY()
 */

void gText::CursorTo( uint8_t column, uint8_t row)
{
uint8_t width, height;

	if(this->Font == 0)
		return; // no font selected

	width = GetAreaProp(gTextProp_FontWidth);
	height = GetAreaProp(gTextProp_FontHeight);

	/*
	 * Text position is relative to current text area
	 */

	this->x = column * width  + this->tarea.x1;
	this->y = row * height + this->tarea.y1;

	/*
	 * Sanity check final positions
	 * If either is outside the bounds of the text area
	 * then revert the postion back to the "home" position.
	 */
	if((this->x >= this->tarea.x2) || (this->y >= this->tarea.y2))
	{
		this->x = this->tarea.x1;
		this->y = this->tarea.y1;
	}

#ifndef GLCDCFG_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when repositioning
	 */
	this->need_scroll = 0;
#endif
}

/**
 * Positions cursor to a character based column on the current row.
 *
 * @param column specifies the horizontal position 
 *
 *	Column is a 0 based character position
 *	based on the size of the currently selected font.
 *
 * If column is negative then the column position is relative to the current cursor
 * position.
 *
 * @warning
 * While intended only for fixed width fonts, cursor repositioning will be done for variable
 * width fonts.
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character in the font.
 * Because the widest character is used for the amount of cursor movement, the amount
 * of cursor movement when using relative positioning will often not be consistent with
 * the number characters previously rendered. For example, if a letter "l" was written
 * and the cursor was reposisitioned with a -1, the amount backed up will be much larger
 * than the width of the "l".
 *
 *
 * @see CursorToXY()
 */
void gText::CursorTo( int8_t column)
{
uint8_t width;

	if(this->Font == 0)
		return; // no font selected

	width = GetAreaProp(gTextProp_FontWidth);

	/*
	 * Text position is relative to current text area
	 * negative value moves the cursor backwards
	 */
    if(column >= 0) 
	  this->x = column * width + this->tarea.x1;
	else
   	  this->x -= column * width;   	

#ifndef GLCDCFG_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when repositioning
	 */
	this->need_scroll = 0;
#endif
}


/**
 * Positions cursor to a X,Y position
 *
 * @param x specifies the horizontal location
 * @param y specifies the vertical location
 *
 *	X & Y are zero based pixel coordinates and are relative to 
 *	the upper left corner of the text area.
 *
 * @b Examples:
 * @code
 * GLCD.CursorToXY(GLCD.CenterX, GLCD.CenterY); // position cursor to center of display
 * GLCD.CursorToXY(5,8);     // position cursor 5 pixels from left, 8 pixels from top in default text area
 * textarea.CursorToXY(10,2);// position cursor 10 pixels from left,2 pixels from top in text area
 * textarea.CursorToXY(0,0); // position cursor upper left corner in text area
 *
 * @endcode
 *
 * @note:
 * For most uses, DrawString() is a better alternative to position text.
 *
 * @see CursorTo()
 * @see DrawString()
 */

void gText::CursorToXY( uint8_t x, uint8_t y)
{

	/*
	 * Text position is relative to current text area
	 */
	this->x = this->tarea.x1 + x;
	this->y = this->tarea.y1 + y;

#ifndef GLCDCFG_NODEFER_SCROLL
	/*
	 * Make sure to clear a deferred scroll operation when repositioning
	 */
	this->need_scroll = 0;
#endif
}

/**
 * Erase in Line
 *
 * @param type type of line erase
 *
 * @arg \ref eraseTO_EOL Erase from cursor to end of line
 * @arg \ref eraseFROM_BOL Erase from beginning of line to cursor
 * @arg \ref eraseFULL_LINE Erase entire line
 *
 * Erases all or part of a line of text depending on the type
 * of erase specified.
 *
 * If type is not specified it is assumed to be \ref eraseTO_EOL
 *
 * The cursor position does not change.
 *
 * @see ClearArea()
 * @see eraseLine_t
 */

void gText::EraseTextLine( eraseLine_t type) 
{

	uint8_t x = this->x;
	uint8_t y = this->y;
	uint8_t height = FontRead(this->Font+FONT_HEIGHT);
	uint8_t color = (this->FontColor == BLACK) ? WHITE : BLACK;

	switch(type)
	{
		case eraseTO_EOL:
				glcd_Device::SetPixels(x, y, this->tarea.x2, y+height, color);
				break;
		case eraseFROM_BOL:
				glcd_Device::SetPixels(this->tarea.x1, y, x, y+height, color);
				break;
		case eraseFULL_LINE:
				glcd_Device::SetPixels(this->tarea.x1, y, this->tarea.x2, y+height, color);
				break;
		default:	// handles eraseNONE case
				break;
	}

	/*
	 * restore cursor position
	 */
	this->x = x;
	this->y = y;
}

/**
 * Erase Text Line
 *
 * @param row row # of text to earase
 *
 * Erases a line of text and moves the cursor
 * to the begining of the line. Rows are zero based so
 * the top line/row of a text area is 0.
 *
 * @see ClearArea()
 */

void gText::EraseTextLine( uint8_t row)
{
   this->CursorTo(0, row);
   EraseTextLine(eraseTO_EOL);	
}


/**
 * Select a Font and font color
 *
 * @param font a font definition
 * @param fgcolor foreground font color 
 * @param callback optional font read routine
 *
 *
 * Selects the font definition as the current font for the text area.
 *
 * All subsequent printing functions will use this font. 
 *
 * Font definitions stored in program memory.
 * You can have as many fonts defines as will fit in program memory
 * can switch between them with this function.
 *
 * fgcolor PIXEL_ON or BLACK renders "on" pixels on a "off" background,
 * i.e. it turns on the pixels on the LCD for the pixels in the character glpyh
 * and turns off all the background pixels.
 *
 * fgcolor PIXEL_OFF or WHITE renders "off" pixels on a "on" background;
 * i.e. it turns off the pixels on the LCD for the pixels in the character glpyh
 * and turns on all the background pixels.
 *
 * If the optional callback argument is ommitted, a default routine
 * is selected that assumes that the font is in program memory (flash).
 *
 * @b Examples:
 * @code
 *    GLCD.SelectFont(System5x7);            // fg pixels on, bg pixels off
 *    GLCD.SelectFont(System5x7, PIXEL_ON);  // fg pixels on, bg pixels off
 *    GLCD.SelectFont(System5x7, BLACK);     // fg pixels on, bg pixels off
 *    GLCD.SelectFont(System5x7, PIXEL_OFF); // fg pixels off, bg pixels on
 *    GLCD.SelectFont(System5x7, WHITE);     // fg pixels off, bg pixels on
 *    GLCD.SelectFont(Arial14);
 *    textarea.SelectFont(fixednums7x15, PIXEL_OFF);
 * @endcode
 *
 * @note
 * When the display is initilized in INVERTED mode,
 * the colors are reversed.
 * @note
 * AVR systems require fonts to fit in lower 64k of program memory
 *
 * @see SetFontColor()
 * @see SetAreaMode()
 */

void gText::SelectFont(Font_t font,uint8_t fgcolor, FontCallback callback)
{
	this->Font = font;
	FontRead = callback;  // WARNING: this sets the read function that will be used by all instances of gText
	this->FontColor = fgcolor;
}

/**
 * Set a font foreground color
 *
 * @param fgcolor  foreground font color
 *
 * Set the font foreground color for the text area.
 *
 * fgcolor PIXEL_ON or BLACK renders "on" pixels on a "off" background,
 * i.e. it turns on the pixels on the LCD for the pixels in the character glpyh
 * and turns off all the background pixels.
 *
 * fgcolor PIXEL_OFF or WHITE renders "off" pixels on a "on" background;
 * i.e. it turns off the pixels on the LCD for the pixels in the character glpyh
 * and turns on all the background pixels.
 *
 * @b Examples:
 * @code
 *    GLCD.SetFontColor(PIXEL_ON);  // fg pixels on, bg pixels off
 *    GLCD.SetFontColor(BLACK);     // fg pixels on, bg pixels off
 *    GLCD.SetFontColor(PIXEL_OFF); // fg pixels off, bg pixels on
 *    GLCD.SetFontColor(WHITE);     // fg pixels off, bg pixels on
 *    textarea.SetFontColor(PIXEL_OFF);
 * @endcode
 *
 * @note
 *	The color becomes effective for all future text operations but
 *	does not alter the area, including any existing text in the area.
 *  This allows switching colors as printing is done within the area.
 *	If a ClearScreen() is done, then the area is cleared according
 *	to the color.
 *   
 *
 * @see SelectFont()
 * @see SetAreaMode()
 */

void gText::SetFontColor(uint8_t fgcolor)
{
   	this->FontColor = fgcolor;
}

/**
 * Set TextArea mode options
 *
 * @param mode a value from @ref gTextMode
 *
 * @b Examples:
 * @code
 * textarea.SetAreaMode(gTextMode_SCROLLDOWN); // enable reverse vertical scrolling
 * textarea.SetAreaMode(gTextMode_VARasFIXED); // render variable font as fixedwidth
 * textarea.SetAreaMode(gTextMode_OVERSTRIKE); // enable text overstrike
 * @endcode
 *
 * @see ClearAreaMode()
 * @see SelectFont()
 * @see SetFontColor()
 * @see DefineArea()
 * @see gTextMode
 */
/*
 */
void gText::SetAreaMode(gTextMode mode)
{
	if(mode == SCROLL_UP) // special check for backward compability for SCROLL_UP
	{
		ClearAreaMode(SCROLL_DOWN);
	}
	else
	{
		this->tarea.mode |= mode; 
	}

} 

/**
 * Clear TextArea mode options
 *
 * @param mode a value from @ref gTextMode
 *
 * @b Examples:
 * @code
 * textarea.ClearAreaMode(gTextMode_SCROLLDOWN); // Disable reverse vertical scrolling
 * textarea.ClearAreaMode(gTextMode_OVERSTRIKE); // Disable text overstrike mode
 * @endcode
 *
 * @see SetAreaMode()
 * @see SelectFont()
 * @see SetFontColor()
 * @see DefineArea()
 * @see gTextMode
 */
/*
 */
void gText::ClearAreaMode(gTextMode mode)
{
   this->tarea.mode &= ~mode; 
} 


	
/**
 * Returns the pixel width of a character
 *
 * @param c character to be sized
 *
 * @return The width in pixels of the given character
 * including any inter-character gap pixels following the character when the character is
 * rendered on the display.
 *
 * @note The font for the character is the most recently selected font.
 * If there is no font selected or if the character code does not have a valid definition in the selected font,
 * 0 will be returned.\n\n
 * UTF8 character encoding is supported only if it is enabled in the library config file.
 * When UTF8 encoding is enabled, CharWidth() will support receiving the UTF8 encoding
 * as a single character or as multi character byte stream. 
 * If the UTF8 encoding is sent as multiple bytes, only the last byte of the multi-byte
 * sequence will return a a non zero width for the decoded character.
 * All other bytes within the UTF8 multi-byte sequence will return a width of zero.

 *
 * @see CharHeight()
 * @see StringWidth()
 * @see StringWidth_P()
 * @see GetAreaProp()
 */

uint8_t
#ifdef GLCDCFG_UTF8
gText::CharWidth(wchar_t c)
#else
gText::CharWidth(uint8_t c)
#endif
{
	uint8_t width = 0;

#ifdef GLCDCFG_UTF8
	// must use tmp var since wchar is not guaranteed to be signed.
	int tc  = UTF8decode(c);
	if(tc < 0)
	{
		return(0); // processing still on going, so lie and say zero width
	}
	c = tc;
#endif
	
    if(isFixedWidthFont(this->Font))
	{
		width = FontRead(this->Font+FONT_FIXED_WIDTH);
		if(isFixedWidthPaddedFont(this->Font))  // there is 1 pixel pad on right
			width++;
	} 
    else
	{ 
	    // variable width font 

		uint8_t firstChar = FontRead(this->Font+FONT_FIRST_CHAR);
		uint8_t charCount = FontRead(this->Font+FONT_CHAR_COUNT);
	
		if(c >= firstChar && c < (firstChar+charCount))
		{
			if(this->tarea.mode & gTextMode_VARasFIXED)
			{
				// use fixed width
				width = FontRead(this->Font+FONT_FIXED_WIDTH) +1;
			}
			else
			{
				// read width data
				c -= firstChar;
				width = FontRead(this->Font+FONT_WIDTH_TABLE+c)+1; // always has space pixel on right
			}
		}
	}	
	return width;
}

/**
 * Returns the pixel height of a character
 *
 * @param c character to be sized. NULL can be used to size the first char in the font.
 *
 * @return The height in pixels of the given character font
 * including any inter-character gap pixels below the character when the character is
 * rendered on the display. 
 *
 * @note The font for the character is the most recently selected font
 * and while individual character glyphs will actually render different pixel heights,
 * the returned value is actually the height of the text font row. i.e a '.' (period)
 * will return the same height as a '@' (at sign).
 *
 * If there is no font selected, 0 will be returned.
 *
 * @see CharWidth()
 * @see StringWidth()
 * @see StringWidth_P()
 * @see GetAreaProp()
 */

uint8_t
#ifdef GLCDCFG_UTF8
gText::CharHeight(wchar_t c)
#else
gText::CharHeight(uint8_t c)
#endif
{
	uint8_t height = 0;
	
	if(this->Font)
	{
	/*
 	 * For now, ignore the character passed and use the overall height
	 * of the font.
	 * Later we may actually use the character but 0/NULL must be supported.
	 * When this is added, UTF8 processing will also have to be done.
	 */
		height = FontRead(this->Font+FONT_HEIGHT);

		if(!isNoPadFixedFont(this->Font))  // pixel padding?
				height++;
	}
	/*
	 * if(c) is to get around warning of not using 'c' parameter
	 * optimizer should eliminate the code
	 */
	if(c) return height;

	return height;
}

/**
 * Returns the pixel width of a string
 *
 * @param str pointer to string stored in RAM
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see CharHeight()
 * @see StringWidth_P()
 */

uint16_t gText::StringWidth(const char* str)
{
	uint16_t width = 0;
	
	while(*str != 0)
	{
		width += this->CharWidth(*str++);
	}
	
	return width;
}

/**
 * Returns the pixel width of a string
 *
 * @param str pointer to string stored in program memory
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see CharHeight()
 * @see StringWidth()
 */

uint16_t gText::StringWidth_P(PGM_P str)
{
	uint16_t width = 0;
	
	while(pgm_read_byte(str) != 0)
	{
		width += this->CharWidth(pgm_read_byte(str++));
	}
	
	return width;
}

/**
 * Returns the pixel width of a String class string
 *
 * @param str String class string
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see CharHeight()
 * @see StringWidth()
 */

uint16_t gText::StringWidth(String &str)
{
	uint16_t width = 0;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		width += this->CharWidth(str[i]);
	}
	
	return width;
}

/**
 * Returns the pixel width of a FlashString
 *
 * @param str string declared with F() macro
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see CharHeight()
 * @see StringWidth()
 */

uint16_t gText::StringWidth(const __FlashStringHelper *str)
{
	return(StringWidth_P((PGM_P)str));
}


/**
 * Legacy function to print a number
 *
 * @param n is the number to print
 *
 *
 * @see print(n)
 */
void gText::PrintNumber(long n)
{
uint8_t buf[10];  // prints up to 10 digits  
uint8_t i=0;
	if(n==0)
	{
	   PutChar('0');
	}
	else
	{
		if(n < 0)
		{
        	PutChar('-');
			n = -n;
	 	}
		while(n>0 && i <= 10)
		{
			buf[i++] = n % 10;  // n % base
			n /= 10;   // n/= base
		}
		for(; i >0; i--)
			PutChar((char) (buf[i-1] < 10 ? '0' + buf[i-1] : 'A' + buf[i-1] - 10));	  
   }
}

/**
 * output a character to the text area
 * @param c the character to output
 *
 * This method is needed for the Print base class
 *
 * @returns
 * On pre 1.0 it is a void function\n
 * On 1.0 and beyond it returns 1 if a character is rendered, 0 if a character was not rendered
 *
 * @warning
 * Carriage returns are swallowed to prevent println() from
 * printing the 0xd character if the font contains this character.
 * To actually print the 0xd character use PutChar().
 *
 * @note
 * multibyte wide UTF8 characters are not supported as the interface
 * to write() is limited to 8 bit data.\n\n
 * UTF8 characters can be used when the optional UTF8 library support is enabled.
 * However, since the write() interface is 8 bit data, each byte of the multibyte character
 * must be sent seperately.\n
 * When UTF8 support enabled, writeUTF8() and PutChar() can be used to output UTF8 encoded characters.
 * including multibyte wide UTF8 character codes 
 *
 * @see PutChar()
 */

#if ARDUINO < 100
void gText::write(uint8_t c) 
{
	if(c != '\r') // toss carriage returns cause println() sends them
		this->PutChar(c);
} 
#else
size_t gText::write(uint8_t c) 
{
	if(c == '\r') // toss carriage returns
		return(1);
	return(this->PutChar(c));
} 
#endif

/**
 * output a UTF8 encoded character to the text area
 * @param utfc the UTF8 character to output
 *
 * used to output UTF8 encoded characters including multibyte wide UTF8 character codes.\n
 * The UTF character codes are limited to 0-255.
 * To go beyond 255 is a BIG deal since the font header currently only supports up to
 * 256 characters. (it is only an 8 bit field)\n
 * Also by only supporting 0-255 codes, when UTF8 processing is enabled, raw code processing
 * can also be supported for characters above 0x7f with the exception that codes
 * 0xc2 and 0xc3 will be lost.
 * @note
 * This function exists only when UTF8 support is enabled.
 *
 * @returns
 * On pre 1.0 it is a void function\n
 * On 1.0 and beyond it returns 1 if a character is rendered, 0 if a character was not rendered
 *
 * @see write()
 * @see PutChar()
 */

#if defined(GLCDCFG_UTF8) || defined(DOXYGEN)
#if ARDUINO < 100
void gText::writeUTF8(wchar_t utfc) 
{
		this->PutChar(c);
} 
#else
size_t gText::writeUTF8(wchar_t utfc) 
{
	return(this->PutChar(c));
} 
#endif
#endif


#ifndef USE_ARDUINO_FLASHSTR
// functions to print strings in Progmem
// these should be removed when Arduino supports FLASH strings in the base print class
/**
 * print a flash based string
 * @param str pointer to a null terminated character string stored in program memory
 *
 * @see printFlashln()
 * @see Puts_P()
 */
void gText::printFlash(FLASHSTRING str)
{
  Puts_P((PGM_P) str);
}

/**
 * print a flash based string
 * @param str pointer to a null terminated character string stored in program memory
 *
 * The string is output followed by a newline.
 *
 * @see printFlash()
 * @see Puts_P()
 */
void gText::printFlashln(FLASHSTRING str)
{
  printFlash(str);
  write('\n');
}
#endif


#ifndef GLCDCFG_NO_PRINTF
#ifdef __AVR__
/*
 * Support for printf() when using AVRs
 * This code plays a few games with the AVR stdio routines.
 *
 * The Printf() functions fudge up a STDIO stream to point back to a C callable function
 * which recovers the C++ text area object (this) and then prints the character
 * using the C++ text area object.
 */

extern "C"
{
  int glcdputc(char c, FILE *fp)
  {
  gText *gtp;

	gtp = (gText *) fdev_get_udata(fp);
	gtp->write((uint8_t) c);
	return(0);
  }
}
#endif

/**
 * print formatted data
 *
 * @param format string that contains text or optional embedded format tags
 * @param ... Depending on the format string, the function may expect a sequence of additional arguments.
 *
 * Writes a sequence of data formatted as the @em format argument specifies.
 * After the @em format parameter, the function expects at least as many additional
 * arguments as specified in @em format.
 * The format string supports all standard @em printf() formating % tags.
 *
 * @b Examples:
 * @code
 * GLCD.Printf("Hello World");
 * GLCD.Printf(F("Hello World")); // put string in flash memory
 * textarea.Printf("Hello World");
 * GLCD.Printf("x=%02d", x);      // 2 digits decimal with zero fill
 * GLCD.Printf("x=%2d", x);       // 2 digits decimal with <space> fill
 * GLCD.Printf("hexval=%02x", x); // 2 hex digits with zero fill
 * GLCD.Printf("val=%d", val);    // decimal number
 * GLCD.Printf(F("val=%d"), val); // decimal number with format string in flash
 * @endcode
 *
 * @note
 *  By default the xxprintf() routines, which Printf() uses, have no floating
 *  point support in AVR enviornments.  (Works for ARM and PIC32 by default)
 *  In order to enable floating point support for AVR,
 *  a linker option must be changed. Currently,
 *  the Arduino IDE does not support modifying the linker options.
 *
 * @see Printf_P()
 */ 


void gText::Printf(const char *format, ...)
{
#ifdef __AVR__
static FILE stdiostr;

	va_list ap;

	fdev_setup_stream(&stdiostr, glcdputc, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&stdiostr, this);

	va_start(ap, format);
	vfprintf(&stdiostr, format, ap);
	va_end(ap);
#else
char buf[80];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	this->Puts(buf);
	va_end(ap);
#endif
}

/**
 * print formatted data
 *
 * @copydetails gText::Printf(const char *format, ...)
 *
 */
void gText::Printf(const __FlashStringHelper *format, ...)
{
#ifdef __AVR__
static FILE stdiostr;

	va_list ap;

	fdev_setup_stream(&stdiostr, glcdputc, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&stdiostr, this);

	va_start(ap, format);
	vfprintf_P(&stdiostr, (const char *) format, ap);
	va_end(ap);
#else
char buf[80];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), (const char *)format, ap); // non AVRS don't have progmem crap
	this->Puts(buf);
	va_end(ap);
#endif
}




/**
 * print formatted data
 *
 * @param format string in AVR progmem that contains text or optional embedded format tags
 * @param ... Depending on the format string, the function may expect a sequence of additional arguments.
 *
 * See gText::Printf() for full details.
 * @see Printf()
 */ 


void gText::Printf_P(PGM_P format, ...)
{
#ifdef __AVR__
static FILE stdiostr;

	va_list ap;

	fdev_setup_stream(&stdiostr, glcdputc, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&stdiostr, this);

	va_start(ap, format);
	vfprintf_P(&stdiostr, format, ap);
	va_end(ap);
#else
char buf[80];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap); // non AVRS don't have progmem crap
	this->Puts(buf);
	va_end(ap);
#endif
}

#endif
