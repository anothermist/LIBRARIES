/*
 * DrawBitmapXBM - Draw a glcd bitmap image
 *
 * Example of using:
 * DrawBitmapXBM()
 *
 * DrawBitmapXBM(bitmap, x, y, [color]);
 *
 * bitmap a ponter to the XBM bitmap data
 * x X coordinate of the upper left corner of the bitmap
 * y Y coordinate of the upper left corner of the bitmap
 * color
 *
 * Draw a XBM bitmap image with the upper left corner at location x,y
 * The bitmap data is assumed to be in program memory.
 *
 * color is an optional parameter indicating pixel color, default is ::PIXEL_ON
 *
 * Created by Bill Perry 2013-09-07
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>
#include <bitmaps/car914_xbm.h>

#define DELAY 2000 // ms delay between examples

void setup()
{
	GLCD.Init();
}

void loop()
{
	GLCD.ClearScreen();

	/*
	 * Display bitmap centered on Display
	 */
	GLCD.DrawBitmapXBM(car914_xbm, GLCD.CenterX - car914_width/2,GLCD.Top);
	/*
 	 * If enough room draw another one along left edge
	 */
	if(GLCD.Height >=64)
		GLCD.DrawBitmapXBM(car914_xbm, GLCD.Left, GLCD.CenterY); 

	/*
	 * Now do it again, but inverted
	 */
	delay(2000);

	/*
	 * Display bitmap centered on Display
	 */
	GLCD.DrawBitmapXBM(car914_xbm, GLCD.CenterX - car914_width/2,GLCD.Top, PIXEL_OFF);
	/*
 	 * If enough room draw another one along left edge
	 */
	if(GLCD.Height >=64)
		GLCD.DrawBitmapXBM(car914_xbm, GLCD.Left, GLCD.CenterY, PIXEL_OFF); 


	delay(DELAY);
}
