/*
 * DrawBitmap - Draw a glcd bitmap image
 *
 * Example of using:
 * DrawBitmap()
 *
 * DrawBitmap(bitmap, x, y, [color);
 *
 * bitmap a ponter to the bitmap data
 * x X coordinate of the upper left corner of the bitmap
 * y Y coordinate of the upper left corner of the bitmap
 * color
 *
 * Draw a bitmap image with the upper left corner at location x,y
 * The bitmap data is assumed to be in program memory.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * Created by Bill Perry 2013-09-07
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>
#include <bitmaps/BillsHead.h>
#include <bitmaps/ArduinoIcon64x64.h>
#include <bitmaps/ArduinoIcon96x32.h>

#define DELAY 2000 // ms delay between examples

void setup()
{
	GLCD.Init();
}

void loop()
{
	GLCD.ClearScreen();

	/*
	 * Display Arduino icon centered on Display
	 */
	if(GLCD.Height >=64)
		GLCD.DrawBitmap(ArduinoIcon64x64, GLCD.Width/2 - 32, 0);
	else
		GLCD.DrawBitmap(ArduinoIcon96x32, GLCD.Width/2 - 48, 0);

	delay(DELAY);

	GLCD.ClearScreen();

	/*
	 * only if display is big enough
	 * (BillsHead is 54 pixels tall)
	 */
	if(GLCD.Height >= 54)
	{
		/*
		 * Draw "Bills head" images centered on display
		 */

		// left side
		GLCD.DrawBitmap(BillsHead, (GLCD.Width-64)/3, GLCD.CenterY/2 -16);
		// Right side
		GLCD.DrawBitmap(BillsHead, GLCD.Right -32 - ((GLCD.Width-64)/3), GLCD.CenterY/2 -16);

	}
	delay(DELAY);

}
