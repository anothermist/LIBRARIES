/*
  glcd_arduino_io.h - openGLCD library support - map arduino pins
  Copyright (c) 2009-2016 Bill Perry

  vi:ts=4
 
 ------------------------------------------------------------------------

  This file is part of the openGLCD library project

  openGLCD is free software: you can redistribute it and/or modify
  it under the terms of version 3 the GNU General Public License
  as published by the Free Software Foundation.

  openGLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with openGLCD.  If not, see <http://www.gnu.org/licenses/>.

 -----------------------------------------------------------------------

  This file contains macros that map arduino pins to avr ports and bits
  Warning: when adding new board/procesor types there are other headers
  that have CORE type dependencies most notabily glcd_io.h
  and a few in the config directory.

*/ 

#ifndef ARDUINO_IO_H
#define ARDUINO_IO_H

#ifdef __cplusplus
extern "C" {
#endif
#include "pins_arduino.h" // comes from Arduino core/variant 
#ifdef __cplusplus
}
#endif

/*
 * Figure which Arduino core we are using
 */

#if defined(GLCDCFG_FORCE_CORECODE) // force core code mode?
#define GLCD_CORE_CORECODE

#elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
#define GLCD_CORE_UNO

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define GLCD_CORE_MEGA

#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284__)  
#ifndef analogInputToDigitalPin // original Sanguino core had no analogInputToDigitalPin()
#define GLCD_CORE_SANGUINO
#else
/*
 * Now must check which mighty1284p core variant
 * Look at the pin number of the first analog pin to distinguish
 */
#if (analogInputToDigitalPin(0) == 24 )
#define GLCD_CORE_MIGHTY1284P // avr mighty1284p "standard" pin mapping
#elif (analogInputToDigitalPin(0) == 31 )
#define GLCD_CORE_SANGUINO // avr mighty1284p avr_developers pin mapping (SANGUINO)
#elif (analogInputToDigitalPin(0) == 14 && analogInputToDigitalPin(7) == 21)
#define GLCD_CORE_BOBUINO // avr mighty1284p "bobuino" pin mapping
#elif (analogInputToDigitalPin(0) == 21 && analogInputToDigitalPin(7) == 31)
#define GLCD_CORE_SBEAUTY // "SleepBeauty" variant mighty-1284p core ------
#else
#warning "Uknown 644/1284 core: Using Arduino Core-Code Mode"
#define GLCD_CORE_CORECODE
#endif
#endif

#elif defined(CORE_TEENSY)
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) // Teensy++ 2.0
#define GLCD_CORE_TEENSY2pp
#elif defined(__AVR_ATmega32U4__)   // Teensy 2.0
#define GLCD_CORE_TEENSY2
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
 // Teensy 3/3.1/LC
#define GLCD_CORE_TEENSY3
#else
// For unknown Teensy boards, Fallback to using Arduino core code routines digitalWrite()/digitalRead()
// instead of faster direct port i/o
#define GLCD_CORE_CORECODE
#warning "Uknown Teensy: Using Arduino Core-Code Mode"
#endif

#elif defined(__AVR_ATmega32U4__)
#define GLCD_CORE_LEONARDO

//#elif defined(__AVR_ATmega32U4__) && USB_VID == 0x2341 && USB_PID == 0x8036
//#define GLCD_CORE_LEONARDO
//#elif defined(__AVR_ATmega32U4__) && USB_VID == 0x2341 && USB_PID == 0x8037
//#define GLCD_CORE_MICRO
//#elif defined(__AVR_ATmega32U4__) && USB_VID == 0x1B4F && USB_PID == 0x8037
//#define GLCD_CORE_LILLYPAD_USB
//#endif

#elif defined(__PIC32MX__) // assume chipkit for mips pic32 processor
#define GLCD_CORE_CHIPKIT
#else

// Fallback to using Arduino core code routines digitalWrite()/digitalRead()
// instead of faster direct port i/o
#define GLCD_CORE_CORECODE
#warning "Using Arduino Core-Code Mode"

