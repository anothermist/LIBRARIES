/*
 * ks0108_Bobuino.h - configuration for openGLCD library
 *
 * Use this file to set io pins
 * when using the "bobuino" variant with the Mighty1284p core
 *
 * https://github.com/maniacbug/mighty-1284p
 * http://maniacbug.wordpress.com/2011/11/27/arduino-on-atmega1284p-4/
 *
 * This version is for a standard ks0108 display
 * connected using the bobduino pin mappings.
 * The pins have been picked to be pin/shield compatible with
 * "Uno" form factor boards like Uno,2009, Leonardo
 * rather than for performance.
 *
 * This allows moving connections from a working Uno or Leonardo
 * board to a bobuino.
 * 
 * 
 */

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Bobuino"

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins can also be assigned using AVRPIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *    Example: port D bit 3 is AVRPIN_D3
 *
 * Pins can also be assigned using 0xPb for ports A to F
 *     Example: port D bit 3 is 0xD3
 */

/* Data pin definitions
 */
#define glcdPinData0        8
#define glcdPinData1        9
#define glcdPinData2        10
#define glcdPinData3        11

#define glcdPinData4        4 // for nibble mode needs to be A3
#define glcdPinData5        5 // for nibble mode needs to be A2
#define glcdPinData6        6 // for nibble mode needs to be A1
#define glcdPinData7        7 // for nibble mode needs to be A0

/* Arduino pins used for Control
 * default assignment uses the first five analog pins
 */

#define glcdPinCSEL1        A0
#define glcdPinCSEL2        A1

#if NBR_CHIP_SELECT_PINS > 2
#define glcdPinCSEL3         3   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdPinCSEL4         2   // fourth chip select if needed
#endif

#define glcdPinRW           A2
#define glcdPinDI           A3
#define glcdPinEN           A4
// Reset - uncomment the next line if glcd module reset is connected to an output pin
//#define glcdPinRES          A5    // Reset control

//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

#endif //GLCD_PIN_CONFIG_H
