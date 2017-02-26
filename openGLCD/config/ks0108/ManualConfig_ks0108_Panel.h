/*
 * ManualConfig_ks0108.h - User specific configuration for openGLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 * This version is for a standard/generic ks0108 display
 *
 */

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_ConfigName "ks0108-Manual"

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
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins on AVR devices can also be assigned using AVRPIN_Pb
 *   where P is port A-L and b is bit 0-7
 *   Example: port D pin 3 is AVRPIN_D3
 *
 * Pins on AVR devices can also be assigned using 0xPb for ports A to F
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

/* Arduino pins used for control
 * default assignment uses the first five analog pins
 */
#define glcdPinCSEL1        A0
#define glcdPinCSEL2        A1
// uncomment the following if more than two chip select pins are needed
//#define glcdPinCSEL3        3   // third chip select line on pin 3 if needed
//#define glcdPinCSEL4        2   // fourth chip select if needed

// For using multiple displays as a larger virtual display
//#define glcdPinCSEL5        ?   // 5th chip select if needed
//#define glcdPinCSEL6        ?   // 6th chip select if needed
//#define glcdPinCSEL7        ?   // 7th chip select if needed
//#define glcdPinCSEL8        ?   // 8th chip select if needed
//#define glcdPinCSEL9        ?   // 9th chip select if needed

#define glcdPinRW            A2
#define glcdPinDI            A3
#define glcdPinEN            A4
// Reset - uncomment the next line if glcd module reset is connected to an output pin
//#define glcdPinRES            A5    // optional s/w Reset control

// backlight control pin
//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

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
#define glcd_CHIP1  glcdPinCSEL1,LOW,  glcdPinCSEL2,HIGH
#define glcd_CHIP2  glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW

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
 * chip selects are active high
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
 * Defines for using 3 glcds as a virtual larger display with
 * each of the 3 panels using three Chips wth three select pins
 * Chip selects are active low
 * All panels will share data lines, E, DI, & RW.
 * Each panel will have its own contrast circuit.
 */

#elif (NBR_CHIP_SELECT_PINS == 9 && glcd_CHIP_COUNT == 9)  
// some sanity checks for the pin definitions
#if !defined(glcdPinCSEL1)
#error "glcdPinCSEL1 not defined"
#endif
#if !defined(glcdPinCSEL2)
#error "glcdPinCSEL2 not defined"
#endif
#if !defined(glcdPinCSEL3)
#error "glcdPinCSEL3 not defined"
#endif
#if !defined(glcdPinCSEL4)
#error "glcdPinCSEL4 not defined"
#endif
#if !defined(glcdPinCSEL5)
#error "glcdPinCSEL5 not defined"
#endif
#if !defined(glcdPinCSEL6)
#error "glcdPinCSEL6 not defined"
#endif
#if !defined(glcdPinCSEL7)
#error "glcdPinCSEL7 not defined"
#endif
#if !defined(glcdPinCSEL8)
#error "glcdPinCSEL8 not defined"
#endif
#if !defined(glcdPinCSEL9)
#error "glcdPinCSEL9 not defined"
#endif

// the actual chipselect definitions
#define glcd_CHIP0  glcdPinCSEL1,LOW, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP1  glcdPinCSEL1,HIGH, glcdPinCSEL2,LOW, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP2  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,LOW, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP3  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,LOW, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP4  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,LOW, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP5  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,LOW, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP6  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,LOW, glcdPinCSEL8,HIGH, glcdPinCSEL9,HIGH
#define glcd_CHIP7  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,LOW, glcdPinCSEL9,HIGH
#define glcd_CHIP8  glcdPinCSEL1,HIGH, glcdPinCSEL2,HIGH, glcdPinCSEL3,HIGH, glcdPinCSEL4,HIGH, glcdPinCSEL5,HIGH, glcdPinCSEL6,HIGH, glcdPinCSEL7,HIGH, glcdPinCSEL8,HIGH, glcdPinCSEL9,LOW

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
#define GLCD_tAS    140    /* Address setup time (ctrl line changes to E high)   */
#define GLCD_tDSW   200    /* Data setup time (data lines setup to dropping E)   */
#define GLCD_tWH    450    /* E hi level width (minimum E hi pulse width)        */
#define GLCD_tWL    450    /* E lo level width (minimum E lo pulse width)        */

#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
