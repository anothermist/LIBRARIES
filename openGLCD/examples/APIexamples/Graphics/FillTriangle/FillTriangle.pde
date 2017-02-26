/*
 * FillTriangle - openGLCD API example
 *
 * Example of using:
 * DrawTriagle()
 * FillTriangle()
 *
 * DrawTriangle(x1,y1, x2,y2, x3,y3, [color]);
 * FillTriangle(x1,y1, x2,y2, x3,y3, [color]);
 *
 * Draw/Fill triangle defined by the 3 points
 *
 * Warning:
 *   FillTriangle() currently uses floating point, which will
 *   pull in the floating point support code.

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
	 * Draw a triangle between points: 
	 * - upper left corner
	 * - center of screen
	 * - upper right corner
	 */
	GLCD.DrawTriangle(GLCD.Left,GLCD.Top, GLCD.CenterX,GLCD.CenterY, GLCD.Right,GLCD.Top);

	/*
	 * Draw a triangle between points: 
	 * - lower left corner
	 * - center of screen
	 * - bottom right corner
	 */
	GLCD.DrawTriangle(GLCD.Left,GLCD.Bottom, GLCD.CenterX,GLCD.CenterY, GLCD.Right,GLCD.Bottom);

	delay(2000);


	/*
	 * Fill a triangle between points: 
	 * - upper left corner
	 * - center of screen
	 * - upper right corner
	 */
	GLCD.FillTriangle(GLCD.Left,GLCD.Top, GLCD.CenterX,GLCD.CenterY, GLCD.Right,GLCD.Top);

	/*
	 * Fill a triangle between points: 
	 * - lower left corner
	 * - center of screen
	 * - bottom right corner
	 */
	GLCD.FillTriangle(GLCD.Left,GLCD.Bottom, GLCD.CenterX,GLCD.CenterY, GLCD.Right,GLCD.Bottom);

	delay(4000);
	GLCD.ClearScreen();
}
