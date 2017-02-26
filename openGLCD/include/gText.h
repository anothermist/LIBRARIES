/*
  gText.h - openGLCD library support - Text output routines
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

#ifndef	GTEXT_H
#define GTEXT_H

#include <inttypes.h>

#include "WString.h"
#include "openGLCD_Config.h"
#include "include/glcd_types.h"
#include "include/glcd_Device.h"

#ifndef USE_ARDUINO_FLASHSTR
// these should be replaced when Arduino supports FLASH strings in the base print class
typedef class _FlashString {} *FLASHSTRING;
#define flashStr(x) ((FLASHSTRING)(PSTR((x))))
#endif

/*
 * On pre Arduino 1.0, define the F() flash string stuff if not already defined
 * (Teensy defines this in pre 1.0)
 */
#if ARDUINO < 100
#ifndef F
class __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<__FlashStringHelper *>(PSTR(string_literal)))
#endif
#endif

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

//
// FONT_LENGTH is a 16 bit Big Endian length field.
// Unfortunately, FontCreator2 screwed up the value it put in the field
// so it is pretty much meaningless. However it still is used to indicate
// some special things.
// 00 00 (fixed width font with 1 padding pixel on right and below)
// 00 01 (fixed width font with no padding pixels)

// any other value means variable width font in FontCreator2 (thiele) format with pixel padding


// the following returns true if the given font is fixed width
// zero length is flag indicating fixed width font (array does not contain width data entries)
#define isFixedWidthFont(font)  ((FontRead(font+FONT_LENGTH) == 0) && (FontRead(font+FONT_LENGTH+1) < 2))
#define isFixedWidthPaddedFont(font)  ((FontRead(font+FONT_LENGTH) == 0) && (FontRead(font+FONT_LENGTH+1) == 0))

#define isNoPadFixedFont(font)  ((FontRead(font+FONT_LENGTH) == 0) && (FontRead(font+FONT_LENGTH+1) == 1))

/*
 * Coodinates for predefined areas are compressed into a single 32 bit token.
 *
 * This works as the coordinates are cuurenly only 8 bit values.
 *
 * This allows the creatation of an unlmited number of predefined areas with zero code or
 * data space overhead.
 * 
 * A macro is used to create the tokens from a set of x1,y1 x2,y2 coordinates.
 *
 * A union is used to extract the coordinates from the 32 bit token.
 *
 * WARNING:
 *	This is non portable code in that it will only work on little endian processors.
 *	If you use big endian you have to switch the byte ordering in the union.
 */

#define MK_TareaToken(x1, y1, x2, y2) \
	(uint32_t) (((uint32_t) (x1) << 24) | ((uint32_t)(y1) << 16) | ((uint32_t)(x2) << 8) | (uint32_t)(y2))
        
/// @cond hide_from_doxygen
typedef union
{       
 struct
 {      
        uint8_t y2;
        uint8_t x2;
        uint8_t y1;
        uint8_t x1;
 }coord;
        
 uint32_t token; // swap byte order above for big endian
        
}TareaToken;
/// @endcond

/**
 * @defgroup glcd_enum openGLCD enumerations
 */


/*
 * When defining text mode enums, keep in mind that they are bits not values.
 * Each mode bit enables/disables a feature. So if a bit is set the feature is enable if not it is disabled.
 * With scrolling, if DOWN is set, scrolling is down, if it is not set scrolling is up.
 */
						
/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief Text area modes
 *
 * These constants are used to define operation modes of text areas 
 * They are used with:
 *	- \ref gText::DefineArea(predefinedArea selection, gTextMode mode)
 *	- \ref gText::DefineArea(predefinedArea selection, Font_t font, gTextMode mode)
 *	- \ref gText::SetAreaMode(gTextMode mode)
 *	- \ref gText::ClearAreaMode(gTextMode mode)
 */

/*
 * Note all enumeration values must be powers of 2
 * enumerations are "enables". 
 * if there are mutually excluse features, there should only be 1 "enable"
 * i.e. if the enumeration bit not set, the reverse mode is enabled.
 * so for example when SCROLLDOWN is not set, SCROLLUP is enabled.
 */

typedef enum {
/// @cond hide_from_doxygen
	SCROLL_UP = 0, // MUST be zero - legacy/deprecated value
	SCROLL_DOWN = 1, // Must match gTextMode_SCROLLDOWN - lagacy/deprecated value
/// @endcond
	gTextMode_SCROLLDOWN = 1, /**<Reverse/Downward scrolling */
	gTextMode_VARasFIXED = 2, /**<Render Variable font as fixed width */
	gTextMode_OVERSTRIKE = 4, /**<Character Overstrike mode */
} gTextMode;

const gTextMode DEFAULT_gTEXTMODE = SCROLL_UP; // default is scroll up

/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief Pre Defined Text areas
 *
 * These enumerations are used to easily define text areas 
 * using predefined display areas.\n
 * They are used with the
 * \ref gText::DefineArea(predefinedArea selection, gTextMode mode) "DefineArea()" function call.
 */
