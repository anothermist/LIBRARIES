/*
  xxxk_Device.h - openGLCD library support for graphic LCDs 

 vi:ts=4

	This is a device template file that shows what needs to be
	in a device header for a glcd.

The following lcd primitives must be defined

	glcdDev_SelectChip(chip)
	glcdDev_ENstrobeHi(chip)
	glcdDev_ENstrobeLo(chip)
	glcdDev_Xval2Chip(x)
	glcdDev_Xval2ChipCol(x)

	For devices that split set column into to 2 commands of hi/lo: 

	glcdDev_Col2addrlo(x)		(x & 0xf)	// lo nibble
	glcdDev_Col2addrhi(x)		(x & 0xf0)	// hi nibble
  
*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcdDev_DeviceName "xxx"

/*
 * Sanity check XXX config pins
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

#ifndef glcdPinCSEL1
#error "XXXX configuration missing glcdPinCSEL1"
#endif
#ifndef glcdCSEL2
#error "XXXX configuration missing glcdPinCSEL2"
#endif

#ifdef glcdPinEN
#error "XXXX configuration missing glcdPinEN"
#endif

#ifdef glcdPinE1
#error "XXXX configuration does not use glcdPinE1"
#endif
#ifdef glcdPinE2
#error "XXXX configuration does not use glcdPinE2"
#endif


/*
 * LCD commands -------------------------------------------------------------
 */

#define LCD_ON
#define LCD_OFF
#define LCD_SET_ADD
#define LCD_DISP_START
#define LCD_SET_PAGE


/*
 * Status register bits/flags -----------------------------------------------
 */

#define LCD_BUSY_FLAG
#define LCD_BUSY_BIT



/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */

#define glcdDev_SelectChip(chip)

/*
 * Define how to strob EN signals
 */
#define glcdDev_ENstrobeHi(chip)
#define glcdDev_ENstrobeLo(chip)

/*
 * Define how to get low and how addresses
 * (if needed)
 */

/*
 * Convert X & Y coordinates to chip values
 */
#define	glcdDev_XYval2Chip(x,y)
#define	glcdDev_Xval2ChipCol(x)

/*
 * Convert from chip column value to hi/lo address value
 * for LCD commands.
 *	(If Needed)
 */

#define glcdDev_Col2addrlo(x)
#define glcdDev_Col2addrhi(x)

#endif //GLCD_PANEL_DEVICE_H
