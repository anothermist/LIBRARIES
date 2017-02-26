/*
 * DefineArea - openGLCD API example
 *
 * Example of using:
 * DefineArea()
 *
 * DefineArea(selection, [mode])
 * DefineArea(selection, font, [mode])
 * DefineArea(x1, y1, x2, y2, [mode]);
 * DefineArea(x1, y1, columns, rows, font, [mode]);
 *
 * Defines a text area based on the supplied parameters.
 *
 * DefineArea(selection, [mode])
 *  selection - a value from predefineArea
 *       mode - a value from gTextMode
 *
 * Define a text area using a selection form a set of predefined areas.
 * The area within the newly defined text area is intentionally not cleared.
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * DefineArea(selection, font, [mode])
 *  selection - a value from predefineArea
 *       font - the font
 *
 * Define a text area using a selection from a set of predefined areas.
 * The area within the newly defined text area is intentionally not cleared.
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * DefineArea(x1, y1, x2, y2, [mode]);
 *        x1 - X coordinate of upper left corner
 *        y1 - Y coordinate of upper left corner
 *        x2 - X coordinate of lower right corner
 *        y2 - Y coordinate of lower right corner
 *      mode - a value from gTextMode
 *
 * Define a text area based on absolute coordinates.
 * The pixel coordinates for the text area are inclusive so x2,y2 is the
 * lower right pixel of the text area.
 * x1,y1 and x2,y2 are an absolute coordinates and are relateive to the
 * 0,0 origin of the display.
 * The area within the newly defined text area is intentionally not cleared.
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * DefineArea(x1, y1, columns, rows, font, [mode]);
 *        x1 - X coordinate of upper left corner
 *        y1 - Y coordinate of upper left corner
 *      font - the font
 *      mode - a value from gTextMode
 *
 * Define a text area sized to hold columns characters across and rows
 * characters tall.
 * It is properly sized for the specified font.
 * The area within the newly defined text area is intentionally not cleared.
 * While intended for fixed width fonts, sizing will work for variable
 * width fonts.
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 * x,y is an absolute coordinate and is relateive to the 0,0 origin of the
 * display.
 * mode is an optional parameter and defaults to normal/up scrolling
 *
 * Returns:
 *    0 with the desired area defined if all the coordinates are valid,
 *    otherwise returns non zero error code with the area set to
 *    the full display
 *
 * Examples:
 *
 * // top half without setting or changing font
 * textarea.DefineArea(textAreaTOP);
 *
 * // bottom half using System5x7 font
 * textarea.DefineArea(textAreaBOTTOM, Sytem5x7);
 *
 * // upper left is at x1,y1 and is columnsxrows in size
 * // and uses "font" as font.
 * textarea.DefineArea(x1, y1, columns, rows, font);
 *
 * // upper left is at x1,y1 lower left is at x2,y2
 * textarea.DefineArea(x1, y1, x2, y2);
 *
 * Created by Bill Perry 2013-09-15
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

/*
 * Create text areas;
 * Note: these text areas overlap. Normally, this should be avoided,
 * especially if using scrolling in the area.
 */

gText textAreaFull;
gText textAreaBot;
gText textAreaNums;
gText textAreaUpRt;

void setup()
{
	Serial.begin(9600);

	GLCD.Init();
	GLCD.SelectFont(System5x7); // use SystemFont in "default" text area

	// full display using font8x8 font
	textAreaFull.DefineArea(textAreaFULL, font8x8);

	// bottom half of display reverse scrolling
	textAreaBot.DefineArea(textAreaBOTTOM, System5x7, gTextMode_SCROLLDOWN);

 	// 8 columns, 1 line in upper left corner
	textAreaNums.DefineArea(GLCD.Left, GLCD.Top, 8, 1, fixednums7x15);

 	// upper right quadrant, no font yet
	textAreaUpRt.DefineArea(GLCD.CenterX, 0, GLCD.Right, GLCD.CenterY);
	// now set font
	textAreaUpRt.SelectFont(Arial14);

}

void loop()
{
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.ClearScreen();
	GLCD.DrawString(F("DefineArea"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
	 * Print to the full text area
	 */
	for(int x = 0; x < 10; x++)
	{
		textAreaFull.print("Line ");
		textAreaFull.println(x);
		delay(DELAY/4);
	}
	delay(DELAY/2);
	textAreaFull.ClearArea();

	/*
	 * Print to the Bottom area & the Nums area
	 * (Those areas do not overlap)
	 */
	for(int x = 0; x < 10; x++)
	{
		textAreaBot.print("Line ");
		textAreaBot.println(x);
		delay(DELAY/4);

		textAreaNums.println(x);
		delay(DELAY/4);

		textAreaUpRt.print("Line ");
		textAreaUpRt.println(x);
		delay(DELAY/4);
	}
	delay(DELAY/2);
	/*
	 * Clear out the areas
	 */
	textAreaBot.ClearArea();
	delay(DELAY/4);
	textAreaNums.ClearArea();
	delay(DELAY/4);
	textAreaUpRt.ClearArea();
}
