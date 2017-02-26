/*
 * openGLCD_Config.h - User specific configuration for openGLCD library
 *
 * This file is shipped to automatically configure the library for a 
 * 128x64 ks0108 panel using the wiring described in the documentation.
 *
 * Use this file to select the active panel configuration file
 * Then edit the selected panel file to configure parameters for that panel.
 *
 * For Teensy devices the default wiring matches the wiring documented
 * on the Teensy website.
 * http://www.pjrc.com/teensy/td_libs_GLCD.html
 *
 */

#ifndef openGLCD_CONFIG_H
#define openGLCD_CONFIG_H

/*
 * Select a configuration file by uncommenting one line to define the
 * the desired configuration file.
 *
 * Select 1 and only 1 configuration file.
 *
 * configurations can be "Manual" or "AutoConfig".
 *
 * The "Manual" configuration files configure the library in a fixed configuration
 * regardless of Arduino board type. Everything to configure the panel and pins is
 * in a single file.
 *
 * The "AutoConfig" configuration files will configure the library based on a set of
 * pre-customized configurations for the supported Arduino board type. Autonfiguration
 * files break out the panel confuration from the pin configuration and allows the
 * Arduino pins selected to be based on the Arduino board type.
 *
 */

/****************************************************************************************************
 *
 * Autoconfig files
 *
 ****************************************************************************************************/

/*
 *
 * These configurations match the the panel and pin configurations
 * in the library documentation based on the type of board selected in the Arduino IDE.
 *
 * NOTE: if you have a panel that has different dimensions than the distributed configuration,
 * you can either edit the supplied configuration file for that board type or create a fully
 * custom configuration file.
 *
 * The auto configuration panel configuration files can be found under the "config" directory,
 * with a seperate directory for the files related to each panel type.
 * The naming is:
 *  config/{PANELNAME}/AutoConfig_{PANELNAME}-{MODELNAME}_Panel.h
 * Where:
 *   {PANELNAME} is the glcd panel type. (for example, ks0108)
 *   -{MODELNAME} is optional and is a model name of the glcd (for example, 128x64, JHD19264A)
 *
 * The pin configuration files for each board type can also be found in the same directory,
 * the naming is:
 *  config/{PANELNAME}/PinConfig_{PANELNAME}-{BOARDNAME}.h
 * Where:
 *   {PANELNAME} is the glcd panel type. (for example, ks0108)
 *   {BOARDNAME} is the name of the Arduino board (or board family as selected in the Arduino IDE).
 *
 * So for example, the auto configuration file name for the default ks0108 panel
 * is: "config/ks0108/AutoConfig_ks0108-128x64_Panel.h"
 * The ks0108 pin configuration file for a "Uno" board is:
 *  "config/ks0108/PinConfig_ks0108-Uno.h"
 * The pin configuration file for a "Mega" board:
 *  "config/ks0108/PinConfig_ks0108-Mega.h"
 * Teensy boards are an exception as all boards share a common pinconfig file:
 *  "config/ks0108/PinConfig_ks0108-Teensy.h"
 * 
 */


// automatically configure library for a ks0108 128x64 panel
#define GLCDCFG_GLCDCONFIG "config/ks0108/AutoConfig_ks0108-128x64_Panel.h"

// automatically configure library for a ks0108 192x64 panel
//#define GLCDCFG_GLCDCONFIG "config/ks0108/AutoConfig_ks0108-192x64_Panel.h"

// automatically configure library for a ks0108 JHD19264A panel
//#define GLCDCFG_GLCDCONFIG "config/ks0108/AutoConfig_ks0108-JHD19264A_Panel.h"

// automatically configure library for a ks0108 HJ19264A panel
//#define GLCDCFG_GLCDCONFIG "config/ks0108/AutoConfig_ks0108-HJ19264A_Panel.h"

// automatically configure library for a hd44102 panel
//#define GLCDCFG_GLCDCONFIG "config/hd44102/AutoConfig_hd44102_Panel.h"    

/****************************************************************************************************
 *
 * Manual config files
 *
 ****************************************************************************************************/

/*
 * If you want to explicitly select a manual configuration, you can edit the desired manual
 * configuration to fill in or modify the configuration information to meet your needs.
 * You can also use these files as a template to make customized copies to hold specific
 * configurations.
 *
 * Remember that to activate your manual configuration you uncomment the #define for the desired file
 * and make sure that all the other config  #includes are commented (including the autoconfig above) 
 */

// generic ks0108 configuration (currently this uses same pins as the UNO autoconfiguration)
//#define GLCDCFG_GLCDCONFIG "config/ks0108/ManualConfig_ks0108_Panel.h"

// configuration for BGMicro 128x64 display with pinout diagram
//#define GLCDCFG_GLCDCONFIG "config/ks0108/ManualConfig_ks0108-agm1264f_Panel.h"

