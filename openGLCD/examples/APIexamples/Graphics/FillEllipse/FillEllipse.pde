/*
 * DrawEllipse - openGLCD API example
 *
 * Example of using:
 * DrawEllipse()
 * FillEllipse()
 *
 * DrawEllipse(xCenter, yCenter, xRadius, yRadius, [color]);
 * FillEllipse(xCenter, yCenter, xRadius, yRadius, [color]);
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
	/*
	 * Draw ellipse in center of display with xrad=32, yrad=14
	 */

	GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, 32, 14);
	delay(2000);
	GLCD.ClearScreen();

	/*
	 * Draw filled ellipse in center of display with xrad=32, yrad=14
	 */

	GLCD.FillEllipse(GLCD.CenterX, GLCD.CenterY, 32, 14);
	delay(2000);
	GLCD.ClearScreen();

	/*
	 * Draw ellipse with equal x an y rad (circle)
	 * Note: DrawCircle() is more efficient
	 */
	GLCD.FillEllipse(GLCD.CenterX, GLCD.CenterY, 10, 10);
	delay(2000);
	GLCD.ClearScreen();

	/*
 	 * Draw an ellipse that fills the display in center
	 */
	GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Width/2-1, GLCD.Height/2-1);

	/*
	 * A little animation for eye candy
	 */
	
	/*
	 * Fill pixels from inside out
	 */
	for(int x = 1; x < GLCD.Height/2; x++)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Width/2-x, GLCD.Height/2-x);
		delay(30);
	}
	/*
	 * remove pixels from inside out
	 */
	for(int x = GLCD.Height/2; x > 0; x--)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Width/2-x, GLCD.Height/2-x, PIXEL_OFF);
		delay(30);
	}

	/*
	 * Fill pixels from inside out (as a circle)
	 */
	for(int x = 1; x < GLCD.Height/2 -1; x++)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, x);
		delay(20);
	}
	/*
	 * remove pixels from inside out (as a circle)
	 */
	for(int x = GLCD.Height/2 -1; x > 1; x--)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-x, GLCD.Height/2-x, PIXEL_OFF);
		delay(20);
	}

	/*
	 * fill pixels from inside out starting as circle then expanding to ellipse
	 */
	for(int x = 1; x < GLCD.Width/2 -1; x++)
	{
		if(x >= GLCD.Height/2)
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, GLCD.Height/2-1);
		else
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, x);
		delay(20);
	}
	/*
	 * erase pixels from inside out starting as circle then expanding to ellipse
	 */
	for(int x = 1; x < GLCD.Width/2 -1; x++)
	{
		if(x >= GLCD.Height/2)
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, GLCD.Height/2-1,PIXEL_OFF);
		else
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, x,PIXEL_OFF);
		delay(20);
	}

	delay(1000);
	GLCD.ClearScreen();
}
