/*
 * AutoPinConfig_ks0108.h - automatically select pin config file for board type
 */

#ifndef GLCD_AUTOPINCONFIG_H
#define GLCD_AUTOPINCONFIG_H

 /*
  * The sequence below selects a configuration file for pin assignment based on
  * the board selected in the IDE. These configurations are compatible with
  * wiring used in earlier versions of the library
  * WARNING: When adding new board types it is not as simple as just editing
  * these lines.
  * There is also a dependency on the file openGLCD/include/glcdio_arduino_io.h
  * which does the actual arduino pin mappings
  */

#if defined(GLCD_CORE_UNO)
#include "PinConfig_ks0108-Uno.h"	// config file for standard Arduino Uno/2009/nano etc..m328 boards
#elif defined(GLCD_CORE_MEGA)
#include "PinConfig_ks0108-Mega.h"      // config for mega 1280/2560 board
#elif defined(GLCD_CORE_LEONARDO)
#include "PinConfig_ks0108-Leonardo.h"	// config for Leonardo/micro
#elif defined(CORE_TEENSY) && !defined(GLCD_CORE_CORECODE)
#include "PinConfig_ks0108-Teensy.h"	// config for Teensy, Teensy++  and Teensy3
#elif defined(GLCD_CORE_MIGHTY1284P)
#include "PinConfig_ks0108-Mighty1284p.h" // config for "standard" mighty1284p core
#elif defined(GLCD_CORE_SANGUINO)
#include "PinConfig_ks0108-Sanguino.h"	// config for Sanguino ATmega644/p or avr_dev mighty1284p core
#elif defined(GLCD_CORE_BOBUINO)
#include "PinConfig_ks0108-Bobuino.h"	// config for bobuino with mighty1284p core
#elif defined(GLCD_CORE_SBEAUTY)
#include "PinConfig_ks0108-SBeauty.h"	// config for SleepingBeauty with mighty1284p core
#elif defined(GLCD_CORE_CHIPKIT)
#include "PinConfig_ks0108-chipKIT.h"	// config for chipKIT
#else

// Issue warning if user isn't forcing CORECODE mode
#ifndef GLCDCFG_FORCE_CORECODE
#warning using Arduino CoreCode pin config file
#endif
#include "PinConfig_ks0108-CoreCode.h"
#endif

#endif // GLCD_AUTOPINCONFIG_H
