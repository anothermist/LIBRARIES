/*
  glcd_io.h - openGLCD library support
  Copyright (c) 2009-2016 Bill Perry
  
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

 ------------------------------------------------------------------------

  This file maps abstract io requests from glcd_Device.
  glcd_arduino_io.h maps arduino pins to avr ports and pins.
  The raw direct port io for AVR is handled by macros in avrio.h
 
 */

#ifndef	GLCD_IO_H
#define GLCD_IO_H

#if ARDUINO < 100
#include "wiring.h"
#else
#include "Arduino.h"
#endif

#include "include/glcd_arduino_io.h"    // these macros map arduino pins & set CORE defines
#include GLCDCFG_GLCDCONFIG				// include glcd panel config file


/*
 * Must set AVRIO modes before including avrio
 */

#ifdef GLCDCFG_ATOMIC_IO
#define AVRIO_NO4BIT // for now disable nibble mode
#endif

#if defined(__AVR__) && !defined(GLCD_CORE_CORECODE)
#include "include/avrio.h"         // these macros do AVR direct port io    
#endif

#if defined(GLCD_CORE_CHIPKIT)
#include "pic32FastDigital.h" // definitions for digitalReadFAST()/digitalWriteFast()
#endif

/*
 * Check to make sure all data pins are defined and none are duplicates.
 */


#if !defined(glcdPinData0) || !defined(glcdPinData1) || !defined(glcdPinData2) || !defined(glcdPinData3) || !defined(glcdPinData4) || !defined(glcdPinData5) || !defined(glcdPinData6) || !defined(glcdPinData7)
#error not all gcdPinDataN data pins have been defined
#endif

#if (glcdPinData0 == glcdPinData1) || (glcdPinData0 == glcdPinData2) || (glcdPinData0 == glcdPinData3) || (glcdPinData0 == glcdPinData4) || (glcdPinData0 == glcdPinData5) || (glcdPinData0 == glcdPinData6) || (glcdPinData0 == glcdPinData7) 
#error gcdPinData0 is a duplicate data pin
#endif

#if (glcdPinData1 == glcdPinData0) || (glcdPinData1 == glcdPinData2) || (glcdPinData1 == glcdPinData3) || (glcdPinData1 == glcdPinData4) || (glcdPinData1 == glcdPinData5) || (glcdPinData1 == glcdPinData6) || (glcdPinData1 == glcdPinData7) 
#error gcdPinData1 is a duplicate data pin
#endif

#if (glcdPinData2 == glcdPinData0) || (glcdPinData2 == glcdPinData1) || (glcdPinData2 == glcdPinData3) || (glcdPinData2 == glcdPinData4) || (glcdPinData2 == glcdPinData5) || (glcdPinData2 == glcdPinData6) || (glcdPinData2 == glcdPinData7) 
#error gcdPinData2 is a duplicate data pin
#endif

#if (glcdPinData3 == glcdPinData0) || (glcdPinData3 == glcdPinData1) || (glcdPinData3 == glcdPinData2) || (glcdPinData3 == glcdPinData4) || (glcdPinData3 == glcdPinData5) || (glcdPinData3 == glcdPinData6) || (glcdPinData3 == glcdPinData7) 
#error gcdPinData3 is a duplicate data pin
#endif


#if (glcdPinData4 == glcdPinData0) || (glcdPinData4 == glcdPinData1) || (glcdPinData4 == glcdPinData2) || (glcdPinData4 == glcdPinData3) || (glcdPinData4 == glcdPinData5) || (glcdPinData4 == glcdPinData6) || (glcdPinData4 == glcdPinData7) 
#error gcdPinData4 is a duplicate data pin
#endif


#if (glcdPinData5 == glcdPinData0) || (glcdPinData5 == glcdPinData1) || (glcdPinData5 == glcdPinData2) || (glcdPinData5 == glcdPinData3) || (glcdPinData5 == glcdPinData4) || (glcdPinData5 == glcdPinData6) || (glcdPinData5 == glcdPinData7) 
#error gcdPinData5 is a duplicate data pin
#endif

