/*
 * AutoConfig_ks0108-JHD19264A_Panel.h - configuration for openGLCD library
 *
 * Use this file to set LCD panel parameters
 * This config file is for a JHD19264A ks0108 display
 * This file uses a board specific pin assignment file based
 * on the board selected in the IDE
 *
 * NOTE: that CSA should be connected to glcdPinCSEL2 and CSB is connected to glcdPinCSEL1
 *
 * The chip select mappings are set up this way intentionally to be compabible with the 
 * default 128x64 ks0108 configuration file.
 * If you wire up CSA to glcdPinCSEL2 and CSB to glcd CSEL1, the display will function with
 * the default 128x64 ks0108 config file out of the box but will only use 128 of the 192 pixels. The pixels used
 * will be the right most 128 pixels. If you wire up CSA to glcdSEL1 and CSB to glcdSEL2
 * then when using the default 128x64 ks0108 config file, you end up with a 128 pixel display
 * that uses the right side of the display but the 0-63 and 64-128 chips/pixels will be flipped.
 *
 *    +--------------------------------------------------------------------------------------------+
 *    |   | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|                            |
 *    +------------------------------+------------------------------+------------------------------+
 *    |                              |                              |                              | ^
 *    |<--------- 64 Pixels -------->|                              |                              | |
 *    |                              |                              |                              | |
 *    | Chip 0 controls these pixels | Chip 1 Controls these pixels | Chip 2 Controls these pixels | |
 *    |                              |                              |                              | 64 pixels
 *    | (pixels 0-63 in 192x64 mode) |(pixels 64-127 in 192x64 mode)|(pixels 128-191in 192x64 mode)| |
 *    |                              |                              |                              | |
 *    |  (Not used in 128x64 mode)   | (pixels 0-63 in 128x64 mode) |(pixels 64-128 in 128x64 mode)| |
 *    |                              |                              |                              | v
 *    +------------------------------+------------------------------+------------------------------+
 *    <--------------------------------------- 192 Pixels ----------------------------------------->
 *
 *    +---+------+--------------------------------+
 *    | 1 | DB7  | Data Bit 7                     | AVR Pin (glcdPinData0)
 *    +---+------+--------------------------------+
 *    | 2 | DB6  | Data Bit 6                     | AVR Pin (glcdPinData1)
 *    +---+------+--------------------------------+
 *    | 3 | DB5  | Data Bit 5                     | AVR Pin (glcdPinData2)
 *    +---+------+--------------------------------+
 *    | 4 | DB4  | Data Bit 4                     | AVR Pin (glcdPinData3)
 *    +---+------+--------------------------------+
 *    | 5 | DB3  | Data Bit 3                     | AVR Pin (glcdPinData4)
 *    +---+------+--------------------------------+
 *    | 6 | DB2  | Data Bit 2                     | AVR Pin (glcdPinData5)
 *    +---+------+--------------------------------+
 *    | 7 | DB1  | Data Bit 1                     | AVR Pin (glcdPinData6)
 *    +---+------+--------------------------------+
 *    | 8 | DB0  | Data Bit 0                     | AVR Pin (glcdPinData7)
 *    +---+------+--------------------------------+
 *    | 9 | E    | Enable signal                  | AVR Pin (glcdPinEN)
 *    +---+------+--------------------------------+
 *    |10 | RW   | H = Read, L = Write            | AVR Pin (glcdPinRW)
 *    +---+------+--------------------------------+
 *    |11 | RS   | H = Data, L=instruction/Status | AVR Pin (glcdPinDI)
 *    +---+------+--------------------------------+
 *    |12 | Vo   | LCD contrast adjust            |-----(wiper)-------------+
 *    |---+------+--------------------------------+                         |
 *    |13 | Vdd  | +5v                            | +5v ---------------/\/\/\/\/\ (10k-100k pot)
 *    +---+------+--------------------------------+                         |
 *    |14 | Vss  | Ground                         | Gnd                     |
 *    +---+------+--------------------------------+                         |
 *    |15 | CSB  |  B=L,A=L  LEFT  chip (chip 0)  | AVR Pin (glcdPinCSEL1)  |
 *    +---+------+- B=H,A=L MIDDLE chip (chip 1) -+                         |
 *    |16 | CSA  |  B=L,A=H RIGHT  chip (chip 2)  | AVR Pin (glcdPinCSEL2)  |
 *    +---+---------------------------------------+                         |
 *    |18 | VEE  | LCD contrast power supply(-10v)|-------------------------+
 *    +-------------------------------------------+
 *    |19 | LED+ | Backlight LED Anode            | +5v (otionally use 3-5 ohm resistor to reduce current)
 *    +-------------------------------------------+
 *    |20 | LED- | Backlight LED Negative         | Gnd or connect to BL circuit for s/w control
 *    +-------------------------------------------+
 *
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_PanelConfigName "ks0108-JHD19264A"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 192
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip
#define CHIP_HEIGHT    64  // pixels per chip

/*
 * the following is the calculation of the number of chips - do not change
 */
