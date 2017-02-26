/* 23 May 2014
 * ManualConfig_sed1520-4GLM12232_Panel.h - Custom configuration for openGLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 *
 *    This configuration file is for the Trumeter 4GLM12232 module manufactured
 *    by Falcon.
 *
 *    This module uses 2 SED1520 chips each controlling one half of the 
 *    display. There is a green LED backlight, and you must provide a current
 *    limiting resistor. The device needs a negative LCD bias voltage applied
 *    to VLCD, through a contrast pot. A charge pump fed from a PWM pin can 
 *    deliver this. For 3.3V operation expect VLCD to be approximately -1.8v.
 *
 *    Pixels are slightly taller than they are wide (0.41mm x 0.36mm) so that
 *    things like circles may look slightly squished. Additionally, there is a
 *    gap horizontally across the middle of the display, which makes graphics
 *    more interesting/difficult.
 *
 *    The datasheet can be downloaded from here:
 *    http://www.2shared.com/complete/XkJaQMzQ/4GLM_12232-2.html
 *    
 *                                  View from Front of LCD
 *
 *    <-------------------------------- 122 Pixels -------------------------------->
 *    +--------------------------------------+-------------------------------------+ 
 *    |                                  ^   |   ^                                 | ^
 *    |<------------ 61 Pixels ----------+-->|<--+--------- 61 Pixels ------------>| |
 *    |<---Chip 0 controls these pixels--+-->|<--+-Chip 1 Controls these pixels--->| |
 *    |                             16 pixels|16pixels                             | |
 *    |                                  |   |   |                                 | |
 *    |                                  v   |   v                                 | |
 *    +--------------------------------------+-------------------------------------+ |        
 *    |                     Gap between upper and lower pixels                     | 32 pixels
 *    +--------------------------------------+-------------------------------------+ |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |<---Chip 0 controls these pixels----->|<----Chip 1 Controls these pixels--->| |
 *    |                                      |                                     | |
 *    |                                      |                                     | |
 *    |                                      |                                     | v
 *    +--------------------------------------+-------------------------------------+
 *                               
 *     View from Back of LCD
 *    +----------------------------------------------------------------------------+ 
 *  --|-|1                                              |---SMT COMPONENTS---|     |
 *    | |                                                                          |
 *  F |C|                                                                          |
 *  l |a|                                                                          |
 *  e |b|                                                                          |
 *  x |l|                                                                          |
 *    |e|                                                                          |
 *    | |                                                                          |
 *  --|-| 18                                                                       |
 *    |                                                                            |
 *    |                                                                            |
 *    |                                                                            |
 *    |   |------------------ Connector to LCD itself --------------------------|  |
 *    +----------------------------------------------------------------------------+

 *
 *    +-------------------------------------------+
 *    |Pin|Symbol|        Function                | Hook To
 *    +-------------------------------------------+
 *    | 1 | Vdd  | +3.3v                          | +3.3v 
 *    +-------------------------------------------+
 *    | 2 | Vss  | Ground                         | GND
 *    +-------------------------------------------+
 *    | 3 | Vlcd | LCD bias voltage (contrast)    | Roughly -2.0v (approx Vlcd = Vdd-5.5)
 *    +-------------------------------------------+
 *    | 4 | RST  | Reset                          | glcdPinRES
 *    +-------------------------------------------+
 *    | 5 | E1   | chip 0 enable strobe           | glcdPinE1
 *    +-------------------------------------------+
 *    | 6 | E2   | chip 1 enable strobe           | glcdPinE2
 *    +-------------------------------------------+
 *    | 7 | RW   | H = Read, L = Write            | glcdPinRW
 *    +-------------------------------------------+
 *    | 8 | A0/DI| H = Data, L=instruction/Status | glcdPinDI
 *    +-------------------------------------------+
 *    | 9 | DB0  | Data Bit 0                     | glcdPinData0
 *    +-------------------------------------------+
 *    |10 | DB1  | Data Bit 1                     | glcdPinData1
 *    +-------------------------------------------+
 *    |11 | DB2  | Data Bit 2                     | glcdPinData2
 *    +-------------------------------------------+
 *    |12 | DB3  | Data Bit 3                     | glcdPinData3
 *    +-------------------------------------------+
 *    |13 | DB4  | Data Bit 4                     | glcdPinData4
 *    +-------------------------------------------+
 *    |14 | DB5  | Data Bit 5                     | glcdPinData5
 *    +-------------------------------------------+
 *    |15 | DB6  | Data Bit 6                     | glcdPinData6
 *    +-------------------------------------------+
 *    |16 | DB7  | Data Bit 7                     | glcdPinData7
 *    +-------------------------------------------+
 *    |17 | BLA  | Backlight LED Anode            | Connect to Vdd through resistor
 *    +-------------------------------------------+ (3.3v=8.2 Ohms, 5v=22 Ohms)
 *    |18 | BLK  | Backlight LED Cathode          | GND 
 *    +-------------------------------------------+
 *
 *    Simple inverting charge pump
 *           C1    D1
 *     PWM --|(--+-|<|-+-----+
 *     pin   +   |     |     /VR1
 *               |     -C2   \<--- Vlcd
 *               |     -+    /
 *               | D2  |     |
 *               |-|>|-+-----+
 *                     |
 *                    GND
 *     C1>200nF, C2=10uF
 *     D1,D2 = 1N5817 or similar
 *     VR1 = 10K-50K  
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_ConfigName "4GLM 12232"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 122
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     61   // pixels per chip 
#define CHIP_HEIGHT    32   // pixels per chip 

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


#define glcdPinData0    26
#define glcdPinData1    27
#define glcdPinData2    28
#define glcdPinData3    29
#define glcdPinData4    30
#define glcdPinData5    31
#define glcdPinData6    32
#define glcdPinData7    33

#define glcdPinE1      2
#define glcdPinE2      3
#define glcdPinRW      4
#define glcdPinDI      5    // A0 glcd pin
#define glcdPinRES     6    // Reset pin

//#define glcdPinBL       7 


/*********************************************************/
/*   Backlight Active Level Configuration                */
/*   Defines level on glcdPinBL pin to turn on backlight */
/*********************************************************/

#define glcd_BLactlevel HIGH

// defines for panel specific timing - from SED1520 datasheet.
#define GLCD_tDDR    180    /* tACC6: Data Delay time (E high to valid read data)             */
#define GLCD_tAS      40    /* tAW6:  Address setup time (ctrl line changes to E high)        */
#define GLCD_tDSW    160    /* tDS6:  Data setup time (Write data lines setup to dropping E)  */
#define GLCD_tWH     200    /* tEW:   E hi level width (minimum E hi pulse width)             */ 
#define GLCD_tWL     200    /* E lo level width (minimum E lo pulse width) - not specified    */

// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))


#include "device/sed1520_Device.h"

#ifdef _PERIPHERAL_LIBRARY_MASTER_HEADER_FILE
#error plib.h is included
#endif 
#endif //GLCD_PANEL_CONFIG_H
