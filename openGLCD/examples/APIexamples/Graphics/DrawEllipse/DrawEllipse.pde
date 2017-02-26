/*
 * DrawEllipse - openGLCD API example
 *
 * Example of using:
 * DrawEllipse()
 *
 * DrawEllipse(xCenter,yCenter, xRadius, yRadius, [color]);
 *
 *  Draw an ellipse of the 2 given radi extending out from
 *  the center pixel.
 *  The ellipse will fit inside a rectanglular area bounded by
 *  x-radius,y-radius and x+radius,y+radius
 *  Final ellipse dimensions will be xRadius*2 +1 by yRadius*2 +1 pixels
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
	 * Draw Ellipses on the display shrinking every 4 pixels starting
	 * from the outsided and moving in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawEllipse(GLCD.CenterX,GLCD.CenterY, (GLCD.Right - 2*i)/2, (GLCD.Bottom - 2*i)/2);
		delay(200);
	}
	delay(2000);
}
