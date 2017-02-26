/*
 * FillCircle - openGLCD API example
 *
 * Example of using:
 * FillCircle()
 * DrawCircle()
 *
 * FillCircle(xCenter, yCenter, Radius, [color]);
 * DrawCircle(xCenter, yCenter, Radius, [color]);
 *
 * Created by Bill Perry 2013-08-11
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
	 * Draw circle in center of display
	 */

	GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-1);
	delay(2000);

	/*
	 * Draw filled circle in center of display
	 */

	GLCD.FillCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-1);
	delay(2000);

	/*
	 * A little animation for eye candy
	 */

	GLCD.ClearScreen();

	/*
	 * Fill pixels from outside in
	 */
	for(int r = 1; r < GLCD.Height/2; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-r);
		delay(30);
	}

	/*
	 * Remove pixels from inside out
	 */
	for(int r = GLCD.Height/2 -1; r > 0; r--)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-r, PIXEL_OFF);
		delay(30);
	}

	/*
	 * Fill pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r);
		delay(20);
	}

	/*
	 * remove pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r, PIXEL_OFF);
		delay(20);
	}

	/*
	 * Fill pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r);
		delay(20);
	}

	/*
	 * remove pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r, WHITE);
		delay(20);
	}

	delay(1000);
	GLCD.ClearScreen();
}
