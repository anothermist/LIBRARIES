/*
 * DrawRoundRect - openGLCD API example
 *
 * Example of using:
 * DrawRoundRect()
 *
 * DrawRoundRect(x,y, width, height, radius, [color]);
 *
 * Draw a rectangle the same as DrawRect() but with rounded corners.
 * Radius is a value from 1 to half the smaller of height or width.
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
	 * Draw rectangles on the display every 4 pixels starting
	 * from the outsided and move in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2-8; i +=8)
	{
		GLCD.DrawRoundRect(GLCD.Left + i,GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i, 5);
		delay(200);
	}

	delay(2000);

	GLCD.ClearScreen();

	/*
	 * Draw rectangles on the display every 4 pixels starting
	 * from the outsided and move in.
	 * (Different corner radius from above)
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2-8; i +=8)
	{
		GLCD.DrawRoundRect(GLCD.Left + i,GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i, 15);
		delay(200);
	}
	delay(2000);
}