typedef enum  {

	textAreaFULL         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	/**<Entire GLCD display */

	textAreaTOP          = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	/**<Top half of GLCD display */

	textAreaBOTTOM       = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	/**<Bottom half of GLCD display */

	textAreaLEFT         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	/**<Left side of GLCD display */

	textAreaRIGHT        = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	/**<Right side of GLCD display */

	textAreaTOPLEFT      = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT/2 -1 ),
	/**<Upper left quarter of GLCD display */

	textAreaTOPRIGHT     = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	/**<Upper right quarter of GLCD display */

	textAreaBOTTOMLEFT   = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	/**<Buttom left quarter of GLCD display */

	textAreaBOTTOMRIGHT  = MK_TareaToken( DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   )
	/**<Bottom right quarter of GLCD display */

} predefinedArea;



/*
 * enums for ansi style erase function
 * These values match the ANSI EraseInLine terminal primitive: CSI n K
 */  

/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief Erase line options
 * @details
 * These enumerations are used with the function calls:\n
 * \ref gText::EraseTextLine( eraseLine_t type) "EraseTextLine()" 
 * \ref gText::DrawString() "DrawString()"
 * They provide similar functionality to the
 * ANSI EraseInLine terminal primitive: CSI n K
 *
 */
enum eraseLine_t
{
	eraseTO_EOL, 	/**< Erase From cursor to end of Line */
	eraseFROM_BOL,	/**< Erase From Begining of Line to Cursor*/
	eraseFULL_LINE,	/**< Erase Entire line */
	eraseNONE		/**< Erase Nothing - not part of ANSI*/
};

/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief DrawString Format Options
 * @details
 * These enumerations are used with the function calls:\n
 * \ref gText::DrawString() "DrawString()" to align the output string.
 *
 * @note
 * In addtion to these enumeration types,
 * @em gTextfmt_col(col) and @em gTextfmt_row(row) can be use to specify a text column or text row/line.
 *
 */
enum gTextfmt_t
{
/// @cond hide_from_doxygen
	gTextfmt_base = 3000,		// must be larger than largest x/y pixel value but must be positive int
/// @endcond
	gTextfmt_left,		 		/**< horizontal left adjust */
	gTextfmt_right,				/**< horizontal right adjust */
	gTextfmt_top,				/**< vertical top */
	gTextfmt_bottom,			/**< vertical bottom */
	gTextfmt_center,			/**< horizonal/vertical center */
	gTextfmt_current,			/**< current horizontal/vertical location */
};

/*
 * For now use simple macros to change the value to a negative value
 * which means the value is a row/column.
 * The two different versions for row or column are identical and are just for readibility.
 */
#define gTextfmt_colrow(colrow) (-(colrow))
/**
 * @hideinitializer
 *
 */
/*@{*/
#define gTextfmt_col(col) gTextfmt_colrow(col)		/**< \ref gText::DrawString() "DrawString()" format option to align text to a text column */
#define gTextfmt_row(row) gTextfmt_colrow(row)		/**< \ref gText::DrawString() "DrawString()" format option to align text to a text row/line */

/*@}*/

/**
 * @ingroup glcd_enum
 * @hideinitializer
 * @brief Get text area properties
 * @details
 * These enumerations are used with the function call:\n
 * \ref gText::GetAreaProp() "GetAreaProp()" to retrieve text area properties
 *
 */
enum gTextProp_t
{
	gTextProp_x1,		 		/**< x1 - Upper left x coordinate */
	gTextProp_y1,		 		/**< y1 - Upper left y coordinate */
	gTextProp_x2,		 		/**< x2 - Lower right x coordinate */
	gTextProp_y2,		 		/**< y2 - Lower right y coordinate */
	gTextProp_cols,		 		/**< Text colums in text area */
	gTextProp_rows,		 		/**< Text Rows in text area */
	gTextProp_FontWidth,		/**< Current Font rendered max fixed width */
	gTextProp_FontHeight,		/**< Current Font rendered height */
	gTextProp_minC,				/**< Minimum character code for current font */
	gTextProp_maxC,				/**< Maximum character code for current font */
};

typedef const uint8_t* Font_t;  	
typedef uint8_t (*FontCallback)(Font_t);

uint8_t ReadPgmData(const uint8_t* ptr);	//Standard Read Callback

/// @cond hide_from_doxygen
struct tarea
{
	uint8_t x1;
	uint8_t y1;
	uint8_t	x2;
	uint8_t y2;
	int8_t  mode;
};
/// @endcond

/**
 * @class gText
 * @brief Functions for Text Areas
 * @details
 * A text area acts like a terminal monitor and text output is displayed 
 * within the confines of a rectangle given in the DefineArea command.
 * All of the following text area functions operate on a user defined text area.
 */
  
 // graphical device text routines
