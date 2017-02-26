/*
 * DrawTriangle - openGLCD API example
 *
 * Example of using:
 * DrawTriagle()
 *
 * DrawTriangle(x1,y1, x2,y2, x3,y3, [color]);
 *
 * Draw a triangle defined by the 3 points
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000

void setup()
{

    GLCD.Init();
}

void loop(void)
{
	/*
	 * Draw shrinking Triangles on the display starting
	 * on the outside and moving in.
	 */

	for(uint8_t i=0; i< GLCD.Bottom/2; i +=8)
	{
	    GLCD.DrawTriangle(GLCD.Left+2*i+1, GLCD.Bottom-i, GLCD.CenterX, GLCD.Top+i, GLCD.Right-2*i, GLCD.Bottom-i);
	    delay(200);
	}

	delay(DELAY);
	GLCD.ClearScreen();
}
