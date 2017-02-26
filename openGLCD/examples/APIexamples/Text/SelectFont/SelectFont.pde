/*
 * SelectFont - openGLCD API example
 *
 * Example of using:
 * SelectFont()
 *
 * SelectFont(font, [fgcolor]);
 *
 * font     Font definition
 * fgcolor  Foreground font color 
 * 
 * Select a Font and font color
 *
 * Selects the font definition as the current font for the text area.
 *
 * All subsequent printing functions will use this font. 
 *
 * Font definitions are stored in program memory 
 * You can have as many fonts defines as will fit in program memory
 * and can switch between them with this function.
 *
 * fgcolor PIXEL_ON or BLACK renders "on" pixels on a "off" background,
 * i.e. it turns on the pixels on the LCD for the pixels in the character
 * glpyh* and turns off all the background pixels.
 *
 * fgcolor PIXEL_OFF or WHITE renders "on" pixels on a "off" background;
 * i.e. it turns off the pixels on the LCD for the pixels in the character
 * glpyh and turns on all the background pixels.
 *
 * Examples:
 *    GLCD.SelectFont(System5x7);            // fg pixels on, bg pixels off
 *    GLCD.SelectFont(System5x7, PIXEL_ON);  // fg pixels on, bg pixels off
 *    GLCD.SelectFont(System5x7, PIXEL_OFF); // fg pixels off, bg pixels on
 *    GLCD.SelectFont(Arial14);
 *    textarea.SelectFont(fixednums7x15, PIXEL_OFF);
 *
 * note:
 * When the display is initilized in INVERTED mode,
 * the colors are reversed.
 *
 * note:
 * AVR systems require fonts to fit in lower 64k of program memory
 *
 *
 * Created by Bill Perry 2013-09-13
 * bperrybap@opensource.billsworld.billandterrie.com
 *  
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

/*
 * Create a text area for left & right half of display
 */
gText textAreaLeft(textAreaLEFT);
gText textAreaRight(textAreaRIGHT);

void setup()
{
	GLCD.Init();
}


void loop()
{
	GLCD.SelectFont(System5x7); // use System5x7 in "default" text area

	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("SelectFont"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
	 * Now switch to Arial14
	 */
	GLCD.SelectFont(Arial14);
	GLCD.println(F("Hello"));
	delay(DELAY/4);
	/*
	 * Now switch back to System5x7
	 */
	GLCD.SelectFont(System5x7);
	GLCD.print(F("World"));

	delay(DELAY);
	GLCD.ClearScreen();


	/*
	 * Select fonts for left and right text areas
	 */
	textAreaLeft.SelectFont(System5x7);
	textAreaRight.SelectFont(Arial14);

	textAreaLeft.println(F("Hello"));
	delay(DELAY/4);
	textAreaRight.println(F("Hello"));

	delay(DELAY/2);

	/*
	 * Now reverse the fonts for the left and right areas
	 */
	textAreaLeft.SelectFont(Arial14);
	textAreaRight.SelectFont(System5x7);

	textAreaLeft.println(F("World"));
	delay(DELAY/4);
	textAreaRight.println(F("World"));

	delay(DELAY);
	/*
	 * Clear everything
	 */
	GLCD.ClearScreen();

	/*
	 * Must clear left and right text areas
	 * since although the physical lcd has been cleared
	 * the text areas "remember" where the last text was
	 * written
 	 */
	textAreaLeft.ClearArea();
	textAreaRight.ClearArea();
}