#define glcd_CHIP_COUNT (((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH) * ((DISPLAY_HEIGHT + CHIP_HEIGHT -1) / CHIP_HEIGHT))

/*********************************************************/
/*  Chip Select Configuration                            */
/*********************************************************/

/*
 * Change the following define to match the number of Chip Select pins for this panel
 * Most panels use two pins for chip select,
 * but check your datasheet to see if a different number is required
 */
#define NBR_CHIP_SELECT_PINS   2 // the number of chip select pins required for this panel 

/*
 * The following conditional statements determine the relationship between the chip select
 * pins and the physical chips.
 * If the chips are displayed in the wrong order, you can swap the glcd_CHIPx defines 
 */  

/* 
 * Defines for Panels using two Chip Select pins
 */  
#if  NBR_CHIP_SELECT_PINS == 2

/*
 * Two Chip panels using two select pins (the most common panel type)
 */
#if glcd_CHIP_COUNT == 2
#define glcd_CHIP0 glcdPinCSEL1,HIGH,   glcdPinCSEL2,LOW
#define glcd_CHIP1 glcdPinCSEL1,LOW,    glcdPinCSEL2,HIGH    

/*
 * Three Chip panel using two select pins
 */
#elif  glcd_CHIP_COUNT == 3 

#define glcd_CHIP0  glcdPinCSEL1,LOW,  glcdPinCSEL2,LOW
#define glcd_CHIP1  glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW
#define glcd_CHIP2  glcdPinCSEL1,LOW,  glcdPinCSEL2,HIGH

/*
 * Four Chip panel using two select pins
 */
#elif  glcd_CHIP_COUNT == 4 
#define glcd_CHIP0  glcdPinCSEL1,LOW,  glcdPinCSEL2,LOW
#define glcd_CHIP1  glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW
#define glcd_CHIP2  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH    
#define glcd_CHIP3  glcdPinCSEL1,LOW,  glcdPinCSEL2,HIGH    
#endif

/*
 * Defines for Two Chip panels using one Chip Select pin 
 */
#elif  (NBR_CHIP_SELECT_PINS == 1 && glcd_CHIP_COUNT == 2)  
#define glcd_CHIP0  glcdPinCSEL1,LOW
#define glcd_CHIP1  glcdPinCSEL1,HIGH    

/*
 * Defines for Three Chip panels using three select pins
 */
#elif (NBR_CHIP_SELECT_PINS == 3 && glcd_CHIP_COUNT == 3)  
#define glcd_CHIP0  glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW,  glcdPinCSEL3,LOW
#define glcd_CHIP1  glcdPinCSEL1,LOW,  glcdPinCSEL2,HIGH, glcdPinCSEL3,LOW
#define glcd_CHIP2  glcdPinCSEL1,LOW,  glcdPinCSEL2,LOW,  glcdPinCSEL3,HIGH    

/*
 * Defines for Four Chip panel using four select pins
 */
#elif  (NBR_CHIP_SELECT_PINS == 4 && glcd_CHIP_COUNT == 4) 
#define glcd_CHIP0  glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW,  glcdPinCSEL3,LOW,  glcdPinCSEL4,LOW
#define glcd_CHIP1  glcdPinCSEL1,LOW,  glcdPinCSEL2,HIGH, glcdPinCSEL3,LOW,  glcdPinCSEL4,LOW
#define glcd_CHIP2  glcdPinCSEL1,LOW,  glcdPinCSEL2,LOW,  glcdPinCSEL3,HIGH, glcdPinCSEL4,LOW
#define glcd_CHIP3  glcdPinCSEL1,LOW,  glcdPinCSEL2,LOW,  glcdPinCSEL3,LOW,  glcdPinCSEL4,HIGH    

/*
 * Here if the Number of Chip Selects is not supported for the selected panel size and chip size
 */
#else
#error "The number of Chips and Chip Select pins does not match an option in ks0108_Panel.h"
#error "Check that the number of Chip Select pins is correct for the configured panel size"
#endif

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
#define GLCD_tAS    140    /* Address setup time (ctrl line changes to E HIGH   */
#define GLCD_tDSW   200    /* Data setup time (data lines setup to dropping E)   */
#define GLCD_tWH    450    /* E hi level width (minimum E hi pulse width)        */
#define GLCD_tWL    450    /* E lo level width (minimum E lo pulse width)        */


#include "AutoPinConfig_ks0108.h"
#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
