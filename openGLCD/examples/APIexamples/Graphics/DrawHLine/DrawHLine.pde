/*
 * DrawHLine - openGLCD API example
 *
 * Example of using:
 * DrawHLine()
 *
 * DrawHLine(x, y, width, [color]);
 *
 * Draw a horizontal line of width pixels starting at x,y
 * Resulting line is width pixels.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

void setup()
{

    GLCD.Init();
}

void loop(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw a Horizontal line every 4 pixels
	 */
	for(uint8_t y = 0; y < GLCD.Height; y+=4)
	{
		GLCD.DrawHLine(0, y, GLCD.Width);
		delay(200);
	}
	delay(2000);
}
