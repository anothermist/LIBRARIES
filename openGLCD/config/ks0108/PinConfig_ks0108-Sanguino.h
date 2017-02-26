/*
 * ks0108_Sanguino.h - ATmega644/1284 specific configuration for openGLCD library
 *
 * Use this file to set io pins
 * when using the original Sanguino core or
 * when using the "avr_developers" variant with the Mighty1284p core
 *
 * https://github.com/maniacbug/mighty-1284p
 * http://maniacbug.wordpress.com/2011/11/27/arduino-on-atmega1284p-4/
 *
 * The configuration below uses a single port for data 
 *
*/

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Sanguino"

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

/*
 * Data pin definitions
 * This version uses pins 0-7 for LCD Data 
 */
#define glcdPinData0    0
#define glcdPinData1    1
#define glcdPinData2    2
#define glcdPinData3    3
#define glcdPinData4    4
#define glcdPinData5    5
#define glcdPinData6    6
#define glcdPinData7    7

/*
 * pins used for control
 */

#define glcdPinCSEL1    24     // CS1 Bit   // swap pin assignments with CSEL2 if left/right image is reversed
#define glcdPinCSEL2    25     // CS2 Bit
#define glcdPinRW       26     // R/W Bit
#define glcdPinDI       27     // D/I Bit 
#define glcdPinEN       28     // EN Bit

#if NBR_CHIP_SELECT_PINS > 2
#define glcdPinCSEL3    29   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdPinCSEL4    30   // fourth chip select if needed
#endif

// Reset - uncomment the next line if glcd module reset is connected to an Arduino pin
//#define glcdPinRES         XX    // optional s/w Reset control

//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

#endif //GLCD_PANEL_CONFIG_H
