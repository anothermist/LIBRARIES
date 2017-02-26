/*
  glcd_Device.cpp - openGLCD library support
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
 
  The glcd_Device class implements the protocol for sending and receiving 
  data and commands to a GLCD device.
  It uses glcd_io.h to for the io primitives and openGLCD_Config.h for user
  specific configuration.

*/

#include "include/glcd_Device.h"
#include "include/glcd_errno.h"


/*
 * define the static variables declared in glcd_Device
 */

/// @cond hide_from_doxygen

// note this is intionally not the glcd_device_mode enum 
// since uint8_t is faster on AVR
// because enums are int which is 16 bits on AVR.

uint8_t		glcd_Device::Inverted;

lcdCoord  	glcd_Device::Coord;
/// @endcond

/*
 * Experimental defines
 */

//#define TRUE_WRITE	// does writes to glcd memory on page crossings vs ORs
						// This option only affects writes that span LCD pages.
						// None of the graphic rouintes nor the NEW_FONTDRAW rendering option do this.
						// Only the old font rendering and bitmap rendering do unaligned PAGE writes.
						// While this fixes a few issus for the old routines,
						// it also creates new ones.
						// The issue is routines like the bitmap rendering
						// routine attempt to use a drawing method that does not work.
						// when this is on, pixels are no longer ORd in but are written in.
						// so all the good/desired pixels get set, but then so do some
						// undesired pixels.
						//
						// current RECOMMENDED setting: OFF

#define GLCD_TEENSY_PCB_RESET_WAIT	// turns on code to deal with slow rising reset on Teensy PCB ADAPTER
								// this code is only turned on when library is compiled for teensy boards.
							// The reason for this was to support a ks0108 GLCD adapter PCB for the Teensy.
							// The reset signal created on that board is 250ms long 
							// but rises very slow so reset polling
							// does not work properly. So for now the code simply does a wait of 250ms
							// to give the Teensy PCB reset circuit time to clear.
							//

// NOTE: There used to be code to poll the RESET status. This code has been
// removed since it was not reliable on some modules and was larger than a blind wait.
// The comment about it was left here as historical information.

//#define GLCD_POLL_RESET	// turns on code to poll glcd module RESET signal
							// While this would be optimal, it turns out that on slow
							// rising reset signals to the GLCD the reset bit will clear
							// in the status *before* reset to the actual glcd chips.
							// this creates a situation where the code starts sending commands
							// to the display prior to it being ready. And unfortunately, the first
							// commands sent are to turn on the display. Since the glcd command protocol
							// only has a busy bit, commands appear to work as busy will not be
							// asserted during this reset "grey area". 
							//
							// When enabled the code is 50+ bytes larger than a dumb/blind wait and it 
							// also isn't clear if reset polling works the same across all glcds as 
							// the datasheets don't fully document how it works.
							//
							// For now, RESET polling is disabled, and the teensy code 
							// will get a longer blind delay to allow the code to operate on the 
							// teensy GLCD adapter board which has a very slow rising reset pulse.


//#define GLCD_XCOL_SUPPORT	//turns on code to track the hardware X/column to minimize set column commands.
							// Believe it or not, the code on the ks0108s runs slower with this
							// enabled.


#ifdef GLCDCFG_READ_CACHE
/*
 * Declare a static buffer for the Frame buffer for the Read Cache
 */
uint8_t glcd_rdcache[DISPLAY_HEIGHT/8][DISPLAY_WIDTH];
#endif

	
glcd_Device::glcd_Device()
{
  
}

/**
 * set pixel at x,y to the given color
 *
 * @param x X coordinate, a value from 0 to GLCD.Width-1
 * @param y Y coordinate, a value from 0 to GLCD.Heigh-1
 * @param color
 *
 * Sets the pixel at location x,y to the specified color.
 * x and y are relative to the 0,0 origin of the display which
 * is the upper left corner.
 * Requests to set pixels outside the range of the display will be ignored.
 *
 * @note If the display has been set to ::INVERTED mode then the color
 * will be automically reversed.
 *
 */

