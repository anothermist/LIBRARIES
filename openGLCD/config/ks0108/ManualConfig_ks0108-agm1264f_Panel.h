/*
 * ManualConfig_Modagm1264f.h - User specific configuration for openGLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 *
 *    This configuration file is for the AGM1264F module manufactured by
 *    AZ Displays.
 *    This module uses 2 ks0108 chips each controlling one half of the display.
 *
 *    This is the 128x64 GLCD that was sold by BGmicro
 *    The datasheet can be downloaded from here:
 *    http://docs.bgmicro.com/pdf/lcd1030.pdf
 *    
 *
 *                                  View From Front of LCD
 *
 *    <-------------------------------- 128 Pixels -------------------------------->
 *    +--------------------------------------+-------------------------------------+ 
 *    |                                      |                                     | ^
 *    |<------------ 64 Pixels ------------->|                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | 64 pixels
 *    |    Chip 0 controls these pixels      |     Chip 1 Controls these pixels    | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | v
 *    +--------------------------------------+-------------------------------------+
 *    |       |20|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1|        |
 *    +----------------------------------------------------------------------------+
 *
 *    +-------------------------------------------+
 *    |Pin|Symbol|        Function                | Hook To
 *    +-------------------------------------------+
 *    | 1 | Vss  | Ground                         | Gnd
 *    +-------------------------------------------+
 *    | 2 | Vdd  | +5v (15ma max)                 | +5v ---------------------+
 *    +-------------------------------------------+                          |
 *    | 3 | Vo   | LCD contrast adjust            |--------------------+     |
 *    +-------------------------------------------+                    |     |
 *    | 4 | DI   | H = Data, L=instruction/Status | AVR Pin            |     |
 *    +-------------------------------------------+                    |     |
 *    | 5 | RW   | H = Read, L = Write            | AVR Pin            |     |
 *    +-------------------------------------------+                    |     |
 *    | 6 | E    | Enable signal                  | AVR Pin            |     |
 *    +-------------------------------------------+                    |     |
 *    | 7 | DB0  | Data Bit 0                     | AVR Pin            |     |
 *    +-------------------------------------------+                    |     |
 *    | 8 | DB1  | Data Bit 1                     | AVR Pin            |     |
 *    +-------------------------------------------+                    |     |
 *    | 9 | DB2  | Data Bit 2                     | AVR Pin            |     |
 *    +-------------------------------------------+                    |     |
 *    |10 | DB3  | Data Bit 3                     | AVR Pin      /\/\/\/\/\/\|
 *    +-------------------------------------------+              |  (10-20k)
 *    |11 | DB4  | Data Bit 4                     | AVR Pin      | 
 *    +-------------------------------------------+              | Contrast Pot
 *    |12 | DB5  | Data Bit 5                     | AVR Pin      |
 *    +-------------------------------------------+              |
 *    |13 | DB6  | Data Bit 6                     | AVR Pin      |
 *    +-------------------------------------------+              |
 *    |14 | DB7  | Data Bit 7                     | AVR Pin      |
 *    +-------------------------------------------+              |
 *    |15 | CS1  | H = chip 0 selected            | AVR Pin      |
 *    +-------------------------------------------+              |
 *    |16 | CS2  | H = chip 1 selected            | AVR Pin      |
 *    +-------------------------------------------+              |
 *    |17 | RST  | L = Reset                      | AVR Pin      |
 *    +-------------------------------------------+              |
 *    |18 | VEE  | LCD contrast power supply      |--------------+
 *    +-------------------------------------------+
 *    |20 | BL-  | Backlight -/gnd                | Gnd or connect to BL circuit for s/w control
 *    +-------------------------------------------+
 *    |20 | BL+  | Backlight + (560ma max)        | 4-10+ ohm Resistor to +5v
 *    +-------------------------------------------+  
 *
 */
#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for configuration file
 */
#define glcd_ConfigName "AGM1264F-Manual"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip
#define CHIP_HEIGHT    64  // pixels per chip

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * Pins can be assigned using arduino pin numbers 0-n
 * Pins on AVR devices can also be assigned using AVRPIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *    Example: port D pin 3 is AVRPIN_D3
 *
 * Pins on AVR devices can also be assigned using 0xPb for ports A to F
 *     Example: port D bit 3 is 0xD3
 */


/*
 * Data pin definitions
 */
#define glcdPinData0        8
#define glcdPinData1        9
#define glcdPinData2        10
#define glcdPinData3        11
#define glcdPinData4        4
#define glcdPinData5        5
#define glcdPinData6        6
#define glcdPinData7        7

/*
 * Arduino pins used for control
 */
#define glcdPinCSEL1         14
#define glcdPinCSEL2         15

#define glcdPinRW            16
#define glcdPinDI            17
#define glcdPinEN            18
// Reset - uncomment the next line if glcd reset is connected to an Arduino pin
//#define glcdPinRES            19    // optional s/w Reset control

//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

/*********************************************************/
/*  Chip Select pin assignments                          */
/*********************************************************/

/*
 * Two Chip panel using two select pins
 */

#define glcd_CHIP0 glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW
#define glcd_CHIP1 glcdPinCSEL1,LOW, glcdPinCSEL2,HIGH

/*********************************************************/
/*   Backlight Active Level Configuration                */
/*   Defines level on glcdPinBL pin to turn on backlight */
/*    (Requires additional transistor circuitry)         */
/*********************************************************/

#define glcd_BLactlevel HIGH

/*
 * The following defines are for panel specific low level timing.
 *
 * See your data sheet for the exact timing and waveforms.
 * All defines below are in nanoseconds.
 */

#define GLCD_tDDR   320    /* Data Delay time (E high to valid read data)        */
#define GLCD_tAS    140    /* Address setup time (ctrl line changes to E high)   */
#define GLCD_tDSW   200    /* Data setup time (data lines setup to dropping E)   */
#define GLCD_tWH    450    /* E hi level width (minimum E hi pulse width)        */
#define GLCD_tWL    450    /* E lo level width (minimum E lo pulse width)        */

// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))


#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
