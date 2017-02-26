/*
 * ManualConfig_sed1520_Panel.h - Custom Generic sed1520 configuration for openGLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 *
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_ConfigName "sed1520-Manual"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 122
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     61  // pixels per chip 
#define CHIP_HEIGHT    32  // pixels per chip 

// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))

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


#define glcdPinData0    8
#define glcdPinData1    9
#define glcdPinData2   10
#define glcdPinData3   11
#define glcdPinData4    4
#define glcdPinData5    5
#define glcdPinData6    6
#define glcdPinData7    7

#define glcdPinRW      A4
#define glcdPinDI      A5    // Ao glcd pin

/*
 * modules may use 
 * - chip selects and 1 enable
 * - 2 enables and no chip selects
 * - 3 enables and no chip selects
 * - 4 enables and no chip selects
 *
 */

#define NBR_CHIP_SELECT_PINS 2 // fill in with how many chip selects

#if  NBR_CHIP_SELECT_PINS == 0

#define glcdPinE1      A0
#define glcdPinE2      A1

#if (glcd_CHIP_COUNT > 2) // add a third enable for 3 chips
#define glcdPinE2      A2

#if (glcd_CHIP_COUNT > 3) // add a forth enabl for 4 chips
#define glcdPinE3      A3

#elif NBR_CHIP_SELECT_PINS == 2
// dual chipselects so use dual chipselects and enable
#define glcdPinCSEL1   A0
#define glcdPinCSEL2   A1
#define glcdPinEN      A2
// chipselect definitions for each chip
#define glcd_CHIP0 glcdPinCSEL1,LOW,   glcdPinCSEL2,HIGH
#define glcd_CHIP1 glcdPinCSEL1,HIGH,  glcdPinCSEL2,LOW    


#else
#error unsupported number of chipselects
#endif


//#define glcdPinBL       3 // optional backlight control pin

/*********************************************************/
/*   Backlight Active Level Configuration                */
/*   Defines level on glcdPinBL pin to turn on backlight */
/*********************************************************/

#define glcd_BLactlevel HIGH

// defines for panel specific timing (times in nano seconds)
#define GLCD_tDDR    290    /* tACC6: Data Delay time (E high to valid read data)             */
#define GLCD_tAS      20    /* tAW6:  Address setup time (ctrl line changes to E high)        */
#define GLCD_tDSW    200    /* tDS6:  Data setup time (Write data lines setup to dropping E)  */
#define GLCD_tWH     450    /* tEW:   E hi level width (minimum E hi pulse width)             */ 
#define GLCD_tWL     450    /* E lo level width (minimum E lo pulse width)                    */


#include "device/sed1520_Device.h"
#endif //GLCD_PANEL_CONFIG_H
