/*
 * ks0108_Uno.h - User specific configuration for openGLCD library
 *
 * Use this file to set io pins
 * when using an Uno, 2009, or any AVR m168/m328 based board
 *
 */

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Uno"

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
#define glcdPinData4        4
#define glcdPinData5        5
#define glcdPinData6        6
#define glcdPinData7        7

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
#define glcdPinEN           A4 // (A4 is also I2C SDA)
// Reset  - uncomment the next line if glcd module reset is connected to an Arduino pin
//#define glcdPinRES          A5    // optional s/w Reset control (A5 is also I2C SCL)

//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

#endif //GLCD_PIN_CONFIG_H
