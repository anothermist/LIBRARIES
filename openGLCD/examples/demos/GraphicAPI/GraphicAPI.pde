/*
 * GraphicAPI - Demonstrate graphic API functions
 *
 * This sketch demonstrates the graphics API functions.
 * It is a demo of all the graphic APIexamples
 *
 * 2013-09-04 Bill Perry  - original creation
 *	bperrybap@opensource.billsworld.billandterrie.com
 */

#include "openGLCD.h"

#define BARMAXVAL 100
#define DELAY 900 // delay between "demos"
#define SDELAY DELAY/6

void setup()
{
	GLCD.Init();
	IntroScreen();
	delay(DELAY*3);
}
void IntroScreen()
{
  GLCD.SelectFont(Arial14);
  GLCD.DrawString(GLCD_GLCDLIB_NAMESTR, gTextfmt_center, 3);
  GLCD.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, GLCD.CharHeight(0) + 2);
  GLCD.DrawRoundRect(0+10,0, GLCD.Right-20, GLCD.CharHeight(0) *2 + 1, 5);  // rounded rectangle around text area   
}

void loop()
{
	GLCD.ClearScreen();
  	GLCD.SelectFont(Arial14);
	GLCD.DrawString("Graphics Demo", gTextfmt_center, gTextfmt_center);
	delay(DELAY*3);
	DrawVLine();
	DrawHLine();
	DrawLine();
	DrawRect();
	InvertRect();

	DrawTriangle();
	DrawRoundRect();
	DrawCircle();
	DrawEllipse();

	FillRect();
	FillRoundRect();
	FillTriangle();
	FillCircle();
	FillEllipse();

	DrawHBarGraph();
	DrawVBarGraph();
}


/*
 * Graphic API demo functions
 */


void DrawCircle(void)
{
	GLCD.ClearScreen();
	/*
	 * Draw Circles on the display shrinking every 4 pixels starting
	 * from the outsided and moving in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawCircle(GLCD.CenterX,GLCD.CenterY, (GLCD.Bottom - 2*i)/2);
		delay(SDELAY);
	}
	delay(DELAY);
}

void DrawEllipse(void)
{
	GLCD.ClearScreen();
	/*
	 * Draw Ellipses on the display shrinking every 4 pixels starting
	 * from the outsided and moving in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawEllipse(GLCD.CenterX,GLCD.CenterY, (GLCD.Right - 2*i)/2, (GLCD.Bottom - 2*i)/2);
		delay(SDELAY);
	}
	delay(DELAY);
}

void DrawHBarGraph()
{
	GLCD.ClearScreen(); 
	GLCD.SelectFont(fixednums8x16);
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
		delay(DELAY/30);
	}
	delay(DELAY);
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

		delay(DELAY/30);
	}

	delay(DELAY);
}
void DrawHLine(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw a Horizontal line every 4 pixels
	 */
	for(uint8_t y = 0; y < GLCD.Height; y+=4)
	{
		GLCD.DrawHLine(0, y, GLCD.Width);
		delay(SDELAY/2);
	}
	delay(DELAY);
}

