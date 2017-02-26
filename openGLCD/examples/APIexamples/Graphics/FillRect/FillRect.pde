/*
 * FillRect - openGLCD API example
 *
 * Example of using:
 * FillRect()
 *
 * FillRect(x,y, width, height, [color]);
 *
 *  Fill a rectanglular area of the specified width and height.
 *  The resulting rectangle covers an area width pixels wide by
 *  height pixels tall starting from the pixel at x,y.
 *
 *  The upper left corner at x,y and the lower right corner at 
 *  x+width-1,y+height-1.
 *
 *  The length of the horizontal sides will be width pixels
 *  The length of the vertical sides will be height pixels
 *
 * Note: On monochorome displays
 *  BLACK means the same as PIXEL_ON and
 *  WHITE means the same as PIXEL_OFF
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
	GLCD.DrawString("FillRect", gTextfmt_center, gTextfmt_center);
	delay(1000);

	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from top of display
	 * 25 pixels in from each end
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Top+2, GLCD.Width-25*2, 8);
	delay(500);

	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from bottom of display
	 * 25 pixels in from each end
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Bottom-8-2+1, GLCD.Width-25*2, 8);
	delay(500);

	/*
	 * Create a filled rectangle 15x15 pixels in each corner
	 */
	draw4corners(15, PIXEL_ON); // turn on the pixels

	/*
	 * erase a filled rectangle 10x10 pixels in each corner
	 */
	draw4corners(10, PIXEL_OFF); // turn off the pixels

	delay(2000);

	/*
 	 * Now do it all over again in INVERTED mode
	 */
	GLCD.SetDisplayMode(INVERTED); // "INVERTED" display mode
	GLCD.ClearScreen();
	GLCD.DrawString("FillRect", gTextfmt_center, gTextfmt_center);

	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from top of display
	 * 25 pixels in from each end
	 * 
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Top+2, GLCD.Width-25*2, 8);

	delay(500);
	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from bottom of display
	 * 25 pixels in from each end
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Bottom-8-2+1, GLCD.Width-25*2, 8);

	delay(500);

	/*
	 * Create a filled rectangle 15x15 pixels in each corner
	 */
	draw4corners(15, PIXEL_ON); // turn on the pixels

	/*
	 * erase a filled rectangle 10x10 pixels in each corner
	 */
	draw4corners(10, PIXEL_OFF); // turn off the pixels

	delay(2000);
}

/*
 * create a filled in rectangle in each corner
 * that is exactly size x size pixels.
 */
void draw4corners(uint8_t size, uint8_t color)
{
	/*
	 * upper left corner
	 */
	GLCD.FillRect(GLCD.Left, GLCD.Top, size, size, color);
	delay(500);

	/*
	 * upper right corner
	 */
	GLCD.FillRect(GLCD.Right-size+1,GLCD.Top, size, size, color);
	delay(500);

	/*
	 * lower left corner
	 */
	GLCD.FillRect(GLCD.Left,GLCD.Bottom-size+1, size, size, color);
	delay(500);

	/*
	 * Lower right corner
	 */
	GLCD.FillRect(GLCD.Right-size+1,GLCD.Bottom-size+1, size, size, color);
	delay(500);
}
