/*
  glcd_Device.h - support for specific graphical LCDs
  Copyright (c) 2011-2016 Bill Perry
  Copyright (c) 2009, 2010 Michael Margolis and Bill Perry

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
  
  The glcd_Device class impliments the protocol for sending and receiving
  data and commands to a GLCD device.
  
*/

#ifndef	GLCD_DEVICE_H
#define GLCD_DEVICE_H

#include "openGLCD_Config.h"
#include "include/glcd_types.h"
#include "include/glcd_io.h"

#if defined (ARDUINO)
#include "Print.h" // used when deriving this class in Arduino 
#endif

/**
 * @defgroup glcd_Device_enum glcd_Device enumerations
 */

// useful user constants
/** @addtogroup glcd_Device_enum */
/*@{*/
/**
 * @hideinitializer
 * @brief \ref glcd_Device::Init() "Init()" initialization modes
 *
 * @details
 * In "normal" or ::NON_INVERTED mode, pixels are turned on for ::PIXEL_ON or ::BLACK
 * and turned off for ::PIXEL_OFF or ::WHITE.
 *
 * In "inverted" or ::INVERTED mode, pixels are turned off for ::PIXEL_ON or ::BLACK
 * and turned on for ::PIXEL_OFF or ::WHITE.
 *
 * ::NON_INVERTED mode clears the display with all pixels off and drawing with
 * ::PIXEL_ON or ::BLACK will turn on pixels.
 *
 * ::INVERTED mode clears the display with all pixels on and drawing with ::PIXEL_ON
 * or ::WHITE will turn on pixels.
 *
 * The terminology of ::BLACK and ::WHITE is from the perspective of a display
 * that uses black pixels on a light background.
 * For example, if a pixel is set to ::BLACK and used on a black pixel on light background display,
 * you get a black pixel.
 * If the display is a white pixel on dark background display,
 * then setting a pixel to ::BLACK turns on white pixel on the display.
 *
 * Because of this, it can be less confusing to use ::PIXEL_ON and ::PIXEL_OFF vs ::BLACK and ::WHITE.
 */
typedef enum {
	NON_INVERTED  = 0,  /**<"normal" mode */
	INVERTED = 1,  		/**<"inverted" mode */
}glcd_device_mode;

#define PIXEL_ON	0xFF /**<color to turn on pixel - same as ::BLACK */
#define BLACK		0xFF /**<color to turn on pixel  */
#define PIXEL_OFF	0x00 /**<color to turn off pixel - same as ::WHITE */
#define WHITE		0x00 /**<color to turn off pixel */

/*@}*/ // for addtogroup above

/// @cond hide_from_doxygen
typedef struct {
	uint8_t x;
	uint8_t y;
	struct {
#ifdef GLCD_XCOL_SUPPORT
		uint8_t col;
#endif
		uint8_t page;
	} chip[glcd_CHIP_COUNT];
} lcdCoord;
/// @endcond
	
/*
 * Note that all data in glcd_Device is static so that all derived instances  
 * (gText instances for example) share the same device state.
 * Any added data fields should also be static unless there is explicit reason
 * to not share the field among the instances 
 */
 
/**
 * @class glcd_Device
 * @brief Low level device functions
 * 
 */
class glcd_Device
{
  private:
  // Control functions
	uint8_t DoReadData(void);
	void WriteCommand(uint8_t cmd, uint8_t chip);
	inline void Enable(void);
	inline void SelectChip(uint8_t chip); 
	void WaitReady(uint8_t chip);
	uint8_t GetStatus(uint8_t chip);
	
  public:
    glcd_Device();
	protected: 
    int Init(glcd_device_mode invert = NON_INVERTED);      // now public, default is non-inverted
	void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void SetPixels(uint8_t x, uint8_t y,uint8_t x1, uint8_t y1, uint8_t color);
    uint8_t ReadData(void);        // now public
    void WriteData(uint8_t data); 
	int SetBacklight(int val);	// set backlight intensity
	int OnBacklight(void);		// turn on backlight (full intensity)
	int OffBacklight(void);		// turn off backlight
	int OnDisplay(void);		// turn on display (pixels)
	int OffDisplay(void);		// turn off display (pixels
	int On(void);				// turn on display & backlight
	int Off(void);				// turn of display & backlight

  	void GotoXY(uint8_t x, uint8_t y);   
/// @cond hide_from_doxygen
    static lcdCoord	  	Coord;  
	static uint8_t	 	Inverted; 
/// @endcond
};
  
#endif