#if (glcdPinData6 == glcdPinData0) || (glcdPinData6 == glcdPinData1) || (glcdPinData6 == glcdPinData2) || (glcdPinData6 == glcdPinData3) || (glcdPinData6 == glcdPinData4) || (glcdPinData6 == glcdPinData5) || (glcdPinData6 == glcdPinData7) 
#error gcdPinData6 is a duplicate data pin
#endif

#if (glcdPinData7 == glcdPinData0) || (glcdPinData7 == glcdPinData1) || (glcdPinData7 == glcdPinData2) || (glcdPinData7 == glcdPinData3) || (glcdPinData7 == glcdPinData4) || (glcdPinData7 == glcdPinData5) || (glcdPinData7 == glcdPinData6) 
#error gcdPinData7 is a duplicate data pin
#endif

 
/*
 * Map a glcd status bit to a pin.
 * In order to do this properly it takes concatenation
 * to convert the LCD status bit value to a pin
 */

#define xGLCD_STATUS_BIT2PIN(bit)    glcdPinData ## bit
#define GLCD_STATUS_BIT2PIN(bit)    xGLCD_STATUS_BIT2PIN(bit)    


#ifdef  _AVRIO_AVRIO_ // ================= Begin _AVRIO_AVRIO_ specific code =============================


#ifndef OUTPUT
#define OUTPUT 1
#endif

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif



/*
 * Set up the configured LCD data lines 
 */

#define glcdio_avrWriteByte(data) 			\
	avrio_Write8Bits(AVRIO_PORTREG,			\
			glcdPinData0, glcdPinData1,		\
			glcdPinData2, glcdPinData3,		\
			glcdPinData4, glcdPinData5,		\
			glcdPinData6, glcdPinData7, data)


/*
 * Read the configured LCD data lines and return data as 8 bit byte
 */
#define glcdio_avrReadByte() 				\
	avrio_Read8Bits(AVRIO_PINREG,			\
			glcdPinData0, glcdPinData1,		\
			glcdPinData2, glcdPinData3,		\
			glcdPinData4, glcdPinData5,		\
			glcdPinData6, glcdPinData7)


/*
 * Configure the direction of the data pins.
 *	0x00 is for input and 0xff is for output.
 */
#define glcdio_avrDataDir(dirbits)			\
	avrio_Write8Bits(AVRIO_DDRREG, 			\
			glcdPinData0, glcdPinData1,		\
			glcdPinData2, glcdPinData3,		\
			glcdPinData4, glcdPinData5,		\
			glcdPinData6, glcdPinData7, dirbits)


#define glcdio_PinMode(pin, mode)  avrio_PinMode(pin, mode) 
#define glcdio_WritePin(pin, pinval) avrio_WritePin(pin, pinval)
#define glcdio_ReadPin(pin) avrio_ReadPin(pin)
#define glcdio_WriteByte(data) glcdio_avrWriteByte(data)
#define glcdio_ReadByte() glcdio_avrReadByte()
#define	glcdio_DataDirOut() glcdio_avrDataDir(0xff)
#define	glcdio_DataDirIn(_ENpullups)	\
do {									\
		glcdio_avrDataDir(0x00);		\
		if(_ENpullups)					\
			glcdio_avrWriteByte(0xff);	\
}while (0)

#else // ================= non _AVRIO_AVRIO_ below here =============================


// FIXME, switch this #if below to use CORE type for TEENSY3 when its available
#if defined(CORE_TEENSY)  && defined(GLCD_CORE_TEENSY3) // teensy3 core has builtin digitalXXXFast() routines
#define glcdio_ReadPin(pin)				digitalReadFast(pin)
#define glcdio_WritePin(pin, pinval)	digitalWriteFast(pin, pinval)
#define glcdio_PinMode(pin, mode)		pinMode(pin, mode)

#define GLCDIO_DIGITALXXXFAST  // indicate using digitalxxxFast() routines (used by diags)

