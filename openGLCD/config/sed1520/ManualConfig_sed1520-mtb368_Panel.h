/*
 * ManualConfig_sed1520-mtb368.h - Custom configuration for openGLCD library
 *
 *                                  View from Front of LCD
 *
 *    <-------------------------------- 122 Pixels -------------------------------->
 *    +--------------------------------------+-------------------------------------+ 
 *    | | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|              |
 *    +----------------------------------------------------------------------------+
 *    |                                      |                                     | ^
 *    |<------------ 61 Pixels ------------->|<------------ 61 Pixels ------------>| |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | 32 pixels
 *    |    Chip 0 controls these pixels      |     Chip 1 Controls these pixels    | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | v
 *    +--------------------------------------+-------------------------------------+
 *    | | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|              |
 *    +----------------------------------------------------------------------------+
 * Use this file to set io pins and LCD panel parameters
 *
 *
 *    +-------------------------------------------+
 *    |Pin|Symbol|        Function                | Hook To
 *    +-------------------------------------------+
 *    | 1 | Vss  | Ground                         | Ground
 *    +-------------------------------------------+
 *    | 2 | Vdd  | +5v (4ma max)                  | +5v
 *    +-------------------------------------------+
 *    | 3 | Vo   | Contrast input +5v to -5v      | Will work when not connected
 *    +-------------------------------------------+
 *    | 4 | A0/DI| H = Data, L=instruction/Status | glcdPinDI
 *    +-------------------------------------------+
 *    | 5 | CS1  | L = chip 0 selected            | glcdPinCSEL1
 *    +-------------------------------------------+
 *    | 6 | CS2  | L = chip 1 selected            | glcdPinCSEL2
 *    +-------------------------------------------+
 *    | 7 | CL   | Signal Clock 2Khz              | Must supply clock signal
 *    +-------------------------------------------+
 *    | 8 | E    | Enable Signal                  | glcdPinEN
 *    +-------------------------------------------+
 *    | 9 | R/W# | H = Read, L = Write            | glcdPinRW
 *    +-------------------------------------------+
 *    |10 | DB0  | Data Bit 0                     | glcdPinData0
 *    +-------------------------------------------+
 *    |11 | DB1  | Data Bit 1                     | glcdPinData1
 *    +-------------------------------------------+
 *    |12 | DB2  | Data Bit 2                     | glcdPinData2
 *    +-------------------------------------------+
 *    |13 | DB3  | Data Bit 3                     | glcdPinData3
 *    +-------------------------------------------+
 *    |14 | DB4  | Data Bit 4                     | glcdPinData4
 *    +-------------------------------------------+
 *    |15 | DB5  | Data Bit 5                     | glcdPinData5
 *    +-------------------------------------------+
 *    |16 | DB6  | Data Bit 6                     | glcdPinData6
 *    +-------------------------------------------+
 *    |17 | DB7  | Data Bit 7                     | glcdPinData7
 *    +-------------------------------------------+
 *    |18 | RES  | 1 = Bus MPU 68, 0=Bus MPU 80   | Vcc for MPU 68 mode
 *    +-------------------------------------------+
 *    |19 | BL(A)| LED Backlight Anode            | Through 100 ohm resistor to 5v
 *    +-------------------------------------------+
 *    |20 | BL(K)| LED Backlight Cathode          | Gnd or BL circuit
 *    +-------------------------------------------+
 *
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_ConfigName "mtb368-Manual"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 122
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     61  // pixels per chip 
#define CHIP_HEIGHT    32  // pixels per chip 

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * Pins can be assigned using arduino pin numbers 0-n
 * Pins on AVR devices can also be assigned using AVRPIN_Pb
 *    where P is port A-L and b is bit 0-7
 *     Example: port D bit 3 is AVRPIN_D3
 *
 * Pins on AVR devices can also be assigned using 0xPb for ports A to F
 *     Example: port D bit 3 is 0xD3
 */

#define glcdPinDI       A3    // Ao glcd pin
#define glcdPinCSEL1    A0
#define glcdPinCSEL2    A1

#define glcdPinEN      A4 // (is also i2C SDA on "uno" boards)
#define glcdPinRW      A2

/*
 * Data Pin definitions.
 * While any pin can be used for any data line, these pins in this order
 * allow nibble mode on the m328 boards which provides for faster i/o
 */
#define glcdPinData0    8
#define glcdPinData1    9
#define glcdPinData2   10
#define glcdPinData3   11
#define glcdPinData4    4
#define glcdPinData5    5
#define glcdPinData6    6
#define glcdPinData7    7


//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

/*
 * Two Chip panel using two select pins (the most common panel type)
 */
#define glcd_CHIP0 glcdPinCSEL1,LOW,   glcdPinCSEL2,HIGH
#define glcd_CHIP1 glcdPinCSEL1,HIGH,  glcdPinCSEL2,LOW    

/*********************************************************/
/*   Backlight Active Level Configuration                */
/*   Defines level on glcdPinBL pin to turn on backlight */
/*********************************************************/

#define glcd_BLactlevel HIGH

// defines for panel specific timing 
#define GLCD_tDDR    290    /* tACC6: Data Delay time (E high to valid read data)             */
#define GLCD_tAS      20    /* tAW6:  Address setup time (ctrl line changes to E high)        */
#define GLCD_tDSW    200    /* tDS6:  Data setup time (Write data lines setup to dropping E)  */
#define GLCD_tWH     450    /* tEW:   E hi level width (minimum E hi pulse width)             */ 
#define GLCD_tWL     450    /* E lo level width (minimum E lo pulse width)                    */


// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))

#include "device/sed1520_Device.h"
#endif //GLCD_PANEL_CONFIG_H
