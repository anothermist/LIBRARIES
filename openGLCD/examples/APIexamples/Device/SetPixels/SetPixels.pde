/*
 * SetPixels - openGLCD API example
 *
 * Example of using:
 * SetPixels()
 *
 * SetPixels(x1,y1, x2,y2, color);
 *
 * sets the color of pixels in the area bounded by the two coordinates.
 * x1,y1 is the upper left corner and x2,y2 is the lower left corner
 *
 * Note: On monochorome displays
 *  PIXEL_ON or BLACK means "ON" and PIXEL_OFF or WHITE means "OFF".
 *
 * When display mode is INVERTED, color meanings reverse.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

void setup()
{
	GLCD.Init(); // default to "normal" (non inverted) mode
	GLCD.SelectFont(System5x7);
}


void loop(void)
{
	GLCD.SetDisplayMode(NON_INVERTED); // "normal" display mode
	GLCD.ClearScreen();
	GLCD.DrawString("SetPixels", gTextfmt_center, gTextfmt_center);
	delay(1000);

	/*
	 * turn on pixels in a rectangle 
	 * 8 pixels in height 2 pixels from top of display
	 * 25 pixels in from each end
	 */
	GLCD.SetPixels(GLCD.Left+25, GLCD.Top+2, GLCD.Right-25, GLCD.Top+2+8-1, PIXEL_ON);
	delay(500);

	/*
	 * turn on pixels in a rectangle 
	 * 8 pixels in height 2 pixels from bottom of display
	 * 25 pixels in from each end
	 */
	GLCD.SetPixels(GLCD.Left+25, GLCD.Bottom-8-2+1, GLCD.Right-25, GLCD.Bottom-2, PIXEL_ON);
	delay(500);

	/*
	 * turn on pixels in 15x15 pixel rectangle in each corner
	 */
	draw4corners(15, PIXEL_ON); // turn on the pixels

	/*
	 * turn off pixels in 10x10 pixel rectangle in each corner
	 */
	draw4corners(10, PIXEL_OFF); // turn off the pixels

	delay(2000);

	/*
 	 * Now do it all over again in INVERTED mode
	 */
	GLCD.SetDisplayMode(INVERTED); // "INVERTED" display mode
	GLCD.ClearScreen();
	GLCD.DrawString("SetPixels", gTextfmt_center, gTextfmt_center);

	/*
	 * turn on pixels in a rectangle 
	 * 8 pixels in height 2 pixels from top of display
	 * 25 pixels in from each end
	 */
	GLCD.SetPixels(GLCD.Left+25, GLCD.Top+2, GLCD.Right-25, GLCD.Top+2+8-1, PIXEL_ON);
	delay(500);

	/*
	 * turn on pixels in a rectangle 
	 * 8 pixels in height 2 pixels from bottom of display
	 * 25 pixels in from each end
	 */
	GLCD.SetPixels(GLCD.Left+25, GLCD.Bottom-8-2+1, GLCD.Right-25, GLCD.Bottom-2, PIXEL_ON);
	delay(500);

	/*
	 * turn on pixels in 15x15 pixel rectangle in each corner
	 */
	draw4corners(15, PIXEL_ON); // turn on the pixels

	/*
	 * turn off pixels in 10x10 pixel rectangle in each corner
	 */
	draw4corners(10, PIXEL_OFF); // turn off the pixels

	delay(2000);
}

/*
 * create a filled in box in each corner that
 * is exactly size x size pixels.
 */
void draw4corners(uint8_t size, uint8_t color)
{
	/*
	 * upper left corner
	 */
	GLCD.SetPixels(GLCD.Left, GLCD.Top, GLCD.Left+size-1,GLCD.Top+size-1, color);
	delay(500);

	/*
	 * upper right corner
	 */
	GLCD.SetPixels(GLCD.Right-size+1,GLCD.Top, GLCD.Right,GLCD.Top+size-1, color);
	delay(500);

	/*
	 * lower left corner
	 */
	GLCD.SetPixels(GLCD.Left,GLCD.Bottom-size+1, GLCD.Left+size-1,GLCD.Bottom, color);
	delay(500);

	/*
	 * Lower right corner
	 */
	GLCD.SetPixels(GLCD.Right-size+1,GLCD.Bottom-size+1, GLCD.Right,GLCD.Bottom, color);
	delay(500);
}