void glcd_Device::SetDot(uint8_t x, uint8_t y, uint8_t color) 
{
	uint8_t data;

	if((x >= DISPLAY_WIDTH) || (y >= DISPLAY_HEIGHT))
		return;
	
	this->GotoXY(x, y-y%8);					// read data from display memory
  	
	data = this->ReadData();
	if(color == PIXEL_ON)
	{
		data |= 0x01 << (y%8);				// set dot
	}
	else
	{
		data &= ~(0x01 << (y%8));			// clear dot
	}	
	this->WriteData(data);					// write data back to display
}

/**
 * set an area of pixels
 *
 * @param x X coordinate of upper left corner
 * @param y Y coordinate of upper left corner
 * @param x2 X coordinate of lower right corner
 * @param y2 Y coordinate of lower right corner
 * @param color
 *
 * sets the pixels an area bounded by x,y to x2,y2 inclusive
 * to the specified color.
 * x,y is the upper left corner and x2,y2 is the lower left corner.
 *
 * The width of the area is x2-x + 1. 
 * The height of the area is y2-y+1 
 * 
 *
 */

// set pixels from upper left edge x,y to lower right edge x1,y1 to the given color
// the width of the region is x1-x + 1, height is y1-y+1 

void glcd_Device::SetPixels(uint8_t x, uint8_t y,uint8_t x2, uint8_t y2, uint8_t color)
{
uint8_t mask, pageOffset, h, i, data;
uint8_t height = y2-y+1;
uint8_t width = x2-x+1;
	
	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset)
	{
		mask >>= (8-height);
		h = height;
	}
	else
	{
		h = 8-pageOffset;
	}
	mask <<= pageOffset;
	
	this->GotoXY(x, y);
	for(i=0; i < width; i++)
	{
		data = this->ReadData();
		
		if(color == PIXEL_ON)
		{
			data |= mask;
		}
		else
		{
			data &= ~mask;
		}

		this->WriteData(data);
	}
	
	while(h+8 <= height)
	{
		h += 8;
		y += 8;
		this->GotoXY(x, y);
		
		for(i=0; i <width; i++)
		{
			this->WriteData(color);
		}
	}
	
	if(h < height)
	{
		mask = ~(0xFF << (height-h));
		this->GotoXY(x, y+8);
		
		for(i=0; i < width; i++)
		{
			data = this->ReadData();
		
			if(color == PIXEL_ON)
			{
				data |= mask;
			}
			else
			{
				data &= ~mask;
			}
	
			this->WriteData(data);
		}
	}
}

/**
 * set current x,y coordinate on display device
 *
 * @param x X coordinate
 * @param y Y coordinate
 *
 * Sets the current pixel location to x,y.
 * x and y are relative to the 0,0 origin of the display which
 * is the upper left most pixel on the display.
 */

