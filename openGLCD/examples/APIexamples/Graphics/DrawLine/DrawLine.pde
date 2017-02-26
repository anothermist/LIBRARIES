/*
 * DrawLine - openGLCD API example
 *
 * Example of using:
 * DrawLine()
 *
 * DrawLine(x1,y1, x2,y2, [color]);
 *
 * Draw a line from x1,y1 to x2,y2
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
	 * Draw lines from center connecting to the 8th pixels down the left side
	 */
	for(uint8_t y = 0; y < GLCD.Height; y+=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, GLCD.Left,y);
		delay(150);
	}
	/*
	 * Draw lines from center connecting to the 8th pixels along bottom
	 */
	for(uint8_t x = 0; x < GLCD.Width; x+=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, x,GLCD.Bottom);
		delay(150);
	}

	/*
	 * Draw lines from center connecting to the 8th pixels up the right side
	 */
	for(int y = GLCD.Bottom; y > 0; y -=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, GLCD.Right,y);
		delay(150);
	}

	/*
	 * Draw lines from center connecting to the 8th pixels along top
	 */
	for(uint8_t x = GLCD.Right; x > 7; x -=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, x,GLCD.Top);
		delay(150);
	}
	
	delay(2000);
}