#endif


/*
 * define Arduino pin mappings if not already defined
 * These MUST match the 
 *    digital_pin_to_port_PGM
 *    digital_pin_to_bit_mask_PGM
 * tables in the variant file pins_arduino.h that is used
 * for the given core
 */

#if !(defined(digitalPinToPortReg) && defined(digitalPinToBit))

#if defined(GLCD_CORE_UNO) // "standard" variant Arduino Pins ------------------
#define digitalPinToPortReg(P) \
	(\
	(P) == 0 ? &PORTD : \
	(P) == 1 ? &PORTD : \
	(P) == 2 ? &PORTD : \
	(P) == 3 ? &PORTD : \
	(P) == 4 ? &PORTD : \
	(P) == 5 ? &PORTD : \
	(P) == 6 ? &PORTD : \
	(P) == 7 ? &PORTD : \
	(P) == 8 ? &PORTB : \
	(P) == 9 ? &PORTB : \
	(P) == 10 ? &PORTB : \
	(P) == 11 ? &PORTB : \
	(P) == 12 ? &PORTB : \
	(P) == 13 ? &PORTB : \
	(P) == 14 ? &PORTC : \
	(P) == 15 ? &PORTC : \
	(P) == 16 ? &PORTC : \
	(P) == 17 ? &PORTC : \
	(P) == 18 ? &PORTC : \
	(P) == 19 ? &PORTC : \
	&PORTC) // dummy entry (should never happen)

#define digitalPinToBit(P) \
	(\
	(P) == 0 ? 0 : \
	(P) == 1 ? 1 : \
	(P) == 2 ? 2 : \
	(P) == 3 ? 3 : \
	(P) == 4 ? 4 : \
	(P) == 5 ? 5 : \
	(P) == 6 ? 6 : \
	(P) == 7 ? 7 : \
	(P) == 8 ? 0 : \
	(P) == 9 ? 1 : \
	(P) == 10 ? 2 : \
	(P) == 11 ? 3 : \
	(P) == 12 ? 4 : \
	(P) == 13 ? 5 : \
	(P) == 14 ? 0 : \
	(P) == 15 ? 1 : \
	(P) == 16 ? 2 : \
	(P) == 17 ? 3 : \
	(P) == 18 ? 4 : \
	(P) == 19 ? 5 : \
	0) // dummy entry (should never happen)

#elif defined(GLCD_CORE_MEGA) // "mega" variant Arduino Pins ------------------

