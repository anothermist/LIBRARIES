/*
 * InvertRect - openGLCD API example
 *
 * Example of using:
 * InvertRect()
 *
 * InvertRect(x,y, width, height);
 *
 *   x X coordinate of the upper left corner of the rectangle
 *   y Y coordinate of the upper left corner of the rectangle
 *   width Width in pixels of the rectangular area
 *   height Height in pixels of the rectangular area
 *
 * Invert the pixels in the rectanglular area of
 * the specified width and height.
 * BLACK pixels become WHITE and WHITE pixels become BLACK.
 *
 * The upper left corner at x,y and the lower right
 * corner at x+width-1,y+width-1.
 *
 * The length of the horizontal sides will be width pixels
 * The length of the vertical sides will be height pixels
 *
 * Created by Bill Perry 2013-09-01
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
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawRect(GLCD.Left+i, GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i);
		delay(200);
	}

	delay(1000);
	/*
	 * invert the entire display
	 */

	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height);
	delay(750);
	/*
	 * invert the entire display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height);
	delay(1000);

	/*
	 * invert the left side of display
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(750);

	/*
	 * invert the left side of display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(1000);

	/*
	 * invert the right side of display
	 */
	GLCD.InvertRect(GLCD.CenterX, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(750);

	/*
	 * invert the right side of display to put it back
	 */
	GLCD.InvertRect(GLCD.CenterX, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(1000);

	/*
	 * invert the top half of display
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height/2);
	delay(750);

	/*
	 * invert the top half of display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height/2);
	delay(1000);

	/*
	 * invert the Bottom half of display
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.CenterY, GLCD.Width, GLCD.Height/2);
	delay(750);

	/*
	 * invert the Bottom half of display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.CenterY, GLCD.Width, GLCD.Height/2);
	delay(1000);
}
