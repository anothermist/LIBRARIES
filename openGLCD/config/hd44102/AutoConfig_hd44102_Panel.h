/*
 * AutoConfig_hd44102_Panel.h - User specific configuration for openGLCD library
 *
 * Use this file to set LCD panel parameters
 * This version is for a standard hd44102 display
 * This file uses a board specific pin assignment file based on the board selected in the IDE
 * FIXME: For now, it borrows the pin config files from the ks0108
 *
*/

#ifndef GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_PanelConfigName "hd44102"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 100
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     50  // pixels per chip
#define CHIP_HEIGHT    32  // pixels per chip

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
#error "The number of Chips and Chip Select pins does not match an option in hd44102_Panel.h"
#error "Check that the number of Chip Select pins is correct for the configured panel size"
#endif

/*********************************************************/
/*  End of Chip Select Configuration                     */
/*********************************************************/

/*********************************************************/
/*   Backlight Active Level Configuration                */
/*   Defines level on glcdPinBL pin to turn on backlight */
/*    (may require additional transistor circuitry)      */
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


#include "../ks0108/AutoPinConfig_ks0108.h"  // cheat and use same pin configs as ks0108
#include "device/hd44102_Device.h"
#endif //GLCD_PANEL_CONFIG_H