void glcd_Device::GotoXY(uint8_t x, uint8_t y)
{
  uint8_t chip, cmd;

  if((x == this->Coord.x) && (y == this->Coord.y))
	return;

  if( (x > DISPLAY_WIDTH-1) || (y > DISPLAY_HEIGHT-1) )	// exit if coordinates are not legal  
  {
    return;
  }

  this->Coord.x = x;								// save new coordinates
  this->Coord.y = y;

  chip = glcdDev_XYval2Chip(x, y);

#ifdef LCD_SET_XYADD
  	this->Coord.chip[chip].page = glcdDev_Yval2ChipPage(y);
	cmd = (LCD_SET_XYADD | glcdDev_XYval2XYaddr(x,y));
	this->WriteCommand(cmd, chip);	
#else
	if(y/8 != this->Coord.chip[chip].page)
	{
  		this->Coord.chip[chip].page = y/8;
		cmd = LCD_SET_PAGE | this->Coord.chip[chip].page;
	   	this->WriteCommand(cmd, chip);	
	}
	
	/*
	 * NOTE: For now, the "if" below is intentionally commented out.
	 * In order for this to work, the code must properly track
	 * the x coordinate of the chips and not allow it go beyond proper
	 * boundaries. It isnn't complicated to do, it just isn't done that
	 * way right now. We currently allow the coordinates to be larger than
	 * the actual geometry and just ignore them. This allows things like
	 * graphic functions to plot things larger than the display but display
	 * those portions that land within the diplays geometry.
	 */

	x = glcdDev_Xval2ChipCol(x);

#ifdef GLCD_XCOL_SUPPORT
	if(x != this->Coord.chip[chip].col)
#endif
	{

#ifdef GLCD_XCOL_SUPPORT
		this->Coord.chip[chip].col = x;
#endif

#ifdef LCD_SET_ADDLO
		cmd = LCD_SET_ADDLO | glcdDev_Col2addrlo(x);
	   	this->WriteCommand(cmd, chip);	

		cmd = LCD_SET_ADDHI | glcdDev_Col2addrhi(x);
	   	this->WriteCommand(cmd, chip);	
#else
		cmd = LCD_SET_ADD | x;
	   	this->WriteCommand(cmd, chip);	
#endif
	}
#endif // LCD_SET_XYADD
}
/**
 * Low level h/w initialization of library and display.
 *
 * @param invert specifices whether display is in normal mode or inverted mode.
 *
 * This should be called prior to any other graphic library function.
 * It does all the needed initializations including taking care of the
 * low level hardware initalization of the display device.
 *
 * The optional @em invert parameter specifies if the display should be run in a normal
 * mode, or inverted mode.\n
 * It does not refer to whether the display uses light or darg pixels.
 *
 * In "normal" or ::NON_INVERTED mode, pixels are turned on for ::PIXEL_ON or ::BLACK
 * and turned off for ::PIXEL_OFF or ::WHITE.
 *
 * In "inverted" or ::INVERTED mode, pixels are turned off for ::PIXEL_ON or ::BLACK
 * and turned on for ::PIXEL_OFF or ::WHITE.
 *
 * ::NON_INVERTED mode clears the display with all pixels off and drawing with
 * ::PIXEL_ON or ::BLACK will turn on pixels.
 *
 * ::INVERTED mode clears the display with all pixels on and drawing with ::PIXEL_OFF
 * or ::WHITE will turn on pixels.
 *
 * The terminology of ::BLACK and ::WHITE is from the perspective of a display
 * that uses black pixels on a light background.
 * For example, if a pixel is set to ::BLACK and used on a black pixel on light background display,
 * you get a black pixel.\n
 * However, if the display is a white pixel on dark background display,
 * then setting a pixel to ::BLACK turns on a white pixel on the display.
 *
 * Because of this, it can be less confusing to use ::PIXEL_ON and ::PIXEL_OFF vs ::BLACK and ::WHITE.
 *
 * Upon successful completion of the initialization: 
 * - the display pixels will be enabled
 * - the backlight will be turned on (if configured)
 * - the entire display will be cleared
 * - the x,y postion will be set to 0,0
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @note
 * This function can also be called to re-initliaze the display hardware.
 *
 */