#define digitalPinToPortReg(P) \
	(\
	(P) == 0 ? &PORTE : \
	(P) == 1 ? &PORTE : \
	(P) == 2 ? &PORTE : \
	(P) == 3 ? &PORTE : \
	(P) == 4 ? &PORTG : \
	(P) == 5 ? &PORTE : \
	(P) == 6 ? &PORTH : \
	(P) == 7 ? &PORTH : \
	(P) == 8 ? &PORTH : \
	(P) == 9 ? &PORTH : \
	(P) == 10 ? &PORTB : \
	(P) == 11 ? &PORTB : \
	(P) == 12 ? &PORTB : \
	(P) == 13 ? &PORTB : \
	(P) == 14 ? &PORTJ : \
	(P) == 15 ? &PORTJ : \
	(P) == 16 ? &PORTH : \
	(P) == 17 ? &PORTH : \
	(P) == 18 ? &PORTD : \
	(P) == 19 ? &PORTD : \
	(P) == 20 ? &PORTD : \
	(P) == 21 ? &PORTD : \
	(P) == 22 ? &PORTA : \
	(P) == 23 ? &PORTA : \
	(P) == 24 ? &PORTA : \
	(P) == 25 ? &PORTA : \
	(P) == 26 ? &PORTA : \
	(P) == 27 ? &PORTA : \
	(P) == 28 ? &PORTA : \
	(P) == 29 ? &PORTA : \
	(P) == 30 ? &PORTC : \
	(P) == 31 ? &PORTC : \
	(P) == 32 ? &PORTC : \
	(P) == 33 ? &PORTC : \
	(P) == 34 ? &PORTC : \
	(P) == 35 ? &PORTC : \
	(P) == 36 ? &PORTC : \
	(P) == 37 ? &PORTC : \
	(P) == 38 ? &PORTD : \
	(P) == 39 ? &PORTG : \
	(P) == 40 ? &PORTG : \
	(P) == 41 ? &PORTG : \
	(P) == 42 ? &PORTL : \
	(P) == 43 ? &PORTL : \
	(P) == 44 ? &PORTL : \
	(P) == 45 ? &PORTL : \
	(P) == 46 ? &PORTL : \
	(P) == 47 ? &PORTL : \
	(P) == 48 ? &PORTL : \
	(P) == 49 ? &PORTL : \
	(P) == 50 ? &PORTB : \
	(P) == 51 ? &PORTB : \
	(P) == 52 ? &PORTB : \
	(P) == 53 ? &PORTB : \
	(P) == 54 ? &PORTF : \
	(P) == 55 ? &PORTF : \
	(P) == 56 ? &PORTF : \
	(P) == 57 ? &PORTF : \
	(P) == 58 ? &PORTF : \
	(P) == 59 ? &PORTF : \
	(P) == 60 ? &PORTF : \
	(P) == 61 ? &PORTF : \
	(P) == 62 ? &PORTK : \
	(P) == 63 ? &PORTK : \
	(P) == 64 ? &PORTK : \
	(P) == 65 ? &PORTK : \
	(P) == 66 ? &PORTK : \
	(P) == 67 ? &PORTK : \
	(P) == 68 ? &PORTK : \
	(P) == 69 ? &PORTK : \
	&PORTC) // dummy entry (should never happen)

#define digitalPinToBit(P) \
	(\
	(P) == 0 ? 0 : \
	(P) == 1 ? 1 : \
	(P) == 2 ? 4 : \
	(P) == 3 ? 5 : \
	(P) == 4 ? 5 : \
	(P) == 5 ? 3 : \
	(P) == 6 ? 3 : \
	(P) == 7 ? 4 : \
	(P) == 8 ? 5 : \
	(P) == 9 ? 6 : \
	(P) == 10 ? 4 : \
	(P) == 11 ? 5 : \
	(P) == 12 ? 6 : \
	(P) == 13 ? 7 : \
	(P) == 14 ? 1 : \
	(P) == 15 ? 0 : \
	(P) == 16 ? 1 : \
	(P) == 17 ? 0 : \
	(P) == 18 ? 3 : \
	(P) == 19 ? 2 : \
	(P) == 20 ? 1 : \
	(P) == 21 ? 0 : \
	(P) == 22 ? 0 : \
	(P) == 23 ? 1 : \
	(P) == 24 ? 2 : \
	(P) == 25 ? 3 : \
	(P) == 26 ? 4 : \
	(P) == 27 ? 5 : \
	(P) == 28 ? 6 : \
	(P) == 29 ? 7 : \
	(P) == 30 ? 7 : \
	(P) == 31 ? 6 : \
	(P) == 32 ? 5 : \
	(P) == 33 ? 4 : \
	(P) == 34 ? 3 : \
	(P) == 35 ? 2 : \
	(P) == 36 ? 1 : \
	(P) == 36 ? 0 : \
	(P) == 38 ? 7 : \
	(P) == 39 ? 2 : \
	(P) == 40 ? 1 : \
	(P) == 41 ? 0 : \
	(P) == 42 ? 7 : \
	(P) == 43 ? 6 : \
	(P) == 44 ? 5 : \
	(P) == 45 ? 4 : \
	(P) == 46 ? 3 : \
	(P) == 47 ? 2 : \
	(P) == 48 ? 1 : \
	(P) == 49 ? 0 : \
	(P) == 50 ? 3 : \
	(P) == 51 ? 2 : \
	(P) == 52 ? 1 : \
	(P) == 53 ? 0 : \
	(P) == 54 ? 0 : \
	(P) == 55 ? 1 : \
	(P) == 56 ? 2 : \
	(P) == 57 ? 3 : \
	(P) == 58 ? 4 : \
	(P) == 59 ? 5 : \
	(P) == 60 ? 6 : \
	(P) == 61 ? 7 : \
	(P) == 62 ? 0 : \
	(P) == 63 ? 1 : \
	(P) == 64 ? 2 : \
	(P) == 65 ? 3 : \
	(P) == 66 ? 4 : \
	(P) == 67 ? 5 : \
	(P) == 68 ? 6 : \
	(P) == 69 ? 7 : \
	0) // dummy entry (should never happen)