// if digitalWriteFast and digitalReadFast are defined, use them
#elif defined(digitalWriteFast) && defined(digitalReadFast)
#define glcdio_WritePin(pin, pinval)	digitalWriteFast(pin, pinval)
#define glcdio_ReadPin(pin)				digitalReadFast(pin)

#if defined (pinModeFast) // check for pinModeFast, as not all of them contain this
#define glcdio_PinMode(pin, mode)		pinModeFast(pin, mode)
#else
#define glcdio_PinMode(pin, mode)		pinMode(pin, mode)
#endif

#define GLCDIO_DIGITALXXXFAST  // indicate using digitalxxxFast() routines (used by diags)

// default back to regular Arduino digital core io routines
#else
#define glcdio_ReadPin(pin)				digitalRead(pin)
#define glcdio_WritePin(pin, pinval)	digitalWrite(pin, pinval)
#define glcdio_PinMode(pin, mode)		pinMode(pin, mode)
#endif

/*
 * write byte to the configured LCD data lines 
 * WARNING: this violates the digitalWrite() API in
 * that it does not use HIGH/LOW for the value.
 * All the iplementations so var, seem to only look if
 * if the value is non-zero rather than for HIGH.
 */

#define glcdio_WriteByte(data) 					\
do {											\
	glcdio_WritePin(glcdPinData0, data & _BV(0));	\
	glcdio_WritePin(glcdPinData1, data & _BV(1));	\
	glcdio_WritePin(glcdPinData2, data & _BV(2));	\
	glcdio_WritePin(glcdPinData3, data & _BV(3));	\
	glcdio_WritePin(glcdPinData4, data & _BV(4));	\
	glcdio_WritePin(glcdPinData5, data & _BV(5));	\
	glcdio_WritePin(glcdPinData6, data & _BV(6));	\
	glcdio_WritePin(glcdPinData7, data & _BV(7));	\
} while(0)


/*
 * Read the configured LCD data lines and return data as 8 bit byte
 */
#define glcdio_ReadByte() 								\
({													\
	uint8_t rval = 0;										\
	if(glcdio_ReadPin(glcdPinData0)) rval |= _BV(0);	\
	if(glcdio_ReadPin(glcdPinData1)) rval |= _BV(1);	\
	if(glcdio_ReadPin(glcdPinData2)) rval |= _BV(2);	\
	if(glcdio_ReadPin(glcdPinData3)) rval |= _BV(3);	\
	if(glcdio_ReadPin(glcdPinData4)) rval |= _BV(4);	\
	if(glcdio_ReadPin(glcdPinData5)) rval |= _BV(5);	\
	if(glcdio_ReadPin(glcdPinData6)) rval |= _BV(6);	\
	if(glcdio_ReadPin(glcdPinData7)) rval |= _BV(7);	\
	rval;	/* return value of macro */				\
})


/*
 * Configure the direction of the data pins.
 * If the IDE/core does not support INPUT_PULLUP then
 * you don't get the pullups. It was added in IDE 1.0.2 so most users will
 * be using something newer. It isn't really needed by the glcd device code,
 * It is only used by the initial selftest in Init() and even then,
 * it only comes into play if the user wired up things incorrectly.
 * So its not a big deal if the pullups don't get enabled.
 */
#ifdef INPUT_PULLUP
#define glcdio_DataDirIn(_ENpullups)					\
do {													\
	if(_ENpullups)										\
	{													\
			glcdio_PinMode(glcdPinData0, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData1, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData2, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData3, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData4, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData5, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData6, INPUT_PULLUP);	\
			glcdio_PinMode(glcdPinData7, INPUT_PULLUP);	\
	}													\
	else												\
	{													\
			glcdio_PinMode(glcdPinData0, INPUT);		\
			glcdio_PinMode(glcdPinData1, INPUT);		\
			glcdio_PinMode(glcdPinData2, INPUT);		\
			glcdio_PinMode(glcdPinData3, INPUT);		\
			glcdio_PinMode(glcdPinData4, INPUT);		\
			glcdio_PinMode(glcdPinData5, INPUT);		\
			glcdio_PinMode(glcdPinData6, INPUT);		\
			glcdio_PinMode(glcdPinData7, INPUT);		\
	}													\
}while (0)
#else
#define glcdio_DataDirIn(_ENpullups)		\
do {										\
	glcdio_PinMode(glcdPinData0, INPUT);	\
	glcdio_PinMode(glcdPinData1, INPUT);	\
	glcdio_PinMode(glcdPinData2, INPUT);	\
	glcdio_PinMode(glcdPinData3, INPUT);	\
	glcdio_PinMode(glcdPinData4, INPUT);	\
	glcdio_PinMode(glcdPinData5, INPUT);	\
	glcdio_PinMode(glcdPinData6, INPUT);	\
	glcdio_PinMode(glcdPinData7, INPUT);	\
}while (0)
#endif