int glcd_Device::Init(glcd_device_mode invert)
{  

	/*
	 * Now setup the pinmode for all of our control pins.
	 * The data lines will be configured as necessary when needed.
	 */

	glcdio_PinMode(glcdPinDI,OUTPUT);	
	glcdio_PinMode(glcdPinRW,OUTPUT);	

#ifdef glcdPinE1
	glcdio_PinMode(glcdPinE1,OUTPUT);	
	glcdio_WritePin(glcdPinE1,LOW); 	
#endif
#ifdef glcdPinE2
	glcdio_PinMode(glcdPinE2,OUTPUT);	
	glcdio_WritePin(glcdPinE2,LOW); 	
#endif

#ifdef glcdPinE3
	glcdio_PinMode(glcdPinE3,OUTPUT);	
	glcdio_WritePin(glcdPinE3,LOW); 	
#endif

#ifdef glcdPinE4
	glcdio_PinMode(glcdPinE4,OUTPUT);	
	glcdio_WritePin(glcdPinE4,LOW); 	
#endif

#ifdef glcdPinEN
	glcdio_PinMode(glcdPinEN,OUTPUT);	
	glcdio_WritePin(glcdPinEN, LOW);
#endif

#ifdef glcdPinCSEL1
	glcdio_PinMode(glcdPinCSEL1,OUTPUT);
	glcdio_WritePin(glcdPinCSEL1, LOW);
#endif

#ifdef glcdPinCSEL2
	glcdio_PinMode(glcdPinCSEL2,OUTPUT);
	glcdio_WritePin(glcdPinCSEL2, LOW);
#endif

#ifdef glcdPinCSEL3
	glcdio_PinMode(glcdPinCSEL3,OUTPUT);
	glcdio_WritePin(glcdPinCSEL3, LOW);
#endif

#ifdef glcdPinCSEL4
	glcdio_PinMode(glcdPinCSEL4,OUTPUT);
	glcdio_WritePin(glcdPinCSEL4, LOW);
#endif

#ifdef glcdPinCSEL5
	glcdio_PinMode(glcdPinCSEL5,OUTPUT);
	glcdio_WritePin(glcdPinCSEL5, LOW);
#endif

#ifdef glcdPinCSEL6
	glcdio_PinMode(glcdPinCSEL6,OUTPUT);
	glcdio_WritePin(glcdPinCSEL6, LOW);
#endif

#ifdef glcdPinCSEL7
	glcdio_PinMode(glcdPinCSEL7,OUTPUT);
	glcdio_WritePin(glcdPinCSEL7, LOW);
#endif

#ifdef glcdPinCSEL8
	glcdio_PinMode(glcdPinCSEL8,OUTPUT);
	glcdio_WritePin(glcdPinCSEL8, LOW);
#endif

#ifdef glcdPinCSEL9
	glcdio_PinMode(glcdPinCSEL9,OUTPUT);
	glcdio_WritePin(glcdPinCSEL9, LOW);
#endif

	/*
	 * If reset control
	 */
#ifdef glcdPinRES
	glcdio_PinMode(glcdPinRES,OUTPUT);
#endif

	/*
	 * If backlight control
	 */

#ifdef glcdPinBL
	glcdio_PinMode(glcdPinBL,OUTPUT);
#endif


	glcdio_SetRWDI(LOW, LOW); // R/W = LOW, D/I = LOW

	this->Coord.x = -1;  // invalidate the s/w coordinates so the first GotoXY() works
	this->Coord.y = -1;  // invalidate the s/w coordinates so the first GotoXY() works
	
	this->Inverted = invert;

#ifdef glcdPinRES
	/*
	 * Reset the glcd module if there is a reset pin defined
	 */ 
	glcdio_Reset();
	glcdio_DelayMilliseconds(2);  
	glcdio_UnReset();
#endif

	/*
	 *  extra blind delay for slow rising external reset signals
	 *  and to give time for glcd to get up and running
	 */
	glcdio_DelayMilliseconds(50); 

#if defined(GLCD_TEENSY_PCB_RESET_WAIT) && defined(CORE_TEENSY)
	/*
	 * Delay for Teensy ks0108 PCB adapter reset signal
	 * Reset polling is not realiable by itself since the reset signal is
	 * very slow rising, so this is easier and much less code
	 * - see long comment above where GLCD_POLL_RESET is defined
	 */
	glcdio_DelayMilliseconds(250);
#endif


	/*
	 * Each chip on the module must be initliazed
	 */

	for(uint8_t chip=0; chip < glcd_CHIP_COUNT; chip++)
	{
	uint8_t status;

#ifndef GLCDCFG_NOINIT_CHECKS
		/*
		 * At this point RESET better be complete and the glcd better have
		 * cleared BUSY status for the chip and be ready to go.
		 * So we check them and if the GLCD chip is not ready to go, we fail the init.
		 */

		status = this->GetStatus(chip);
		if(glcdio_IsResetStatus(status))
			return(GLCD_ERESET);
		if(glcdio_IsBusyStatus(status))
			return(GLCD_EBUSY);
#endif
			
		/*
		 * flush out internal state to force first GotoXY() to talk to GLCD hardware
		 */
		this->Coord.chip[chip].page = -1;
#ifdef GLCD_XCOL_SUPPORT
		this->Coord.chip[chip].col = -1;
#endif

#ifdef glcdDev_Init // this provides custom chip specific init 

		status = glcdDev_Init(chip);	// call device specific initialization if defined    
		if(status)
			return(status);
#else
		this->WriteCommand(LCD_ON, chip);			// display on
		this->WriteCommand(LCD_DISP_START, chip);	// display start line = 0
#endif

	}

	/*
	 * All hardware initialization is complete.
	 *
	 * Now, turn on the backlight, clear the screen and home the cursor to
	 * ensure that the display always starts
	 * in an identical state after being initialized.
	 *
	 * Note: the reason that SetPixels() below always uses ::PIXEL_OFF, is that once the
	 * the invert flag is in place, the lower level read/write code will invert data
	 * as needed.
	 * So clearing an area to ::PIXEL_OFF when the mode is ::INVERTED will set the area to
	 * all pixels on/off as is required by the state of the invert flag.
	 */

	this->OnBacklight(); // turn on backlight

	this->SetPixels(0,0, DISPLAY_WIDTH-1,DISPLAY_HEIGHT-1, PIXEL_OFF); // clear display
	this->GotoXY(0,0); // home cursor position

	return(GLCD_ENOERR);
}
/**
 * Turn on the backlight
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @see OffBacklight()
 * @see OnDisplay()
 * @see On()
 */
