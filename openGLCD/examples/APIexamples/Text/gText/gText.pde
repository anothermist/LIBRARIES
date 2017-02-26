/*
 * gText - openGLCD API example
 *
 * Example of using:
 * gText()
 *
 * gText();
 * gText(x1, y1, x2, y2, [mode]);
 * gText(selection, [mode])
 * gText(selection, font, [mode])
 * gText(x1, y1, columns, rows, font, [mode]);
 *
 * gText class constructor.
 *
 *        x1 - X coordinate of upper left corner
 *        y1 - Y coordinate of upper left corner
 *        x2 - X coordinate of lower right corner
 *        y2 - Y coordinate of lower right corner
 * selection - a value from predefineArea
 *      font - the font
 *      mode - a value from gTextMode
 *
 * Creates a gText area object and then calls DefineArea() with the same
 * parameters to initialize it.
 *
 *
 * Examples:
 *
 * // create object for full display but no font yet
 * gText textArea;
 *
 * // top half with no font yet
 * gText textArea(textAreaTOP);
 *
 * // bottom half using System5x7 font
 * gText textArea(textAreaBOTTOM, Sytem5x7);
 *
 * // upper left is at x1,y1 and is columnsxrows in sizw
 * // and uses "font" as font.
 * gText textArea(x1, y1, columns, rows, font, [mode]);
 *
 * // upper left is at x1,y1 lower left is at x2,y2
 * gText(x1, y1, x2, y2, [mode]);
 *
 *
 * Created by Bill Perry 2013-09-13
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

/*
 * Create text areas;
 * Note: these text areas overlap. Normally, this should be avoided,
 * especially if using scrolling in the area.
 */
gText textAreaFull; // Creates an area for the full display, but no font yet
gText textAreaBot(textAreaBOTTOM, System5x7); // bottom half of display
gText textAreaNums(GLCD.Left, GLCD.Top, 7, 1, fixednums7x15); // 7 columns, 1 line in upper left corner
gText textAreaUpRt(GLCD.CenterX, 0, GLCD.Right, GLCD.CenterY); // upper right quadrant, no font yet

void setup()
{
	Serial.begin(9600);

	GLCD.Init();
	GLCD.SelectFont(System5x7); // use SystemFont in "default" text area

	/*
	 * assign fonts to text areas that need them
	 */

	textAreaFull.SelectFont(font8x8);

	textAreaUpRt.SelectFont(Arial14);
}

void loop()
{
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.ClearScreen();
	GLCD.DrawString(F("gText"), gTextfmt_center, gTextfmt_center);
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