#define glcdio_DataDirOut()				\
do {									\
	glcdio_PinMode(glcdPinData0, OUTPUT);	\
	glcdio_PinMode(glcdPinData1, OUTPUT);	\
	glcdio_PinMode(glcdPinData2, OUTPUT);	\
	glcdio_PinMode(glcdPinData3, OUTPUT);	\
	glcdio_PinMode(glcdPinData4, OUTPUT);	\
	glcdio_PinMode(glcdPinData5, OUTPUT);	\
	glcdio_PinMode(glcdPinData6, OUTPUT);	\
	glcdio_PinMode(glcdPinData7, OUTPUT);	\
}while (0)


#endif // ================= End of non AVRIO stuff =============================


/*
 * alias to set RW and DI pins
 */

#define glcdio_SetRWDI(_RWstate, _DIstate)		\
do {									\
		glcdio_WritePin(glcdPinRW, _RWstate);	\
		glcdio_WritePin(glcdPinDI, _DIstate);	\
} while (0)

/*
 * alias to read status bits
 */
#define glcdio_RdBusystatus()		(glcdio_ReadPin(GLCD_STATUS_BIT2PIN(LCD_BUSY_BIT)))
#define glcdio_RdResetstatus()		(glcdio_ReadPin(GLCD_STATUS_BIT2PIN(LCD_RESET_BIT)))

#ifdef glcdPinRES
#define glcdio_Reset()		glcdio_WritePin(glcdPinRES, 0)
#define glcdio_UnReset()	glcdio_WritePin(glcdPinRES, 1)
#else
#define glcdio_Reset()		
#define glcdio_UnReset()		
#endif

/*
 * alias to check status bits
 */

#define glcdio_IsBusyStatus(status) (status & LCD_BUSY_FLAG)
#define glcdio_IsResetStatus(status) (status & LCD_RESET_FLAG)


/*
 * Delay functions
 */

#include "include/glcd_delay.h"

// WARNING, values passed to glcdio_DelayNanoseconds() MUST be constants
#ifdef _delayNanoseconds
#define glcdio_DelayNanoseconds(__ns) _delayNanoseconds(__ns)
#else
/*
 * Fall back to using Arduino delayMicroseconds() function for delays
 * Will be much longer than needed but should allow code to still work.
 */
#warning "glcd_io: using slower Arduino compabitible delay functions"
#define glcdio_DelayNanoseconds(_ns) delayMicroseconds( (unsigned int) (1 + (_ns / 1000.0)))
#endif

#define glcdio_DelayMilliseconds(__ms) delay(__ms)	// Arduino delay function


/*
 * functions to perform chip selects on panel configurations
 * that use them.
 */

#ifdef glcd_CHIP0 // check to see if panel uses chip selects


/*
 * First some sanity checks
 */

#ifndef glcd_CHIP_COUNT
#error glcd_CHIP_COUNT not defined
#endif

/*
 * The actual functions/macros that do the chip selects.
 * each macro is passed a chipselect string defined
 * in the config file. This string has 1, 2, 3, 4, ....
 * pairs of data that define pins that need to be set to
 * a high/low value.
 * These macros rip those strings apart and generate the
 * functions to set the pin values.
 */

#if (glcd_CHIP_COUNT > 9)
#error GLCD chip count beyond max of 9
#endif

