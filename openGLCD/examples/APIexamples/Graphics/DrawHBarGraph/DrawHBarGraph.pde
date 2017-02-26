/*
 * DrawHBarGraph - openGLCD API example
 *
 * Example of using:
 * DrawVBarHraph()
 *
 * DrawHBarGraph(x, y, width, height, border, minval, maxval, curval);
 *   x X coordinate of the corner of the bargraph
 *   y Y coordinate of the corner of the bargraph
 *   width Width in pixels of bargraph, including border pixels
 *   height Height in pixels of bargraph, including border pixels
 *   border Border pixels around bargraph (use zero for no border)
 *   minval Minimum value of the bargraph
 *   maxval Maximum value of the bargraph
 *   curval Current value of the bargraph
 *
 * Draw a horizontal bargraph.
 *
 * If width is positive, the graph will be drawn to the right of the x coordinate
 * and the graph will advance from left to right as curval increases towards maxval.
 * If width is negative, the graph will be drawn to the left of the x coordinate
 * and the graph will advance from right to left as curval increases towards maxval.
 * If height is positive, the graph will be drawn below the y coordinate.
 * If height is negative, the graph will be drawn above the y coordinate.
 *
 * Note:
 * While the behavior of height may at first seem backwards, it is defined with
 * respect to the coordinate system.
 * The y value in the coordinate system increases as the pixel location moves down
 * on the display.
 *
 * Created by Bill Perry 2013-08-29
 * bperrybap@opensource.billsworld.billandterrie.com
 */


#include <openGLCD.h>

#define BARMAXVAL 100
	
void setup()
{
	GLCD.Init();
	GLCD.ClearScreen(); 
	GLCD.SelectFont(fixednums8x16);
}

void loop()
{
	for(int num = 0; num <= BARMAXVAL; num++)
	{
		/*
		 * print number with 0 fill
		 */

		// subtract 2.5 times font width from CenterX
		GLCD.CursorToXY(GLCD.CenterX-12, 0);
		if(num < 100)
			GLCD.print('0');
		if(num < 10)
			GLCD.print('0');

		GLCD.print(num);

		/*
		 * left to right bar graphs
		 */
		
		/*
		 * Draw Horizontal bar graph:
		 * 10 pixels from left edge of display 
		 * 15 pixels from top
		 * (GLCD.Width -20) pixels wide advancing to right of x
		 * 5 pixels in height droping down from y
		 * no border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawHBarGraph(GLCD.Left+10, GLCD.Top+15, GLCD.Width-2*10, 5, 0, 0, BARMAXVAL, num);

		/*
		 * Draw Horizontal bar graph:
		 * 10 pixels from left edge of display 
		 * 21 pixels from top
		 * (GLCD.Width -20) pixels wide advancing to right of x
		 * 5 pixels in height droping down from y
		 * 1 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawHBarGraph(GLCD.Left+10, GLCD.Top+21, GLCD.Width-2*10, 5, 1, 0, BARMAXVAL, num);

		/*
		 * Draw Horizontal bar graph:
		 * 10 pixels from left edge of display 
		 * 27 pixels from top
		 * (GLCD.Width -20) pixels wide advancing to right of x
		 * 5 pixels in height droping down from y
		 * 2 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawHBarGraph(GLCD.Left+10, GLCD.Top+27, GLCD.Width-2*10, 5, 2, 0, BARMAXVAL, num);


	/*
	 * on smaller displays
	 * break up into two seperate loops
	 */
#if (DISPLAY_HEIGHT < 64)
		delay(150);
	}
	delay(1000);
	GLCD.ClearScreen();
	for(int num = 0; num <= BARMAXVAL; num++)
	{
		/*
		 * print number with 0 fill
		 */

		// subtract 2.5 times font width from CenterX
		GLCD.CursorToXY(GLCD.CenterX-12, 0);
		if(num < 100)
			GLCD.print('0');
		if(num < 10)
			GLCD.print('0');

		GLCD.print(num);
#endif

		/*
		 * right to left bar graphs
		 */

		/*
		 * Draw Horizontal bar graph:
		 * 10 pixels from right edge of display 
		 * along bottom
		 * (GLCD.Width -20) pixels wide advancing to left of x
		 * 5 pixels in height rising up from y
		 * no border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawHBarGraph(GLCD.Right-10, GLCD.Bottom, -(GLCD.Width-2*10), -5, 0, 0, BARMAXVAL, num);

		/*
		 * Draw Horizontal bar graph:
		 * 10 pixels from right edge of display 
		 * 6 pixels from bottom
		 * (GLCD.Width -20) pixels wide advancing to left of x
		 * 5 pixels in height rising up from y
		 * 1 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawHBarGraph(GLCD.Right-10, GLCD.Bottom-6, -(GLCD.Width-2*10), -5, 1, 0, BARMAXVAL, num);

		/*
		 * Draw Horizontal bar graph:
		 * 10 pixels from right edge of display 
		 * 6 pixels from bottom
		 * (GLCD.Width -20) pixels wide advancing to left of x
		 * 5 pixels in height rising up from y
		 * 2 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawHBarGraph(GLCD.Right-10, GLCD.Bottom-12, -(GLCD.Width-20), -5, 2, 0, BARMAXVAL, num);

		delay(150);
	}

	delay(2000);
	GLCD.ClearScreen();
}
