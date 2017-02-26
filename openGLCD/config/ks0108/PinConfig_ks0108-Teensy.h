/*
 * ks0108_Teensy.h - User specific configuration for openGLCD library
 *
 * Use this file to set io pins
 * when using a Teensy, Teensy++, or Teensy3
 *
 * Note that each board uses different pin numbers 
 * so make sure that your wiring matches the device you are using
 *
 */

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/***********************************************************
 *  Config for LCD pins to Teensy 3 boards                 *
 * __MK20DX128__ for Teensy 3.0                            *
 * __MK20DX256__ for Teensy 3.1                            *
 * __MKL26Z64__  for Teensy LC                             *
 * __MK64FX512__ for Teensy 3.4                            *
 * __MK66FX1M0__ for Teensy 3.5                            *
 ***********************************************************/
#if defined(__MK20DX128__)   || defined(__MK20DX256__) || defined(__MKL26Z64__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

#define glcd_PinConfigName "ks0108-Teensy3"   // define name for configuration

// "default" Teensy 3 connections
// used on GLCD demo board(s) for Bill Perry

#define glcdPinData0      16
#define glcdPinData1      17
#define glcdPinData2      18 // i2c SDA0
#define glcdPinData3      19 // i2c SCL0
#define glcdPinData4      20
#define glcdPinData5      21
#define glcdPinData6      22
#define glcdPinData7      23


#define glcdPinDI          9
#define glcdPinRW          8
#define glcdPinEN          7
#define glcdPinCSEL1       3
#define glcdPinCSEL2       2
#if NBR_CHIP_SELECT_PINS > 2
#error Missing glcdPinCSEL3 define
//#define glcdPinCSEL3    ?   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#error Missing glcdPinCSEL4 define
//#define glcdPinCSEL4    ?   // fourth chip select if needed
#endif

//#define glcdPinBL          ?      //  optional backlight control pin controls BL circuit
//#define glcdPinRES         ?      //  optional s/w reset control

//#warning "KS0108 using pins for Teensy 3"


/**********************************************************
 *  Config for LCD pins to Teensy++ Pins                  *
 **********************************************************/
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) // Teensy++ 2.0

/*
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins on AVR devices can also be assigned using AVRPIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *    Example: port D bit 3 is AVRPIN_D3
 *
 * Pins on AVR devices can also be assigned using 0xPb for ports A to F
 *     Example: port D bit 3 is 0xD3
 */

#define glcd_PinConfigName "ks0108-Teensy++"   // define name for configuration
/*
 *                Arduino pin    AVR pin
 */ 
#define glcdPinData0    10      // AVRPIN_C0
#define glcdPinData1    11      // AVRPIN_C1
#define glcdPinData2    12      // AVRPIN_C2
#define glcdPinData3    13      // AVRPIN_C3
#define glcdPinData4    14      // AVRPIN_C4
#define glcdPinData5    15      // AVRPIN_C5
#define glcdPinData6    16      // AVRPIN_C6
#define glcdPinData7    17      // AVRPIN_C7

#define glcdPinDI       9       // AVRPIN_E1
#define glcdPinRW       8       // AVRPIN_E0
#define glcdPinEN       7       // AVRPIN_D7
#define glcdPinCSEL1    18      // AVRPIN_E6 
#define glcdPinCSEL2    19      // AVRPIN_E7

//#define glcdPinBL       27    // AVRPIN_B7     // optional backlight control pin controls BL circuit
//#define glcdPinRES      5       // AVRPIN_D5     // optional s/w reset control

#if NBR_CHIP_SELECT_PINS > 2
#error Missing glcdPinCSEL3 define
//#define glcdPinCSEL3    ?   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#error Missing glcdPinCSEL4 define
//#define glcdPinCSEL4    ?   // fourth chip select if needed
#endif


//#warning "KS0108 using pins for teensy++"

/*********************************************************
 *  Configuration for assigning LCD pins to Teensy Pins  *
 *********************************************************/
#elif defined(__AVR_ATmega32U4__)   // Teensy 2.0

/*
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins on AVR devices can also be assigned using AVRPIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *    Example: port D bit 3 is AVRPIN_D3
 *
 * Pins on AVR devices can also be assigned using 0xPb for ports A to F
 *     Example: port D bit 3 is 0xD3
 */

#define glcd_PinConfigName "ks0108-Teensy"   // define name for configuration

/*
 *                 Arduino pin    AVR pin
 */ 
#define glcdPinData0    0        // AVRPIN_B0
#define glcdPinData1    1        // AVRPIN_B1
#define glcdPinData2    2        // AVRPIN_B2
#define glcdPinData3    3        // AVRPIN_B3
#define glcdPinData4    13       // AVRPIN_B4
#define glcdPinData5    14       // AVRPIN_B5
#define glcdPinData6    15       // AVRPIN_B6
#define glcdPinData7    4        // AVRPIN_B7

#define glcdPinDI       5        // AVRPIN_D0 (i2c SCL)
#define glcdPinRW       6        // AVRPIN_D1 (i2c SDA)
#define glcdPinEN       9        // AVRPIN_C6
#define glcdPinCSEL1    7        // AVRPIN_D2    // normal connection for control signals
#define glcdPinCSEL2    8        // AVRPIN_D3

//#define glcdPinBL       10       // AVRPIN_C7    // optional backlight control pin controls BL circuit
//#define glcdPinRES      17       // AVRPIN_F6    // optional s/w Reset control

#if NBR_CHIP_SELECT_PINS > 2
#error Missing glcdPinCSEL3 define
//#define glcdPinCSEL3    ?   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#error Missing glcdPinCSEL4 define
//#define glcdPinCSEL4    ?   // fourth chip select if needed
#endif

//#warning "KS0108 using pins for teensy"

#else
#error "Unsupported teensy processor type"
#endif

#endif //GLCD_PIN_CONFIG_H