class gText : public glcd_Device , public Print
{
  private:
    //FontCallback	FontRead;     // now static, move back here if each instance needs its own callback
	uint8_t			FontColor;
	Font_t			Font;
	struct tarea tarea;
	uint8_t			x;			// absolute X location on display (not relative to text area)
	uint8_t			y;			// absolute Y location on display (not relative to text area)
#ifndef GLCDCFG_NODEFER_SCROLL
	uint8_t			need_scroll; // set when text scroll has been defered
#endif

#ifdef GLCDCFG_UTF8
	int UTF8decode(wchar_t c);
#endif
	uint8_t SpecialChar(uint8_t c);

	void DrawStringSetXY(int slen, int hpos, int vpos, eraseLine_t erase = eraseNONE);
	// Scroll routines are private for now
	void ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	void ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);

  public:
/** @name CONSTRUCTORS
 * The following gText class constructors are available
 */
/*@{*/
	gText(); // default - uses the entire display
	gText(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode=DEFAULT_gTEXTMODE);
	gText(predefinedArea selection, gTextMode mode=DEFAULT_gTEXTMODE);
	gText(predefinedArea selection, Font_t font, gTextMode mode=DEFAULT_gTEXTMODE);
	gText(uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode=DEFAULT_gTEXTMODE);
/*@}*/

/** @name TEXT FUNCTIONS
 * The following text functions are available
 */
/*@{*/
	// Text area functions

	uint8_t DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, gTextMode mode=DEFAULT_gTEXTMODE);
	uint8_t DefineArea(uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, Font_t font, gTextMode mode=DEFAULT_gTEXTMODE);
	uint8_t DefineArea(predefinedArea selection, Font_t font, gTextMode mode=DEFAULT_gTEXTMODE);
	uint8_t DefineArea(predefinedArea selection, gTextMode mode=DEFAULT_gTEXTMODE);
	void ClearArea(void);
	void SetAreaMode(gTextMode mode); // Set text modes
	void ClearAreaMode(gTextMode mode); // Clear text modes
	int GetAreaProp(gTextProp_t); // get text area property

	// Font Functions
	void SelectFont(Font_t font, uint8_t fgcolor=BLACK, FontCallback callback=ReadPgmData); // default arguments added, callback now last arg
	void SetFontColor(uint8_t fgcolor);
#ifdef GLCDCFG_UTF8
	int PutChar(wchar_t c);
#else
	int PutChar(uint8_t c);
#endif
	void Puts(const char *str);
	void Puts(const String &str); // for Arduino String Class
	void Puts_P(PGM_P str);
#if ARDUINO > 100  || defined(F)
	void Puts(const __FlashStringHelper *str); // for Arduino 1.0 F() macro
#endif

	void DrawString(const char *str,   int hpos, int vpos, eraseLine_t erase = eraseNONE);
	void DrawString(String &str, int hpos, int vpos, eraseLine_t erase = eraseNONE); // for Arduino String Class
	void DrawString_P(PGM_P str, int hpos, int vpos, eraseLine_t erase = eraseNONE);
#if ARDUINO > 100 || defined(F)
	void DrawString(const __FlashStringHelper *str, int hpos, int vpos, eraseLine_t erase = eraseNONE);
#endif
#ifdef flashStr
	void DrawString(FLASHSTRING str, int hpos, int vpos, eraseLine_t erase = eraseNONE);
#endif


#if ARDUINO < 100
	void write(uint8_t c);  // character output for print base class
#if defined(GLCDCFG_UTF8) || defined(DOXYGEN)
	void writeUTF8(wchar_t utfc);
#endif
#else
	size_t write(uint8_t c);  // character output for print base class
#if defined(GLCDCFG_UTF8) || defined(DOXYGEN)
	size_t writeUTF8(wchar_t utfc);
#endif
#endif

	void CursorTo( uint8_t column, uint8_t row); // 0 based coordinates for character columns and rows
	void CursorTo( int8_t column); // move cursor on the current row
	void CursorToXY( uint8_t x, uint8_t y); // coordinates relative to active text area
#ifdef GLCDCFG_UTF8
	uint8_t CharWidth(wchar_t c);
	uint8_t CharHeight(wchar_t c); // 0 returns default height of font
#else
	uint8_t CharWidth(uint8_t c);
	uint8_t CharHeight(uint8_t c); // 0 returns default height of font
#endif
	uint16_t StringWidth(const char* str);
	uint16_t StringWidth_P(PGM_P str);
	uint16_t StringWidth(String &str);
#if ARDUINO > 100 || defined(F)
	uint16_t StringWidth(const __FlashStringHelper *str);
#endif

	void EraseTextLine( eraseLine_t type=eraseTO_EOL); //ansi like line erase function 
	void EraseTextLine( uint8_t row); // erase the entire text line in the given row and move cursor to left position

    // legacy text output functions 
	void PrintNumber(long n);

#ifndef USE_ARDUINO_FLASHSTR	
	// when the following functionality is supported in arduino it will be removed from this library
	void printFlash(FLASHSTRING str);
    void printFlashln(FLASHSTRING str);
#endif

#ifndef GLCDCFG_NO_PRINTF
	void Printf(const char *format, ...);
	void Printf(const __FlashStringHelper *format, ...);
	void Printf_P(PGM_P format, ...);
#endif
/*@}*/

};

#endif