// check for using Jack's mighty-1284p vendor core
// Jack's mighty-1284p vendor core has a PORT_TO_OUTPUT macro
// that can be used to get the address of the port register
#elif defined(PORT_TO_OUTPUT)
#warning using new 1284 pasting macros
// The reasn there is a sub macro here is that 
// macro and if the parameter to digitalPinToPortReg() is not
// a naked constant, then pasting cannot be used to look upt the port index define name
// Code that depends on compile time optimazation vs cpp time optimization may do this.
// Having the extra macro can use the cpp pasting to create a giagantic ternary
// that will still work at compile time when constants but not naked constants are used.
// This allows using things like the A0 arduino symbol which is a const int 
// rather than a simple naked numeric define
#define _digitalPinToPortReg(p) PORT_TO_OUTPUT(PORT_D ## p)
#define digitalPinToPortReg(P) \
	(\
		(P) ==  0 ? _digitalPinToPortReg(0)  : \
		(P) ==  1 ? _digitalPinToPortReg(1)  : \
		(P) ==  2 ? _digitalPinToPortReg(2)  : \
		(P) ==  3 ? _digitalPinToPortReg(3)  : \
		(P) ==  4 ? _digitalPinToPortReg(4)  : \
		(P) ==  5 ? _digitalPinToPortReg(5)  : \
		(P) ==  6 ? _digitalPinToPortReg(6)  : \
		(P) ==  7 ? _digitalPinToPortReg(7)  : \
		(P) ==  8 ? _digitalPinToPortReg(8)  : \
		(P) ==  9 ? _digitalPinToPortReg(9)  : \
		(P) == 10 ? _digitalPinToPortReg(10) : \
		(P) == 11 ? _digitalPinToPortReg(11) : \
		(P) == 12 ? _digitalPinToPortReg(12) : \
		(P) == 13 ? _digitalPinToPortReg(13) : \
		(P) == 14 ? _digitalPinToPortReg(14) : \
		(P) == 15 ? _digitalPinToPortReg(15) : \
		(P) == 16 ? _digitalPinToPortReg(16) : \
		(P) == 17 ? _digitalPinToPortReg(17) : \
		(P) == 18 ? _digitalPinToPortReg(18) : \
		(P) == 19 ? _digitalPinToPortReg(19) : \
		(P) == 20 ? _digitalPinToPortReg(20) : \
		(P) == 21 ? _digitalPinToPortReg(21) : \
		(P) == 22 ? _digitalPinToPortReg(22) : \
		(P) == 23 ? _digitalPinToPortReg(23) : \
		(P) == 24 ? _digitalPinToPortReg(24) : \
		(P) == 25 ? _digitalPinToPortReg(25) : \
		(P) == 26 ? _digitalPinToPortReg(26) : \
		(P) == 27 ? _digitalPinToPortReg(27) : \
		(P) == 28 ? _digitalPinToPortReg(28) : \
		(P) == 29 ? _digitalPinToPortReg(29) : \
		(P) == 30 ? _digitalPinToPortReg(30) : \
		(P) == 31 ? _digitalPinToPortReg(31) : \
	&PORTA) // dummy entry (should never happen)