void DrawLine(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw lines from center connecting to the 8th pixels down the left side
	 */
	for(uint8_t y = 0; y < GLCD.Height; y+=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, GLCD.Left,y);
		delay(SDELAY/2);
	}
	/*
	 * Draw lines from center connecting to the 8th pixels along bottom
	 */
	for(uint8_t x = 0; x < GLCD.Width; x+=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, x,GLCD.Bottom);
		delay(SDELAY/2);
	}

	/*
	 * Draw lines from center connecting to the 8th pixels up the right side
	 */
	for(int y = GLCD.Bottom; y > 0; y -=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, GLCD.Right,y);
		delay(SDELAY/2);
	}

	/*
	 * Draw lines from center connecting to the 8th pixels along top
	 */
	for(uint8_t x = GLCD.Right; x > 7; x -=8)
	{
		GLCD.DrawLine(GLCD.CenterX,GLCD.CenterY, x,GLCD.Top);
		delay(SDELAY/2);
	}
	
	delay(DELAY);
}
void DrawRect(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw rectangles on the display every 4 pixels starting
	 * from the outsided and move in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawRect(GLCD.Left+i, GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i);
		delay(SDELAY);
	}
	delay(DELAY);
}
void DrawRoundRect(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw rectangles on the display every 4 pixels starting
	 * from the outsided and move in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2-8; i +=8)
	{
		GLCD.DrawRoundRect(GLCD.Left + i,GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i, 5);
		delay(SDELAY);
	}

	delay(2000);

	GLCD.ClearScreen();

	/*
	 * Draw rectangles on the display every 4 pixels starting
	 * from the outsided and move in.
	 * (Different corner radius from above)
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2-8; i +=8)
	{
		GLCD.DrawRoundRect(GLCD.Left + i,GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i, 15);
		delay(SDELAY);
	}
	delay(DELAY);
}

void DrawTriangle(void)
{
	GLCD.ClearScreen();
	/*
	 * Draw shrinking Triangles on the display starting
	 * on the outside and moving in.
	 */

	for(uint8_t i=0; i< GLCD.Bottom/2; i +=8)
	{
	    GLCD.DrawTriangle(GLCD.Left+2*i+1, GLCD.Bottom-i, GLCD.CenterX, GLCD.Top+i, GLCD.Right-2*i, GLCD.Bottom-i);
	    delay(SDELAY);
	}

	delay(DELAY);
}