#if defined(glcdPinCSEL9)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect9(cselstr)
#elif defined(glcdPinCSEL8)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect8(cselstr)
#elif defined(glcdPinCSEL7)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect7(cselstr)
#elif defined(glcdPinCSEL6)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect6(cselstr)
#elif defined(glcdPinCSEL5)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect5(cselstr)
#elif defined(glcdPinCSEL4)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect4(cselstr)
#elif defined(glcdPinCSEL3)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect3(cselstr)
#elif defined(glcdPinCSEL2)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect2(cselstr)
#elif defined(glcdPinCSEL1)
#define glcdio_ChipSelect(cselstr) glcdio_ChipSelect1(cselstr)
#endif

#define glcdio_ChipSelect1(p,v) glcdio_WritePin(p,v)

#define glcdio_ChipSelect2(p1,v1, p2,v2) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
} while(0)

#define glcdio_ChipSelect3(p1,v1, p2,v2, p3,v3) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
} while(0)

#define glcdio_ChipSelect4(p1,v1, p2,v2, p3,v3, p4,v4) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
	glcdio_ChipSelect1(p4,v4);\
} while(0)

#define glcdio_ChipSelect5(p1,v1, p2,v2, p3,v3, p4,v4, p5,v5) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
	glcdio_ChipSelect1(p4,v4);\
	glcdio_ChipSelect1(p5,v5);\
} while(0)

#define glcdio_ChipSelect6(p1,v1, p2,v2, p3,v3, p4,v4, p5,v5, p6,v6) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
	glcdio_ChipSelect1(p4,v4);\
	glcdio_ChipSelect1(p5,v5);\
	glcdio_ChipSelect1(p6,v6);\
} while(0)

#define glcdio_ChipSelect7(p1,v1, p2,v2, p3,v3, p4,v4, p5,v5, p6,v6, p7,v7) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
	glcdio_ChipSelect1(p4,v4);\
	glcdio_ChipSelect1(p5,v5);\
	glcdio_ChipSelect1(p6,v6);\
	glcdio_ChipSelect1(p7,v7);\
} while(0)

#define glcdio_ChipSelect8(p1,v1, p2,v2, p3,v3, p4,v4, p5,v5, p6,v6, p7,v7, p8,v8) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
	glcdio_ChipSelect1(p4,v4);\
	glcdio_ChipSelect1(p5,v5);\
	glcdio_ChipSelect1(p6,v6);\
	glcdio_ChipSelect1(p7,v7);\
	glcdio_ChipSelect1(p8,v8);\
} while(0)

#define glcdio_ChipSelect9(p1,v1, p2,v2, p3,v3, p4,v4, p5,v5, p6,v6, p7,v7, p8,v8, p9,v9) \
do{	glcdio_ChipSelect1(p1,v1);\
	glcdio_ChipSelect1(p2,v2);\
	glcdio_ChipSelect1(p3,v3);\
	glcdio_ChipSelect1(p4,v4);\
	glcdio_ChipSelect1(p5,v5);\
	glcdio_ChipSelect1(p6,v6);\
	glcdio_ChipSelect1(p7,v7);\
	glcdio_ChipSelect1(p8,v8);\
	glcdio_ChipSelect1(p9,v9);\
} while(0)

#endif // glcd_CHIP0


/*
 * macros for backlight control
 * These work with the glcdPinBL and glcd_BLactlevel defines in the panel files
 * NOTE: do not modify glcdio_BLon() and glcdio_BLoff() below to alter the backlight
 * active level, fix the glcd_BLactlevel in the config file
 */

#define glcdio_BLon() \
 do { glcd_BLactlevel ?  glcdio_WritePin(glcdPinBL, 1) : glcdio_WritePin(glcdPinBL, 0) ; } while(0)

#define glcdio_BLoff() \
 do { glcd_BLactlevel ?  glcdio_WritePin(glcdPinBL, 0) : glcdio_WritePin(glcdPinBL, 1) ; } while(0)

#endif // GLCD_IO_H