int glcd_Device::OnBacklight(void)
{
#ifdef glcdPinBL
	glcdio_BLon();
	return(GLCD_ENOERR);		// worked ok
#else
	return(GLCD_ENOTSUPPORTED); // no backlight control
#endif
}

/**
 * Turn off the backlight
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @see OnBacklight()
 * @see OffDisplay()
 * @see Off()
 */
int glcd_Device::OffBacklight(void)
{
#ifdef glcdPinBL
	glcdio_BLoff();
	return(GLCD_ENOERR);		// worked ok
#else
	return(GLCD_ENOTSUPPORTED); // no backlight control
#endif
}

/**
 * Set backlight brightness
 *
 * @param val backlight brightness (0-255)
 *
 * If the display circuitry only has the option to turn the backlight on and off
 * then any non zero value will turn the display on with maximum brightness
 * The value HIGH should not be used to turn the backlight on since this function
 * is used to control backlight intensity and not for on / off control.
 * A value of HIGH is typically 1 which would be a very dim backlight.
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @see OnBacklight() OffBacklight()
 *
 * @note
 * Currently dimming is not yet supported.
 */

int glcd_Device::SetBacklight(int val)
{
	/*
	 * For now, simply handle on/off
	 */
	if(val)
		return(OnBacklight());
	else
		return(OffBacklight());
}

/**
 * Turn the display pixels on
 *
 * Turns on the display pixels. It does not alter or modify the backlight setting.
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @see OffDisplay()
 * @see OnBacklight()
 * @see On()
 */
int glcd_Device::OnDisplay(void)
{
#ifdef LCD_ON
	for(uint8_t chip=0; chip < glcd_CHIP_COUNT; chip++)
	{
		this->WriteCommand(LCD_ON, chip);			// display on
	}
	return(GLCD_ENOERR);
#else
	return(GLCD_ENOTSUPPORTED); // not supported
#endif
}

/**
 * Turn the display pixels off
 *
 * Turns off the display pixels. It does not alter or modify the backlight setting.
 * If the display backlight is on, it will remain on.
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @see OnDisplay()
 * @see OffBacklight()
 * @see Off()
 */
