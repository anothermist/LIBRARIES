/*
 * DrawRect - openGLCD API example
 *
 * Example of using:
 * DrawRect()
 *
 * DrawRect(x,y, width, height, [color]);
 *
 * Draw a rectangle with the specified width and height.
 * The upper left corner at x,y and lower cornder at x+width-1, y+width-1.
 * The length of the horizontal sides will be width pixels.
 * The length of the vertical sides will height pixels.
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
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawRect(GLCD.Left+i, GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i);
		delay(200);
	}
	delay(2000);
}
