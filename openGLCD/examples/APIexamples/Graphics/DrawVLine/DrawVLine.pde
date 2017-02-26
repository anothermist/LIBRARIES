/*
 * DrawVLine - openGLCD API example
 *
 * Example of using:
 * DrawVLine()
 *
 * DrawVLine(x, y, height, [color]);
 *
 * Draw a line of height pixels from x,y
 * Resulting line is height pixels.
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
	 * Draw a vertical line every 4 pixels
	 */
	for(uint8_t x = 0; x < GLCD.Width; x+=4)
	{
		GLCD.DrawVLine(x, 0, GLCD.Height-1);
		delay(200);
	}
	delay(2000);
}
