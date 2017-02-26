/*
 * ClearAreaMode - openGLCD API example
 *
 * Example of using:
 * ClearAreaMode()
 * SetAreaMode()
 *
 * ClearArea(mode)
 * SetAreaMode(mode)
 *   mode     - a value from gTextMode
 *
 *  Set/Clear a text area mode option
 *
 * Examples:
 *  textarea.SetAreaMode(gTextMode_SCROLLDOWN);   // Enable reverse vertical scrolling
 *  textarea.ClearAreaMode(gTextMode_SCROLLDOWN); // Disable reverse vertical scrolling

 *  textarea.SetAreaMode(gTextMode_OVERSTRIKE);   // Enable text overstrike mode
 *  textarea.ClearAreaMode(gTextMode_OVERSTRIKE); // Disable text overstrike mode
 *
 *
 * Created by Bill Perry 2013-09-15
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

/*
 * Create a text area for each of the 4 quadrants
 */
gText areaLeft(textAreaLEFT, System5x7);
gText areaRight(textAreaRIGHT, System5x7);

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use system in "default" text area
}

void loop()
{
	GLCD.ClearScreen();

	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("ClearAreaMode"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);

	/*
	 * output lines to show scrolling on default text area
	 */

	GLCD.ClearAreaMode(gTextMode_SCROLLDOWN);
	GLCD.ClearArea();

	for(int line = 0; line < 10; line++)
	{
		GLCD.print("Line ");
		GLCD.println(line);
		delay(DELAY/4);
	}
	delay(DELAY/2);

	/*
	 * Now do it again in reverse scrolling
	 */
	GLCD.SetAreaMode(gTextMode_SCROLLDOWN);
	GLCD.ClearArea();

	for(int line = 0; line < 10; line++)
	{
		GLCD.print("Line ");
		GLCD.println(line);
		delay(DELAY/4);
	}

	delay(DELAY/2);

	/*
	 * Now do left and right at same time:
	 */
	areaLeft.ClearAreaMode(gTextMode_SCROLLDOWN);
	areaRight.SetAreaMode(gTextMode_SCROLLDOWN);
	areaLeft.ClearArea();
	areaRight.ClearArea();
	for(int line = 0; line < 10; line++)
	{
		areaLeft.print("Line ");
		areaRight.print("Line ");
		areaLeft.println(line);
		areaRight.println(line);
		delay(DELAY/4);
	}
	areaRight.ClearAreaMode(gTextMode_SCROLLDOWN);
	delay(DELAY/2);

	/*
	 * Now show overstrike on right half
	 * Overstrike causes the glyph pixels to write on top
	 * of any existing pixels. i.e. the background behind
	 * the glyph pixels is not erased.
	 * (use a bigger font for visibility)
	 */

	areaRight.SelectFont(fixed_bold10x15);
	areaLeft.SelectFont(fixed_bold10x15);
	areaRight.SetAreaMode(gTextMode_OVERSTRIKE);
	areaLeft.ClearArea();
	areaRight.ClearArea();
	slowprint(areaLeft, "hello");
	slowprint(areaRight, "hello");
	areaLeft.CursorTo(0,0);
	areaRight.CursorTo(0,0);
	delay(DELAY/4);
	slowprint(areaLeft,  "-----");
	slowprint(areaRight, "-----");

	// turn overstrike back off
	areaRight.ClearAreaMode(gTextMode_OVERSTRIKE);
	// put the font back
	areaRight.SelectFont(System5x7);
	areaLeft.SelectFont(System5x7);
	
	delay(DELAY);
}

void slowprint(gText &area, const char *str)
{
	while(*(str))
	{
		area.print(*str);
		delay(200);
		str++;
	}
}
