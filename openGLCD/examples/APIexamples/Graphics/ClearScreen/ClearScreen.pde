/*
 * ClearScreen - Clear the lcd display
 *
 * Example of using:
 * ClearScreen()
 *
 * ClearScreen([color]);
 *  Sets all the pixels on the display from
 *  GLCD.Left,GLCD.Top to GLCD.Right,GLCD.Bottom to the specified color.
 *
 * Color is optional and defaults to ::PIXEL_OFF
 *
 * Created by Bill Perry 2013-09-07
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 2000 // ms delay between examples

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use SystemFont in "default" text area
}

void loop()
{
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.ClearScreen();
	GLCD.DrawString(F("ClearScreen"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen(); // clear with default color which is all pixels off
	delay(DELAY);

	/*
	 * Now again but show with clearing to PIXEL_ON
	 */
	GLCD.DrawString(F("ClearScreen"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen(PIXEL_ON); // clear with all pixels on

	delay(DELAY);
}