void DrawVBarGraph()
{
int num = 0;

	GLCD.ClearScreen(); 
	GLCD.SelectFont(fixednums8x16);

	while(num <= BARMAXVAL)
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
		 * with a range of 0 to BARMAXVAL
		 */

		GLCD.DrawVBarGraph(GLCD.Left, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 0, 0, BARMAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 10 pixels from left edge of display 
		 * 4 pixels up from bottom
		 * 5 pixels wide to right of x,
		 * (HEIGHT - 8) pixels in height advancing up from y
		 * 1 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Left+10, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 1, 0, BARMAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 20 pixels from left edge of display 
		 * 4 pixels up from bottom
		 * 5 pixels wide to right of x,
		 * (HEIGHT - 8) pixels in height advancing up from y
		 * 2 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Left+20, GLCD.Bottom-4, 5, -(GLCD.Height-2*4), 2, 0, BARMAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * right edge of display 
		 * 4 pixels down from top
		 * 5 pixels wide to left of x,
		 * (HEIGHT - 8) pixels in height advancing down from y
		 * 0 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Right, GLCD.Top+4, -5, (GLCD.Height-2*4), 0, 0, BARMAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 10 pixels from right edge of display 
		 * 4 pixels down from top
		 * 5 pixels wide to left of x,
		 * (HEIGHT - 8) pixels in height advancing down from y
		 * 1 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Right-10, GLCD.Top+4, -5, (GLCD.Height-2*4), 1, 0, BARMAXVAL, num);

		/*
		 * Draw vertical bar graph:
		 * 20 pixels from right edge of display 
		 * 4 pixels down from top
		 * 5 pixels wide to left of x,
		 * (HEIGHT - 8) pixels in height advancing down from y
		 * 2 pixel border,
		 * with a range of 0 to BARMAXVAL
		 */
		GLCD.DrawVBarGraph(GLCD.Right-20, GLCD.Top+4, -5, (GLCD.Height-2*4), 2, 0, BARMAXVAL, num);

		num++;
		delay(DELAY/20);
	}
	delay(DELAY);
}
void DrawVLine(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw a vertical line every 4 pixels
	 */
	for(uint8_t x = 0; x < GLCD.Width; x+=4)
	{
		GLCD.DrawVLine(x, 0, GLCD.Height-1);
		delay(SDELAY/2);
	}
	delay(DELAY);
}
void FillCircle(void)
{
	GLCD.ClearScreen();
	/*
	 * Draw circle in center of display
	 */

	GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-1);
	delay(DELAY);

	/*
	 * Draw filled circle in center of display
	 */

	GLCD.FillCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-1);
	delay(DELAY);

	/*
	 * A little animation for eye candy
	 */

	GLCD.ClearScreen();

	/*
	 * Fill pixels from outside in
	 */
	for(int r = 1; r < GLCD.Height/2; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-r);
		delay(SDELAY/10);
	}

	/*
	 * Remove pixels from inside out
	 */
	for(int r = GLCD.Height/2 -1; r > 0; r--)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-r, PIXEL_OFF);
		delay(SDELAY/10);
	}

	/*
	 * Fill pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r);
		delay(SDELAY/10);
	}

	/*
	 * remove pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r, PIXEL_OFF);
		delay(SDELAY/10);
	}

	/*
	 * Fill pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r);
		delay(SDELAY/10);
	}

	/*
	 * remove pixels from inside out
	 */
	for(int r = 1; r < GLCD.Height/2 -1; r++)
	{
		GLCD.DrawCircle(GLCD.CenterX, GLCD.CenterY, r, WHITE);
		delay(SDELAY/10);
	}

	delay(DELAY);
}
void FillEllipse(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw ellipse in center of display with xrad=32, yrad=14
	 */

	GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, 32, 14);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
	 * Draw filled ellipse in center of display with xrad=32, yrad=14
	 */

	GLCD.FillEllipse(GLCD.CenterX, GLCD.CenterY, 32, 14);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
	 * Draw ellipse with equal x an y rad (circle)
	 * Note: DrawCircle() is more efficient
	 */
	GLCD.FillEllipse(GLCD.CenterX, GLCD.CenterY, 10, 10);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
 	 * Draw an ellipse that fills the display in center
	 */
	GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Width/2-1, GLCD.Height/2-1);

	/*
	 * A little animation for eye candy
	 */
	
	/*
	 * Fill pixels from inside out
	 */
	for(int x = 1; x < GLCD.Height/2; x++)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Width/2-x, GLCD.Height/2-x);
		delay(SDELAY/10);
	}
	/*
	 * remove pixels from inside out
	 */
	for(int x = GLCD.Height/2; x > 0; x--)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Width/2-x, GLCD.Height/2-x, PIXEL_OFF);
		delay(SDELAY/10);
	}

	/*
	 * Fill pixels from inside out (as a circle)
	 */
	for(int x = 1; x < GLCD.Height/2 -1; x++)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, x);
		delay(SDELAY/10);
	}
	/*
	 * remove pixels from inside out (as a circle)
	 */
	for(int x = GLCD.Height/2 -1; x > 1; x--)
	{
		GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, GLCD.Height/2-x, GLCD.Height/2-x, PIXEL_OFF);
		delay(SDELAY/10);
	}

	/*
	 * fill pixels from inside out starting as circle then expanding to ellipse
	 */
	for(int x = 1; x < GLCD.Width/2 -1; x++)
	{
		if(x >= GLCD.Height/2)
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, GLCD.Height/2-1);
		else
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, x);
		delay(SDELAY/10);
	}
	/*
	 * erase pixels from inside out starting as circle then expanding to ellipse
	 */
	for(int x = 1; x < GLCD.Width/2 -1; x++)
	{
		if(x >= GLCD.Height/2)
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, GLCD.Height/2-1,PIXEL_OFF);
		else
			GLCD.DrawEllipse(GLCD.CenterX, GLCD.CenterY, x, x,PIXEL_OFF);
		delay(SDELAY/10);
	}

	delay(DELAY);
}

void FillRect(void)
{
	GLCD.SelectFont(SystemFont5x7);
	GLCD.SetDisplayMode(NON_INVERTED); // "normal" display mode
	GLCD.ClearScreen();

//	GLCD.DrawString("FillRect", gTextfmt_center, gTextfmt_center);
	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from top of display
	 * 25 pixels in from each end
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Top+2, GLCD.Width-25*2, 8);
	delay(DELAY/2);

	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from bottom of display
	 * 25 pixels in from each end
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Bottom-8-2+1, GLCD.Width-25*2, 8);
	delay(DELAY/2);

	/*
	 * Create a filled rectangle 15x15 pixels in each corner
	 */
	draw4corners_FillRect(15, PIXEL_ON); // turn on the pixels

	/*
	 * erase a filled rectangle 10x10 pixels in each corner
	 */
	draw4corners_FillRect(10, PIXEL_OFF); // turn off the pixels

	delay(DELAY/2);

	/*
 	 * Now do it all over again in INVERTED mode
	 */
	GLCD.SetDisplayMode(INVERTED); // "INVERTED" display mode
	GLCD.ClearScreen();
