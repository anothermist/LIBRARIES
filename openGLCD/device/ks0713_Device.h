/*
  ks0713_Device.h - openGLCD library support for graphic LCDs 

 vi:ts=4

*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcdDev_DeviceName "ks0713"

/*
 * Sanity check KS0713 config pins
 *
 *	Help the user detect pin configuration errors by
 *	detecting when defines are missing or are incorrect.
 *
 *	Check for existence/non-existence of:
 *		glcdPinCSEL1
 *		glcdPinCSEL2
 *		glcdPinEN
 *		glcdPinE1
 *		glcdPinE2
 */

#ifdef glcdPinCSEL1
#error "KS0713 configuration does not use glcdPinCSEL1"
#endif
#ifdef glcdPinCSEL2
#error "KS0713 configuration does not use glcdPinCSEL2"
#endif

#ifndef glcdPinEN
#error "KS0713 configuration missing glcdPinEN"
#endif

#ifdef glcdPinE1
#error "KS0713 configuration does not use glcdPinE1"
#endif
#ifdef glcdPinE2
#error "KS0713 configuration does not use glcdPinE2"
#endif


/*
 * LCD commands -------------------------------------------------------------
 */
#define LCD_ON				0xAF	// Turn on LCD panel
#define LCD_OFF            	0xAE	// Turn off LCD panel
#define LCD_DISP_START		0x40	// Initial Disply Line

#define LCD_SET_PAGE		0xB0
#define LCD_SET_ADDLO     	0x00	// set column (Segment) address lo nibble
#define LCD_SET_ADDHI     	0x10	// set column (Segment) address hi nibble

#define LCD_ADC_RIGHTWWARD	0xA0		
#define LCD_ADC_LEFTWARD	0xA1		
#define LCD_REVERSELCD		0xa7	// white dots on black background.
#define LCD_NORMALLCD		0xa6	// black dots on white backbround

#define LCD_RESET			0xE2	// Reset command not signal reset

/*
 * Status register bits/flags -----------------------------------------------
 */

#define LCD_BUSY_FLAG		0x80 
#define LCD_BUSY_BIT		7

/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */


/*
 * The current ks0713 doesn't use chip selects
 * As it is currently only set up for a single chip
 */
#define glcdDev_SelectChip(chip)

/*
 * ks0713 currently doesn't use/need a chip # to strobe EN
 * as there is only a single EN line, so ignore the chip parameter.
 */
#define glcdDev_ENstrobeHi(chip) glcdio_WritePin(glcdPinEN, 1)
#define glcdDev_ENstrobeLo(chip) glcdio_WritePin(glcdPinEN, 0)


/*
 * Convert X coordinate to chip values
 */
#define glcdDev_XYval2Chip(x,y)		(0)	// no multi chip support yet.

#define glcdDev_Xval2ChipCol(x)	(x)	// no multi chip support yet

/*
 * Convert from chip column value to hi/lo address value
 * for LCD commands.
 */

#define glcdDev_Col2addrlo(x)		(x & 0xf)	// lo nibble
#define glcdDev_Col2addrhi(x)		(x & 0xf0)	// hi nibble

#endif //GLCD_PANEL_DEVICE_H