int glcd_Device::OffDisplay(void)
{
#ifdef LCD_OFF
	for(uint8_t chip=0; chip < glcd_CHIP_COUNT; chip++)
	{
		this->WriteCommand(LCD_OFF, chip);			// display on
	}
	return(GLCD_ENOERR);
#else
	return(GLCD_ENOTSUPPORTED); // not supported
#endif
}

/**
 * Turn on the display & backlight
 *
 * Turns on the display's pixels and backlight
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @note
 *	The backlight will be turned on at full intensity
 *
 * @see Off()
 * @see OnDisplay()
 * @see OnBacklight()
 */
int glcd_Device::On(void)
{
	OnDisplay();
	OnBacklight();
	return(GLCD_ENOERR);
}
/**
 * Turn off the display & backlight
 *
 * Turns off the display's pixels and backlight
 *
 * @returns 0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * @see On()
 * @see OffDisplay()
 * @see OffBacklight()
 */
int glcd_Device::Off(void)
{
	OffBacklight();
	OffDisplay();
	return(GLCD_ENOERR);
}


#ifdef glcd_CHIP0  // if at least one chip select string
__inline__ void glcd_Device::SelectChip(uint8_t chip)
{  

#ifdef glcd_CHIP8
	if(chip == 8) glcdio_ChipSelect(glcd_CHIP8); else
#endif
#ifdef glcd_CHIP7
	if(chip == 7) glcdio_ChipSelect(glcd_CHIP7); else
#endif
#ifdef glcd_CHIP6
	if(chip == 6) glcdio_ChipSelect(glcd_CHIP6); else
#endif
#ifdef glcd_CHIP5
	if(chip == 5) glcdio_ChipSelect(glcd_CHIP5); else
#endif
#ifdef glcd_CHIP4
	if(chip == 4) glcdio_ChipSelect(glcd_CHIP4); else
#endif
#ifdef glcd_CHIP3
	if(chip == 3) glcdio_ChipSelect(glcd_CHIP3); else
#endif
#ifdef glcd_CHIP2
	if(chip == 2) glcdio_ChipSelect(glcd_CHIP2); else
#endif
#ifdef glcd_CHIP1
	if(chip == 1) glcdio_ChipSelect(glcd_CHIP1); else
#endif
	glcdio_ChipSelect(glcd_CHIP0);
}
#endif

// return lcd status bits
uint8_t glcd_Device::GetStatus(uint8_t chip)
{
uint8_t status;

	glcdDev_SelectChip(chip);
	glcdio_DataDirIn(1);			// input mode with pullups enabled
	glcdio_SetRWDI(HIGH, LOW);		// R/W = HIGH, D/I = LOW
//	glcdio_DelayNanoseconds(GLCD_tAS);
	glcdDev_ENstrobeHi(chip);
	glcdio_DelayNanoseconds(GLCD_tDDR);

	status = glcdio_ReadByte();	// Read status bits

	glcdDev_ENstrobeLo(chip);
	return(status);
}


// wait until LCD busy bit goes to zero
void glcd_Device::WaitReady( uint8_t chip)
{
	glcdDev_SelectChip(chip);
	glcdio_DataDirIn(0);				// without pullups enabled
	glcdio_SetRWDI(HIGH, LOW);			// R/W = HIGH, D/I = LOW
//	glcdio_DelayNanoseconds(GLCD_tAS);
	glcdDev_ENstrobeHi(chip);
	glcdio_DelayNanoseconds(GLCD_tDDR);

	while(glcdio_RdBusystatus())
	{
       ;
	}
	glcdDev_ENstrobeLo(chip);
}

/*
 * read a single data byte from chip
 */