//	GLCD.DrawString("FillRect", gTextfmt_center, gTextfmt_center);

	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from top of display
	 * 25 pixels in from each end
	 * 
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Top+2, GLCD.Width-25*2, 8);

	delay(DELAY/2);
	/*
	 * Fill a rectangle 
	 * 8 pixels in height 2 pixels from bottom of display
	 * 25 pixels in from each end
	 */
	GLCD.FillRect(GLCD.Left+25, GLCD.Bottom-8-2+1, GLCD.Width-25*2, 8);

	delay(DELAY/2);

	/*
	 * Create a filled rectangle 15x15 pixels in each corner
	 */
	draw4corners_FillRect(15, PIXEL_ON); // turn on the pixels

	/*
	 * erase a filled rectangle 10x10 pixels in each corner
	 */
	draw4corners_FillRect(10, PIXEL_OFF); // turn off the pixels

	GLCD.SetDisplayMode(NON_INVERTED); // "normal" display mode
	delay(DELAY);
}

/*
 * create a filled in rectangle in each corner
 * that is exactly size x size pixels.
 */
void draw4corners_FillRect(uint8_t size, uint8_t color)
{
	/*
	 * upper left corner
	 */
	GLCD.FillRect(GLCD.Left, GLCD.Top, size, size, color);
	delay(DELAY/2);

	/*
	 * upper right corner
	 */
	GLCD.FillRect(GLCD.Right-size+1,GLCD.Top, size, size, color);
	delay(DELAY/2);

	/*
	 * lower left corner
	 */
	GLCD.FillRect(GLCD.Left,GLCD.Bottom-size+1, size, size, color);
	delay(DELAY/2);

	/*
	 * Lower right corner
	 */
	GLCD.FillRect(GLCD.Right-size+1,GLCD.Bottom-size+1, size, size, color);
	delay(DELAY/2);
}
void FillRoundRect(void)
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

	delay(DELAY);

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

		delay(DELAY/2);
	}

	delay(DELAY);

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
		delay(DELAY/2);
	}

	
	delay(DELAY);
}
void FillTriangle(void)
{
	GLCD.ClearScreen();
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

	delay(DELAY);


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

	delay(DELAY);
}

void InvertRect(void)
{
	GLCD.ClearScreen();

	/*
	 * Draw rectangles on the display every 4 pixels starting
	 * from the outsided and move in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawRect(GLCD.Left+i, GLCD.Top+i, GLCD.Width - 2*i, GLCD.Height - 2*i);
	}

	delay(DELAY);
	/*
	 * invert the entire display
	 */

	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height);
	delay(DELAY/2);
	/*
	 * invert the entire display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height);
	delay(DELAY);

	/*
	 * invert the left side of display
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(DELAY/2);

	/*
	 * invert the left side of display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(DELAY);

	/*
	 * invert the right side of display
	 */
	GLCD.InvertRect(GLCD.CenterX, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(DELAY/2);

	/*
	 * invert the right side of display to put it back
	 */
	GLCD.InvertRect(GLCD.CenterX, GLCD.Top, GLCD.Width/2, GLCD.Height);
	delay(DELAY);

	/*
	 * invert the top half of display
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height/2);
	delay(DELAY/2);

	/*
	 * invert the top half of display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.Top, GLCD.Width, GLCD.Height/2);
	delay(DELAY);

	/*
	 * invert the Bottom half of display
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.CenterY, GLCD.Width, GLCD.Height/2);
	delay(DELAY/2);

	/*
	 * invert the Bottom half of display to put it back
	 */
	GLCD.InvertRect(GLCD.Left, GLCD.CenterY, GLCD.Width, GLCD.Height/2);
	delay(DELAY);
}
