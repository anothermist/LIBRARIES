/*
 * SetFontColor - openGLCD API example
 *
 * Example of using:
 * SetFontColor()
 *
 * SetFontColor(fgcolor);
 *
 * fgcolor  Foreground font color 
 *
 * Set the font foreground color for the text area.
 *
 * fgcolor PIXEL_ON or BLACK renders "on" pixels on an "off" background,
 * i.e. it turns on the pixels on the LCD for the pixels in the character
 * glpyh and turns off all the background pixels.
 *
 * fgcolor PIXEL_OFF or WHITE renders "on" pixels on a "off" background;
 * i.e. it turns off the pixels on the LCD for the pixels in the character
 * glpyh and turns on all the background pixels.
 *
 * Examples:
 *    GLCD.SetFontColor(PIXEL_ON);  // fg pixels on, bg pixels off
 *    GLCD.SetFontColor(BLACK);     // fg pixels on, bg pixels off
 *    GLCD.SetFontColor(PIXEL_OFF); // fg pixels off, bg pixels on
 *    GLCD.SetFontColor(WHITE);     // fg pixels off, bg pixels on
 *    textarea.SetFontColor(PIXEL_OFF);
 *
 * note:
 * When the display is initilized in INVERTED mode,
 * the colors are reversed.
 *
 * The color becomes effective for all future text operations but
 * does not alter the area, including any existing text in the area.
 * This allows switching colors as printing is done within the area.
 * If a ClearScreen() is done, then the area is cleared according
 * to the color.
 *
 * Created by Bill Perry 2013-09-13
 * bperrybap@opensource.billsworld.billandterrie.com
 *  
 */

#include <openGLCD.h>

/*
 * Note: delays are simply for visuals
 */

#define DELAY 3000 // ms delay between examples

/*
 * Create a text area for left & right half of display
 * And set the fonts for them.
 */
gText textAreaLeft(textAreaLEFT, System5x7);
gText textAreaRight(textAreaRIGHT, Arial14);

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use System5x7 in "default" text area
}

void loop()
{
	GLCD.SetFontColor(PIXEL_ON);

	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString("SetFontColor", gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();

	GLCD.print("Hello ");
	delay(DELAY/4);
	/*
	 * Now switch color
	 */
	GLCD.SetFontColor(PIXEL_OFF);
	GLCD.print("World");

	delay(DELAY/2);
	/*
	 * This ClearArea will use the current font fgcolor
	 * which is PIXEL_OFF
	 */
	GLCD.ClearArea();

	delay(DELAY/4);
	GLCD.print("Hello ");
	delay(DELAY/4);
	GLCD.print("World");
	delay(DELAY/2);

	/*
	 * Clear the screen
	 * note: ClearScreen() does not use text area fgcolor
	 */

	GLCD.ClearScreen();

	 /*
	  * Set the colors
	  */
	textAreaLeft.SetFontColor(PIXEL_ON);
	textAreaRight.SetFontColor(PIXEL_OFF);

	textAreaLeft.print("Hello");
	delay(DELAY/4);
	textAreaRight.print("Hello");

	delay(DELAY/2);

	/*
	 * Now Change the font colors the fonts for the left and right areas
	 */
	textAreaLeft.SetFontColor(PIXEL_OFF);
	textAreaRight.SetFontColor(PIXEL_ON);

	/*
	 * println() was not used above since
	 * the newline processing will clear to the end
	 * of the line with the font color and we don't want that
	 * in this case.
	 */
	textAreaLeft.CursorTo(0,1);
	textAreaRight.CursorTo(0,1);

	textAreaLeft.print("World");
	delay(DELAY/4);
	textAreaRight.print("World");

	delay(DELAY);
	/*
	 * Clear everything
	 */
	GLCD.ClearScreen();

	/*
	 * Must reset the text area cursor position
	 * since although the physical lcd has been cleared
	 * the text areas "remember" where the last text was
	 * written
 	 */
	textAreaLeft.CursorTo(0,0);
	textAreaRight.CursorTo(0,0);
}