#define _digitalPinToBit(p) (BIT_D ## p)
#define digitalPinToBit(P) \
	(\
		(P) ==  0 ? _digitalPinToBit(0)  : \
		(P) ==  1 ? _digitalPinToBit(1)  : \
		(P) ==  2 ? _digitalPinToBit(2)  : \
		(P) ==  3 ? _digitalPinToBit(3)  : \
		(P) ==  4 ? _digitalPinToBit(4)  : \
		(P) ==  5 ? _digitalPinToBit(5)  : \
		(P) ==  6 ? _digitalPinToBit(6)  : \
		(P) ==  7 ? _digitalPinToBit(7)  : \
		(P) ==  8 ? _digitalPinToBit(8)  : \
		(P) ==  9 ? _digitalPinToBit(9)  : \
		(P) == 10 ? _digitalPinToBit(10) : \
		(P) == 11 ? _digitalPinToBit(11) : \
		(P) == 12 ? _digitalPinToBit(12) : \
		(P) == 13 ? _digitalPinToBit(13) : \
		(P) == 14 ? _digitalPinToBit(14) : \
		(P) == 15 ? _digitalPinToBit(15) : \
		(P) == 16 ? _digitalPinToBit(16) : \
		(P) == 17 ? _digitalPinToBit(17) : \
		(P) == 18 ? _digitalPinToBit(18) : \
		(P) == 19 ? _digitalPinToBit(19) : \
		(P) == 20 ? _digitalPinToBit(20) : \
		(P) == 21 ? _digitalPinToBit(21) : \
		(P) == 22 ? _digitalPinToBit(22) : \
		(P) == 23 ? _digitalPinToBit(23) : \
		(P) == 24 ? _digitalPinToBit(24) : \
		(P) == 25 ? _digitalPinToBit(25) : \
		(P) == 26 ? _digitalPinToBit(26) : \
		(P) == 27 ? _digitalPinToBit(27) : \
		(P) == 28 ? _digitalPinToBit(28) : \
		(P) == 29 ? _digitalPinToBit(29) : \
		(P) == 30 ? _digitalPinToBit(30) : \
		(P) == 31 ? _digitalPinToBit(31) : \
	0) // dummy entry (should never happen)

// If not using Jack's core, try to fall back to
// seperate mapping tables.	
#elif defined(GLCD_CORE_MIGHTY1284P) // "standard" variant mighty-1284p core ----

#define digitalPinToPortReg(P) \
    (((P) >= 0 && (P) <= 7)   ? &PORTB : \
	(((P) >= 8 && (P) <= 15)  ? &PORTD : \
 	(((P) >= 16 && (P) <= 23) ? &PORTC : &PORTA)))
#define digitalPinToBit(P) (P%8)

#elif defined(GLCD_CORE_SANGUINO) // "Sanguino"/avr_developers variant mighty-1284p core
#define digitalPinToPortReg(P) \
    (((P) >= 0 && (P) <= 7)   ? &PORTB : \
	(((P) >= 8 && (P) <= 15)  ? &PORTD : \
 	(((P) >= 16 && (P) <= 23) ? &PORTC : &PORTA)))
#define digitalPinToBit(P) \
     (((P) >= 0 && (P) <= 23) ? (P%8) : (7-(P%8)) )

#elif defined(GLCD_CORE_BOBUINO) // "bobuino" variant mighty-1284p core ------
#define digitalPinToPortReg(P) \
	(\
	(P) == 0 ? &PORTD :\
	(P) == 1 ? &PORTD :\
	(P) == 2 ? &PORTD :\
	(P) == 3 ? &PORTD :\
	(P) == 4 ? &PORTB :\
	(P) == 5 ? &PORTB :\
	(P) == 6 ? &PORTB :\
	(P) == 7 ? &PORTB :\
	(P) == 8 ? &PORTD :\
	(P) == 9 ? &PORTD :\
	(P) == 10 ? &PORTB :\
	(P) == 11 ? &PORTB :\
	(P) == 12 ? &PORTB :\
	(P) == 13 ? &PORTB :\
	(P) == 14 ? &PORTA :\
	(P) == 15 ? &PORTA :\
	(P) == 16 ? &PORTA :\
	(P) == 17 ? &PORTA :\
	(P) == 18 ? &PORTA :\
	(P) == 19 ? &PORTA :\
	(P) == 20 ? &PORTA :\
	(P) == 21 ? &PORTA :\
	(P) == 22 ? &PORTC :\
	(P) == 23 ? &PORTC :\
	(P) == 24 ? &PORTC :\
	(P) == 25 ? &PORTC :\
	(P) == 26 ? &PORTC :\
	(P) == 27 ? &PORTC :\
	(P) == 28 ? &PORTC :\
	(P) == 29 ? &PORTC :\
	(P) == 30 ? &PORTD :\
	(P) == 31 ? &PORTD :\
	&PORTA) // dummy entry (should never happen)
