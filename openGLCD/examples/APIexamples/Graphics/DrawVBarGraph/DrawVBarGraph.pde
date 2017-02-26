/*
 * DrawVBarGraph - openGLCD API example
 *
 * Example of using:
 * DrawVBarGraph()
 *
 * DrawVBarGraph(x, y, width, height, border, minval, maxval, curval);
 *   x X coordinate of the corner of the bargraph
 *   y Y coordinate of the corner of the bargraph
 *   width Width in pixels of bargraph, including border pixels
 *   height Height in pixels of bargraph, including border pixels
 *   border Border pixels around bargraph (use zero for no border)
 *   minval Minimum value of the bargraph
 *   maxval Maximum value of the bargraph
 *   curval Current value of the bargraph
 *
 * Draw a vertical bargraph.
 *
 * If width is positive, the graph will be drawn to the right of the x coordinate
 * If width is negative, the graph will be drawn to the left of the x coordinate
 * If height is positive, the graph will be drawn below the y coordinate and
 *  and the graph will advance down from the top as curval increases towards maxval.
 * If height is negative, the graph will be drawn above the y coordinate and
 * and the graph will advance up from the bottom as curval increases towards maxval.
 *
 * Note:
 * While the behavior of height may at first seem backwards, it is defined with
 * respect to the coordinate system.\n
 * The y value in the coordinate system increases as the pixel location moves down
 * on the display.
 *
 * Created by Bill Perry 2013-08-29
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define MAXVAL 100
	
void setup()
{
	GLCD.Init();
	GLCD.ClearScreen(); 
	GLCD.SelectFont(fixednums8x16);
}

void loop()
{
int num = 0;

	while(num <= MAXVAL)
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
		 * Draw vertical bar graph:
		 * left edge of display 
		 * 4 pixels up from bottom
		 * 5 pixels wide to right of x,
		 * (HEIGHT - 8) pixels in height advancing up from y
		 * no border,
		 * with a range of 0 to MAXVAL
		 */

		GLCD.DrawVBarGraph(GLCD.Left, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 0, 0, MAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 10 pixels from left edge of display 
		 * 4 pixels up from bottom
		 * 5 pixels wide to right of x,
		 * (HEIGHT - 8) pixels in height advancing up from y
		 * 1 pixel border,
		 * with a range of 0 to MAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Left+10, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 1, 0, MAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 20 pixels from left edge of display 
		 * 4 pixels up from bottom
		 * 5 pixels wide to right of x,
		 * (HEIGHT - 8) pixels in height advancing up from y
		 * 2 pixel border,
		 * with a range of 0 to MAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Left+20, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 2, 0, MAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * right edge of display 
		 * 4 pixels down from top
		 * 5 pixels wide to left of x,
		 * (HEIGHT - 8) pixels in height advancing down from y
		 * 0 pixel border,
		 * with a range of 0 to MAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Right, GLCD.Top+4, -5, (GLCD.Height-2*4), 0, 0, MAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 10 pixels from right edge of display 
		 * 4 pixels down from top
		 * 5 pixels wide to left of x,
		 * (HEIGHT - 8) pixels in height advancing down from y
		 * 1 pixel border,
		 * with a range of 0 to MAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Right-10, GLCD.Top+4, -5, (GLCD.Height-2*4), 1, 0, MAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 20 pixels from right edge of display 
		 * 4 pixels down from top
		 * 5 pixels wide to left of x,
		 * (HEIGHT - 8) pixels in height advancing down from y
		 * 2 pixel border,
		 * with a range of 0 to MAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Right-20, GLCD.Top+4, -5, (GLCD.Height-2*4), 2, 0, MAXVAL, num);

		num++;
		delay(150);
	}
	delay(2000);
}
