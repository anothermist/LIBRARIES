/*
 * glcd_delay.h -  openGLCD library support- provides accurate delays
 * Copyright (c) 2011-2016 Bill Perry
 *
 * vi:ts=4
 *
 * ------------------------------------------------------------------------
 *
 *  This file is part of the openGLCD library project
 *
 *  openGLCD is free software: you can redistribute it and/or modify
 *  it under the terms of version 3 of the GNU General Public License
 *  as published by the Free Software Foundation.
 *
 *  openGLCD is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with openGLCD.  If not, see <http://www.gnu.org/licenses/>.
 *
 * -----------------------------------------------------------------------
 *
 *  Provides accurate delays as short as possible based
 *  on requesting a delay for a given number of nanoseconds
 *  Delay has an accuracy as accurate as a single CPU clock
 * (the macro F_CPU must be defined to a constant defining the 
 * CPU clock frequency in Hertz).
 *
 */
 
#ifndef _GLCD_DELAY_H_
#define _GLCD_DELAY_H_

#include <inttypes.h>

#ifndef F_CPU
#error "Macro F_CPU must be defined"
#endif

#if defined(__AVR__)
#include "delay_cycles_avr.h"
#define _delayNanoseconds(__ns)     _delay_cycles( (double)(F_CPU)*((double)__ns)/1.0e9 + 0.5 )

#elif defined(__arm__) // for ARM products
#include "delay_cycles_arm.h"
#define _delayNanoseconds(__ns)     _delay_cycles( (double)(F_CPU)*((double)__ns)/1.0e9 + 0.5 )

#elif defined(__PIC32MX__) // for pic32 (chipKIT)
#include "delay_cycles_pic32.h"
#define _delayNanoseconds(__ns)     _delay_cycles( (double)(F_CPU/2)*((double)__ns)/1.0e9 + 0.5 )

#else
#warning "No delay_cycle support for MCU"
#endif


#endif /* _GLCD_DELAY_H_ */
