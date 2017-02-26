/*
  openGLCD_GLCDv3.h - openGLCD library support - GLCDv3 compability
  Copyright (c) 2013-2016 Bill Perry

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

	Including this header file in a sketch just after <openGLCD.h> will
	provide GLCDv3 library compatibility for the routines that
	have changed or have been eliminated in openGLCD

    Note:
		For even greater temporary GLCDv3 compatibility,
		a dummy glcd.h header file can be created with the folling
		contents:
		#include <openGLCD.h>
		#include <include/openGLCD_GLCDv3.h>
		which should allow most existing GLCDv3 sketches to work "as is". 
		However, if this is done, GLCDv3 and openGLCD cannot both be
		installed at the same time since the sketch cannot be assured
		to get the glcd.h from the openGLCD library.

	Warning:

		While creating a glcd.h compatibility file can provide instant
		backward compatiblity to most existing GLCDv3 sketches,
		it should be cautioned, that those sketches will be using the functions
		and parameters as defined in GLCDv3 vs openGLCD.
		As such, the results obtained from the functions will not match
		the openGLCD documenation.
		Therefore, this method of backward compability is not recommeded as a
		permanent solution.

		Also note that the
		return status of DefineArea() will not be the same as GLCDv3.

		See the openGLCD documentation for further details.

*/

#ifndef __openGLCD_GLCDv3_H__
#define __openGLCD_GLCDv3_H__

/*
 * Note: openGLCD.h will include all the openGLCD fonts and bitmaps
 * GLCDv3 sketches will attempt to include font and bitmap headers as well since that was
 * requied in GLCDv3 but since all the font headers have already been included
 * the #include(s) in the sketch will essentially be ignored by the include gaurd ifdef
 * in the font/bitmap header.
 */

/*
 * map GLCDv3 function behvior to work in openGLCD
 * (must be above ks0108 functions unless the ks0108 target routine parameters are adjusted)
 *
 */

#define DrawVLine(x, y, height, ...) DrawVLine(x, y, (height+1), ##__VA_ARGS__)

#define DrawHLine(x, y, width, ...) DrawHLine(x, y, (width+1), ##__VA_ARGS__)

#define DrawRect(x, y, width, height, ...) \
		DrawRect(x, y, (width+1), (height+1), ##__VA_ARGS__)

#define DrawRoundRect(x, y, width, height, ...) \
		DrawRoundRect(x, y, (width+1), (height+1), ##__VA_ARGS__)

#define FillRect(x, y, width, height, ...) \
		FillRect(x, y, (width+1), (height+1), ##__VA_ARGS__)

#define FillRoundRect(x, y, width, height, ...) \
		FillRoundRect(x, y, (width+1), (height+1), ##__VA_ARGS__)

#define InvertRect(x, y, width, height, ...) \
		InvertRect(x, y, (width+1), (height+1), ##__VA_ARGS__)
/*
 * map really old ks0108 library functions
 * in case somebody is still using these.
 */
#define DrawVertLine(x, y, height, ...) DrawVLine(x,y,height, ##__VA_ARGS__)

#define DrawHoriLine(x, y, height, ...) DrawHLine(x,y,height, ##__VA_ARGS__)

#define ClearSysTextLine(line) \
		FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), PIXEL_OFF )

#define ClearPage(line, ...) \
		FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), ##__VA_ARGS__)

#define SetInverted(_invert)	SetDisplayMode(_invert) 

/*
 * map renamed GLCD function
 */

#define SetTextMode(_mode)	SetAreaMode(_mode) 

/*
 * Create s/w version defines for the classes
 * Use dummy values of 0 since these don't exist in openGLCD
 */
#define GLCD_VERSION 0
#define GTEXT_VERSION 0
#define GLCD_Device 0 

#endif // __openGLCD_GLCDv3_H__
