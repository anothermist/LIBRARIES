/*
 * ks0108_Mega.h - User specific configuration for openGLCD library
 *
 * Use this file to set io pins
 * when using an Arduino Mega1280 or Mega2560 board
 *
 * The configuration below uses a single port for data 
 *
 */

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Mega"

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
 * This version uses pins 22-29 for LCD Data 
 */
#define glcdPinData0    22
#define glcdPinData1    23
#define glcdPinData2    24
#define glcdPinData3    25
#define glcdPinData4    26
#define glcdPinData5    27
#define glcdPinData6    28
#define glcdPinData7    29

/*
 * Pins used for Control
 */
#define glcdPinCSEL1     33    // CS1 Bit  
#define glcdPinCSEL2     34    // CS2 Bit
#define glcdPinRW        35    // R/W Bit
#define glcdPinDI        36    // D/I Bit 
#define glcdPinEN        37    // EN Bit

#if NBR_CHIP_SELECT_PINS > 2
#define glcdPinCSEL3     32   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdPinCSEL4     31   // fourth chip select if needed
#endif

// Reset - uncomment the next line if glcd module reset is connected to an Arduino pin
//#define glcdPinRES         30    // optional s/w Reset control

//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

#endif //GLCD_PIN_CONFIG_H