#define digitalPinToBit(P) \
	(\
	(P) == 0 ? 0 :\
	(P) == 1 ? 1 :\
	(P) == 2 ? 2 :\
	(P) == 3 ? 3 :\
	(P) == 4 ? 0 :\
	(P) == 5 ? 1 :\
	(P) == 6 ? 2 :\
	(P) == 7 ? 3 :\
	(P) == 8 ? 5 :\
	(P) == 9 ? 6 :\
	(P) == 10 ? 4 :\
	(P) == 11 ? 5 :\
	(P) == 12 ? 6 :\
	(P) == 13 ? 7 :\
	(P) == 14 ? 7 :\
	(P) == 15 ? 6 :\
	(P) == 16 ? 5 :\
	(P) == 17 ? 4 :\
	(P) == 18 ? 3 :\
	(P) == 19 ? 2 :\
	(P) == 20 ? 1 :\
	(P) == 21 ? 0 :\
	(P) == 22 ? 0 :\
	(P) == 23 ? 1 :\
	(P) == 24 ? 2 :\
	(P) == 25 ? 3 :\
	(P) == 26 ? 4 :\
	(P) == 27 ? 5 :\
	(P) == 28 ? 6 :\
	(P) == 29 ? 7 :\
	(P) == 30 ? 4 :\
	(P) == 31 ? 7 :\
	0) // dummy entry (should never happen)

#elif defined(GLCD_CORE_SBEAUTY) // "SleepBeauty" variant mighty-1284p core ------
#define digitalPinToPortReg(P) \
	(\
	(P) == 0 ? &PORTD :\
	(P) == 1 ? &PORTD :\
	(P) == 2 ? &PORTD :\
	(P) == 3 ? &PORTD :\
	(P) == 4 ? &PORTB :\
	(P) == 5 ? &PORTB :\
	(P) == 6 ? &PORTB :\
	(P) == 7 ? &PORTB :\
	(P) == 8 ? &PORTD :\
	(P) == 9 ? &PORTD :\
	(P) == 10 ? &PORTB :\
	(P) == 11 ? &PORTB :\
	(P) == 12 ? &PORTB :\
	(P) == 13 ? &PORTB :\
	(P) == 14 ? &PORTC :\
	(P) == 15 ? &PORTC :\
	(P) == 16 ? &PORTA :\
	(P) == 17 ? &PORTA :\
	(P) == 18 ? &PORTA :\
	(P) == 19 ? &PORTA :\
	(P) == 20 ? &PORTA :\
	(P) == 21 ? &PORTA :\
	(P) == 22 ? &PORTD :\
	(P) == 23 ? &PORTD :\
	(P) == 24 ? &PORTC :\
	(P) == 25 ? &PORTC :\
	(P) == 26 ? &PORTC :\
	(P) == 27 ? &PORTC :\
	(P) == 28 ? &PORTC :\
	(P) == 29 ? &PORTC :\
	(P) == 30 ? &PORTA :\
	(P) == 31 ? &PORTA :\
	&PORTA) // dummy entry (should never happen)