uint8_t glcd_Device::DoReadData()
{
	uint8_t data, chip;

	chip = glcdDev_XYval2Chip(this->Coord.x, this->Coord.y);

	this->WaitReady(chip);
	glcdio_SetRWDI(HIGH, HIGH);			// RW = HIGH, D/I = HIGH
	glcdio_DelayNanoseconds(GLCD_tAS);
	glcdDev_ENstrobeHi(chip);
	glcdio_DelayNanoseconds(GLCD_tDDR);

	data = glcdio_ReadByte();	// Read the data bits from the LCD

	glcdDev_ENstrobeLo(chip);
#ifdef GLCD_XCOL_SUPPORT
	this->Coord.chip[chip].col++;
#endif
	return data;
}
/**
 * read a data byte from display device memory
 *
 * @return the data byte at the current x,y position
 *
 * @note the current x,y location is not modified by the routine.
 *	This allows a read/modify/write operation.
 *	Code can call ReadData() modify the data then
 *  call WriteData() and update the same location.
 *
 * @see WriteData()
 */

#ifdef GLCDCFG_READ_CACHE
uint8_t glcd_Device::ReadData()
{
uint8_t x, data;
	x = this->Coord.x;
	if(x >= DISPLAY_WIDTH)
	{
		return(0);
	}
	data = glcd_rdcache[this->Coord.y/8][x];

	if(this->Inverted)
	{
		data = ~data;
	}
	return(data);
}
#else

uint8_t glcd_Device::ReadData()
{  
uint8_t x, data;


	x = this->Coord.x;
	if(x >= DISPLAY_WIDTH)
	{
		return(0);
	}

	this->DoReadData();				// dummy read

	data = this->DoReadData();			// "real" read

	if(this->Inverted)
	{
		data = ~data;
	}

	this->Coord.x = -1;	// force a set column on GotoXY

	this->GotoXY(x, this->Coord.y);	
	return(data);
}
#endif

void glcd_Device::WriteCommand(uint8_t cmd, uint8_t chip)
{
	this->WaitReady(chip);
	glcdio_SetRWDI(LOW, LOW);			// R/W = LOW,  D/I = LOW

	glcdio_DataDirOut();

	glcdio_WriteByte(cmd);		/* This could be done before or after raising E */
	glcdio_DelayNanoseconds(GLCD_tAS);
	glcdDev_ENstrobeHi(chip);
	glcdio_DelayNanoseconds(GLCD_tWH);
	glcdDev_ENstrobeLo(chip);
}


/**
 * Write a byte to display device memory
 *
 * @param data date byte to write to memory
 *
 * The data specified is written to glcd memory at the current
 * x,y position. If the y location is not on a byte boundary, the write
 * is fragemented up into multiple writes.
 *
 * @note the full behavior of this during split byte writes
 * currently varies depending on a compile time define. 
 * The code can be configured to either OR in 1 data bits or set all
 * the data bits.
 * @b TRUE_WRITE controls this behavior.
 *
 * @note the x,y address will not be the same as it was prior to this call.
 * 	The y address will remain the aame but the x address will advance by one.
 *	This allows back to writes to write sequentially through memory without having
 *	to do additional x,y positioning.
 *
 * @see ReadData()
 *
 */

