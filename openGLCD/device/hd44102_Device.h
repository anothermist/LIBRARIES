/*
  hd44102.h - openGLCD library support for graphic LCDs 

 vi:ts=4
  
*/

#ifndef GLCD_PANEL_DEVICE_H
#define GLCD_PANEL_DEVICE_H

/*
 * define name for Device
 */
#define glcdDev_DeviceName "hd44102"

/*
 * Sanity check config pins
 *
 *	Help the user detect pin configuration errors by
 *	detecting when defines are missing or are incorrect.
 *
 */

#if glcd_CHIP_COUNT == 2 && !defined(glcd_CHIP1)
#error "2 chips defined but glcd_CHIP1 chip selects not defined"
#endif

#if glcd_CHIP_COUNT == 3 && !defined(glcd_CHIP2)
#error "3 chips defined but glcd_CHIP2 chip selects not defined"
#endif

#if glcd_CHIP_COUNT == 4 && !defined(glcd_CHIP3)
#error "4 chips defined but glcd_CHIP3 chip selects not defined"
#endif

#if defined(glcd_CHIP3) && glcd_CHIP_COUNT < 4
#error "glcd_CHIP3 defined for less than 4 chips"
#endif

#if defined(glcd_CHIP2) && glcd_CHIP_COUNT < 3
#error "glcd_CHIP2 defined for less than 3 chips"
#endif

#if defined(glcd_CHIP1) && glcd_CHIP_COUNT < 2
#error "glcd_CHIP1 defined for less than 2 chips"
#endif


#if DISPLAY_WIDTH > 255
#warning "Current code only suports up to 255 pixels, downsizing width to 255"
#undef DISPLAY_WIDTH
#define DISPLAY_WIDTH 255
#endif


#ifndef glcdPinEN
#error "HD44102 configuration missing glcdPinEN"
#endif

#ifdef glcdPinE1
#error "HD44102 configuration does not use glcdPinE1"
#endif
#ifdef glcdPinE2
#error "HD44102 configuration does not use glcdPinE2"
#endif

/*
 * LCD commands -------------------------------------------------------------
 */

#define LCD_ON				0x39
#define LCD_OFF				0x38
#define LCD_DISP_START		0xC0
 
#define LCD_SET_XYADD		0x00 // sets x & y address  (page & column)
#define LCD_SET_UPMODE		0x3B // increase column address on reads/writes
#define LCD_SET_DNMODE		0x3A // decrease column address on reads/writes

/*
 * Status register bits/flags -----------------------------------------------
 */

#define LCD_BUSY_BIT		7
#define LCD_BUSY_FLAG		0x80 

#define LCD_RESET_BIT		4
#define LCD_RESET_FLAG		0x10

/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */

#define glcdDev_SelectChip(chip) 	SelectChip(chip)

/*
 * hd44012 doesn't use/need a chip # to strobe EN
 * as there is only a single EN line, so ignore the chip parameter.
 */
#define glcdDev_ENstrobeHi(chip) glcdio_WritePin(glcdPinEN, 1)
#define glcdDev_ENstrobeLo(chip) glcdio_WritePin(glcdPinEN, 0)

/*
 * Convert glcd X & Y coordinates to chip values
 * The macros below assume that chips are either
 * horizontal (normal) or vertical.
 */
#if CHIP_HEIGHT < DISPLAY_HEIGHT
#define glcdDev_XYval2Chip(x,y) ((x/CHIP_WIDTH) + ((y/CHIP_HEIGHT) * (DISPLAY_HEIGHT/CHIP_HEIGHT)))
#else
#define glcdDev_XYval2Chip(x,y)		((x/CHIP_WIDTH))	
#endif

#define GLCD_HD44102_COLREVERSED // set if columns for chip have column 0 on right vs left
									// NOTE: test for now, if this works, then this define will move
									// to the config file.

#ifdef GLCD_HD44102_COLREVERSED
#define glcdDev_Xval2ChipCol(x)		((CHIP_WIDTH - 1)  - ((x) % CHIP_WIDTH))
#else
#define glcdDev_Xval2ChipCol(x)		((x) % CHIP_WIDTH)
#endif

#define glcdDev_Yval2ChipPage(y)	((y) / 8)

#define glcdDev_PageCol2XYaddr(page, col)	((page << 6) | col)
#define glcdDev_XYval2XYaddr(x,y)			(glcdDev_PageCol2XYaddr( \
											glcdDev_Yval2ChipPage(y), glcdDev_Xval2ChipCol(x)))

/*
 * Custom init routine
 * This is to ensure that the up/down counter mode is set properly
 * The main line code depends on the chip advancing the column during reads/writes
 * If the chips are wired to the LCD with column 0 on the right, then
 * the chip will need to be run in DownMode and the libary will have to flip
 * the column addresses before setting the column in the chip.
 */


#ifdef GLCD_HD44102_COLREVERSED
#define LCD_SET_COLMODE LCD_SET_DNMODE
#else
#define LCD_SET_COLMODE LCD_SET_UPMODE
#endif

#define glcdDev_Init(chip)						\
({												\
uint8_t status;									\
	WriteCommand(LCD_ON, chip);					\
	WriteCommand(LCD_DISP_START, chip);			\
	WriteCommand(LCD_SET_COLMODE, chip);	    /* Set column direction for chip reads/writes */ \
	status = GLCD_ENOERR;						\
	status;	/* return value of macro */			\
})



#endif //GLCD_PANEL_DEVICE_H