#define digitalPinToBit(P) \
	(\
	(P) == 0 ? 0 :\
	(P) == 1 ? 1 :\
	(P) == 2 ? 2 :\
	(P) == 3 ? 3 :\
	(P) == 4 ? 0 :\
	(P) == 5 ? 1 :\
	(P) == 6 ? 2 :\
	(P) == 7 ? 3 :\
	(P) == 8 ? 6 :\
	(P) == 9 ? 5 :\
	(P) == 10 ? 4 :\
	(P) == 11 ? 5 :\
	(P) == 12 ? 6 :\
	(P) == 13 ? 7 :\
	(P) == 14 ? 7 :\
	(P) == 15 ? 6 :\
	(P) == 16 ? 5 :\
	(P) == 17 ? 4 :\
	(P) == 18 ? 3 :\
	(P) == 19 ? 2 :\
	(P) == 20 ? 1 :\
	(P) == 21 ? 0 :\
	(P) == 22 ? 4 :\
	(P) == 23 ? 7 :\
	(P) == 24 ? 2 :\
	(P) == 25 ? 3 :\
	(P) == 26 ? 4 :\
	(P) == 27 ? 5 :\
	(P) == 28 ? 1 :\
	(P) == 29 ? 0 :\
	(P) == 30 ? 6 :\
	(P) == 31 ? 7 :\
	0) // dummy entry (should never happen)

#elif defined(GLCD_CORE_LEONARDO) // "leonardo" variant Arduino Pins -----------
#define digitalPinToPortReg(P) \
	(\
	(P) == 0 ? &PORTD : \
	(P) == 1 ? &PORTD : \
	(P) == 2 ? &PORTD : \
	(P) == 3 ? &PORTD : \
	(P) == 4 ? &PORTD : \
	(P) == 5 ? &PORTC : \
	(P) == 6 ? &PORTD : \
	(P) == 7 ? &PORTE : \
	(P) == 8 ? &PORTB : \
	(P) == 9 ? &PORTB : \
	(P) == 10 ? &PORTB : \
	(P) == 11 ? &PORTB : \
	(P) == 12 ? &PORTD : \
	(P) == 13 ? &PORTC : \
	(P) == 14 ? &PORTB : \
	(P) == 15 ? &PORTB : \
	(P) == 16 ? &PORTB : \
	(P) == 17 ? &PORTB : \
	(P) == 18 ? &PORTF : \
	(P) == 19 ? &PORTF : \
	(P) == 20 ? &PORTF : \
	(P) == 21 ? &PORTF : \
	(P) == 22 ? &PORTF : \
	(P) == 23 ? &PORTF : \
	(P) == 24 ? &PORTD : \
	(P) == 25 ? &PORTD : \
	(P) == 26 ? &PORTB : \
	(P) == 27 ? &PORTB : \
	(P) == 28 ? &PORTB : \
	(P) == 29 ? &PORTD : \
	&PORTD) // dummy entry (should never happen)

#define digitalPinToBit(P) \
	(\
	(P) == 0 ? 2 : \
	(P) == 1 ? 3 : \
	(P) == 2 ? 1 : \
	(P) == 3 ? 0 : \
	(P) == 4 ? 4 : \
	(P) == 5 ? 6 : \
	(P) == 6 ? 7 : \
	(P) == 7 ? 6 : \
	(P) == 8 ? 4 : \
	(P) == 9 ? 5 : \
	(P) == 10 ? 6 : \
	(P) == 11 ? 7 : \
	(P) == 12 ? 6 : \
	(P) == 13 ? 7 : \
	(P) == 14 ? 3 : \
	(P) == 15 ? 1 : \
	(P) == 16 ? 2 : \
	(P) == 17 ? 0 : \
	(P) == 18 ? 7 : \
	(P) == 19 ? 6 : \
	(P) == 20 ? 5 : \
	(P) == 21 ? 4 : \
	(P) == 22 ? 1 : \
	(P) == 23 ? 0 : \
	(P) == 24 ? 4 : \
	(P) == 25 ? 7 : \
	(P) == 26 ? 4 : \
	(P) == 27 ? 5 : \
	(P) == 28 ? 6 : \
	(P) == 29 ? 6 : \
	0) // dummy entry (should never happen)

#endif

#endif

#endif // ARDUINO_IO_H