// configuration for sed1520 based 4GLM12232 122x32 display with pinout diagram
//#define GLCDCFG_GLCDCONFIG "config/sed1520/ManualConfig_sed1520-4GLM12232_Panel.h"

// configuration for Russian "sed1520" based mt12232 122x32 display with pinout diagram
//#define GLCDCFG_GLCDCONFIG "config/sed1520/ManualConfig_sed1520-mt12232d_Panel.h"

// configuration for sed1520 basaed mtb-368 122x32 display with pinout diagram
//#define GLCDCFG_GLCDCONFIG "config/sed1520/ManualConfig_sed1520-mtb368_Panel.h"

// configuration for sed1520 basaed vk5121 120x32 display with pinout diagram
//#define GLCDCFG_GLCDCONFIG "config/sed1520/ManualConfig_sed1520-vk5121_Panel.h"

// configuration for sed1520 basaed EA DIP180-5e 180x32 display with pinout diagram
//#define GLCDCFG_GLCDCONFIG "config/sed1520/ManualConfig_sed1520-EADIP180-5_Panel.h"

// generic sed1520 configuration (may required some amount of editing before using)
//#define GLCDCFG_GLCDCONFIG "config/sed1520/ManualConfig_sed1520_Panel.h"

/*
 * For s/w development debugging
 */
//#define GLCDCFG_GLCDCONFIG "build/debug/bap/bapdebug.h"
//#define GLCDCFG_GLCDCONFIG "build/debug/bap/config/Modvk5121_AutoConfig_Config.h"
//#define GLCDCFG_GLCDCONFIG "build/debug/bap/config/Modagm1264f_AutoConfig_Config.h"
//#define GLCDCFG_GLCDCONFIG "build/debug/bap/config/Modagm1264f_TeensyBB128_Config.h"
//#define GLCDCFG_GLCDCONFIG "build/debug/bap/config/ModDualvk5121_TeensyBB_Config.h"



/*========================== Optional User Defines ==================================*/

//#define GLCDCFG_READ_CACHE	// Turns on code that uses a frame buffer for a read cache
				// This adds only ~52 bytes of code (on AVR) but...
				// will use DISPLAY_HEIGHT/8 * DISPLAY_WIDTH bytes of RAM
				// A typical 128x64 ks0108 will use 1k of RAM for this.
				// performance increase is quite noticeable (double or so on FPS test)
				// This will not work on smaller AVRs like the mega168 that only
				// have 1k of RAM total.

//#define GLCDCFG_UTF8		// Turns on code that will process character codes as UTF8
				// encoded characters & character strings.
				// Note that due to the the current font header,
				// only UTF8 code points from 0-255 can be supported.
				// Also, note that when enabled, raw character codes from 0x80-0xff 
				// can still be used with the exception of 0xc2 and 0xc3 which are
				// the 0x80-0xff UTF8 markers.
				// When enabled, the functions writeUTF8(c) and PutChar(c)
				// can be used to output extended (multi-byte)
				// encoded UTF8 character codes.


//#define GLCDCFG_NO_PRINTF	// disable xxprintf() support
				// does not save any code space if no xxprintf() routines
				// are not being used.

//#define GLCDCFG_NO_SCROLLDOWN // disable reverse scrolling (saves ~600 bytes of code on AVR)
                                // Allows those tight on FLASH space to save a bit of code space

//#define GLCDCFG_ATOMIC_IO	// Generate code that ensures all AVR pin i/o operations are atomic
                                // including any potential nibble operations.
                                // Without this option enabled, AVR nibble operations will be
                                // slightly faster but might have issues if a pin used shares a
                                // processor i/o port with an interrupt routine that
                                // updates pins/bits on the same port.


//#define GLCDCFG_NODEFER_SCROLL // uncomment to disable deferred newline processing

//#define GLCDCFG_NOINIT_CHECKS	// uncomment to remove initialization busy status checks
				// this turns off the code in the low level init code that
				// checks for a module stuck BUSY or stuck in RESET.
				// This will save about 100 bytes of code space (on AVR) in normal
                                // sketches and an additional 220 bytes in the diag sketch.
                                // This will cause diags to hang if wires are not correct vs
                                // return an error.

//#define GLCDCFG_FORCE_CORECODE // Forces library to use generic i/o routines rather than
                                 // faster direct port i/o routines
                                 // The config file used when in CORECODE mode is 
                                 // config/{PANELNAME}/PinConfig_{PANELNAME}-CoreCode.h
                                 // While much slower, this mode can be useful if there
                                 // are direct port i/o mapping issues.

/*========================== End of Optional User Defines ==================================*/

#endif  // openGLCD_CONFIG_h
