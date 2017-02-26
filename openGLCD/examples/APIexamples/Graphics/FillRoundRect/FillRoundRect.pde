/*
 * FillRoundRect - openGLCD API example
 *
 * Example of using:
 * FillRoundRect()
 *
 * FillRoundRect(x,y, width, height, radius, [color]);
 *
 * x X coordinate of the upper left corner of the rectangle
 * y Y coordinate of the upper left corner of the rectangle
 * width Width of the rectangle
 * height Height of the rectangle
 * radius Radius of rounded corners
 * color 
 *
 * Fill a rectanglular area with rounded corners 
 * with the same outline as DrawRoundRect().
 * The upper left corner at x,y and lower corner at x+width, y+width.
 * Radius is a value from 1 to half the smaller of height or width.
 *
 * Created by Bill Perry 2013-09-04
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

void setup()
{

    GLCD.Init();
}

void loop(void)
{
uint8_t color;

	GLCD.ClearScreen();

	/*
	 *  Draw filled rectangle on left half of display
 	 * 4 pixels from left, 4 pixels down from top with radius of 10
	 */
	GLCD.FillRoundRect(GLCD.Left+4, GLCD.Top+4, GLCD.Width/2 -2*4, GLCD.Height -2*4, 10);

	/*
	 *  Draw filled rectangle on right half of display
 	 * 4 pixels to right of center, 4 pixels down from top with radius of 10
	 */
	GLCD.FillRoundRect(GLCD.CenterX+4, GLCD.Top+4, GLCD.Width/2 -2*4, GLCD.Height -2*4, 10);

	delay(1500);

	GLCD.ClearScreen();

	/*
	 * Draw Filled Rounded rectangles on the display every 8 pixels starting
	 * from the outside and move in alternating colors;
	 */
	color = PIXEL_ON;
	for(uint8_t i = 0; i < GLCD.Bottom/2-8; i +=8)
	{
		GLCD.FillRoundRect(GLCD.Left + i,GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i, 10,  color);

		if(color == PIXEL_ON)
			color = PIXEL_OFF;
		else
			color=PIXEL_ON;

		delay(500);
	}

	delay(1500);

	GLCD.ClearScreen();

	/*
	 * Draw Filled Rounded rectangles on the display every 8 pixels starting
	 * from the outside and move in alternating colors;
	 * (Different corner radius from above)
	 */
	color = PIXEL_ON;
	for(uint8_t i = 0; i < GLCD.Bottom/2-8; i +=8)
	{
		GLCD.FillRoundRect(GLCD.Left + i,GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i, 15, color);

		if(color == PIXEL_ON)
			color = PIXEL_OFF;
		else
			color=PIXEL_ON;
		delay(500);
	}

	
	delay(2000);
}