void glcd_Device::WriteData(uint8_t data)
{
	uint8_t displayData, yOffset, chip;
	//showHex("wrData",data);
    //showXY("wr", this->Coord.x,this->Coord.y);

	if(this->Coord.x >= DISPLAY_WIDTH)
	{
		return;
	}

    chip = glcdDev_XYval2Chip(this->Coord.x, this->Coord.y);
	
	yOffset = this->Coord.y%8;

	if(yOffset != 0)
	{
		// first page
		displayData = this->ReadData();
		this->WaitReady(chip);
		glcdio_SetRWDI(LOW, HIGH);					// R/W = LOW, D/I = HIGH
		glcdio_DataDirOut();						// data port is output
		glcdio_DelayNanoseconds(GLCD_tAS);
		glcdDev_ENstrobeHi(chip);
		
#ifdef TRUE_WRITE
		/*
		 * Strip out bits we need to update.
		 */
		displayData &= (_BV(yOffset)-1);
#endif

		displayData |= data << yOffset;

		if(this->Inverted)
		{
			displayData = ~displayData;
		}
		glcdio_WriteByte( displayData);					// write data
		glcdio_DelayNanoseconds(GLCD_tWH);
		glcdDev_ENstrobeLo(chip);
#ifdef GLCDCFG_READ_CACHE
		glcd_rdcache[this->Coord.y/8][this->Coord.x] = displayData; // save to read cache
#endif

		// second page

		/*
		 * Make sure to goto y address of start of next page
		 * and ensure that we don't fall off the bottom of the display.
		 */
		uint8_t ysave = this->Coord.y;
		if(((ysave+8) & ~7) >= DISPLAY_HEIGHT)
		{
			this->GotoXY(this->Coord.x+1, ysave);
			return;
		}
	
		this->GotoXY(this->Coord.x, ((ysave+8) & ~7));

		displayData = this->ReadData();
		this->WaitReady(chip);

		glcdio_SetRWDI(LOW, HIGH);			// R/W = LOW, D/I = HIGH
		glcdio_DataDirOut();				// data port is output
		glcdio_DelayNanoseconds(GLCD_tAS);
		glcdDev_ENstrobeHi(chip);

#ifdef TRUE_WRITE
		/*
		 * Strip out bits we need to update.
		 */
		displayData &= ~(_BV(yOffset)-1);

#endif
		displayData |= data >> (8-yOffset);
		if(this->Inverted)
		{
			displayData = ~displayData;
		}
		glcdio_WriteByte(displayData);		// write data
		glcdio_DelayNanoseconds(GLCD_tWH);
		glcdDev_ENstrobeLo(chip);
#ifdef GLCDCFG_READ_CACHE
		glcd_rdcache[this->Coord.y/8][this->Coord.x] = displayData; // save to read cache
#endif
		this->GotoXY(this->Coord.x+1, ysave);
	}
	else 
	{
    	this->WaitReady(chip);

		glcdio_SetRWDI(LOW, HIGH);			// R/W = LOW, D/I = HIGH
		glcdio_DataDirOut();				// data port is output

		// just this code gets executed if the write is on a single page
		if(this->Inverted)
			data = ~data;	  

		glcdio_DelayNanoseconds(GLCD_tAS);
		glcdDev_ENstrobeHi(chip);
	
		glcdio_WriteByte(data);				// write data

		glcdio_DelayNanoseconds(GLCD_tWH);

		glcdDev_ENstrobeLo(chip);
#ifdef GLCDCFG_READ_CACHE
		glcd_rdcache[this->Coord.y/8][this->Coord.x] = data; // save to read cache
#endif

		/*
		 * NOTE/WARNING:
		 * This bump can cause the s/w X coordinate to bump beyond a legal value
		 * for the display. This is allowed because after writing to the display
		 * display, the column (x coordinate) is always bumped. However,
		 * when writing to the the very last column, the resulting column location 
		 * inside the hardware is somewhat undefined.
		 * Some chips roll it back to 0, some stop the maximu of the LCD, and others
		 * advance further as the chip supports more pixels than the LCD shows.
		 *
		 * So to ensure that the s/w is never indicating a column (x value) that is
		 * incorrect, we allow it bump beyond the end.
		 *
		 * Future read/writes will not attempt to talk to the chip until this
		 * condition is remedied (by a GotoXY()) and by having this somewhat
		 * "invalid" value, it also ensures that the next GotoXY() will always send
		 * both a set column and set page address to reposition the glcd hardware.
		 */

		this->Coord.x++;
#ifdef GLCD_XCOL_SUPPORT
		this->Coord.chip[chip].col++;
#endif


		/*
		 * Check for crossing into the next chip.
		 */
		if( glcdDev_XYval2Chip(this->Coord.x, this->Coord.y) != chip)
		{
			if(this->Coord.x < DISPLAY_WIDTH)
			{
				uint8_t x = this->Coord.x;
				this->Coord.x = -1;
				this->GotoXY(x, this->Coord.y);
			}
 		}
	    //showXY("WrData",this->Coord.x, this->Coord.y); 
	}
}
