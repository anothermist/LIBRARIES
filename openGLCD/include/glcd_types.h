/*
  glcd_types.h - openGLCD library support
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

 ------------------------------------------------------------------------

  This file creates types used by openGLCD to ensure that the
  mainline code NEVER uses any architecture or tool specific types.
 
 */

#ifndef	GLCD_TYPES_H
#define GLCD_TYPES_H

/*
 * Include core definitions
 */
#if ARDUINO < 100
#if ARDUINO < 19
#error openGLCD requires Arduino 0019 or later
#endif
#include "WProgram.h"
#else
#include "Arduino.h"
#endif


/*
 * Map a bit to a bit mask
 */
#ifndef _BV
#define _BV(_bit) (1 << _bit)
#endif

#if defined (__AVR__)
// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
// to prevent: warning: only initialized variables can be placed into program memory area
// In order to use this on AVR, ALL progmem variables must be declared the same with respect
// to const since they will land in the same section. So all progmem variables should use const.

#ifdef PROGMEM
//#undef PROGMEM
//#define PROGMEM __attribute__((section(".progmem.data")))
#endif
#define PROGMEMGLCD __attribute__((section(".progmem.openGLCD")))

#else // non AVR
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".rodata"))) // put into read-only data section so it doesn't end up in RAM
#endif

#endif

#if __AVR__
#define GLCDFONTDECL(_n) const uint8_t PROGMEMGLCD _n[]
#define GLCDBMAPDECL(_n) const uint8_t PROGMEMGLCD _n[]
#define GLCDXBMAPDECL(_n) const uint8_t PROGMEMGLCD _n[]
#else
#define GLCDFONTDECL(_n) const uint8_t _n[]
#define GLCDBMAPDECL(_n) const uint8_t _n[]
#define GLCDXBMAPDECL(_n) const uint8_t _n[]
#endif

/*
 * Kludge macros for missing stuff in ChipKit 
 */

#if defined(__PIC32MX__)
#ifndef PGM_P
#define PGM_P const char *
#endif
#ifndef PSTR
#define 	PSTR(s)   ((const char *)(s))
#endif
#endif // __PIC32MX__

#endif // GLCD_TYPES_H
