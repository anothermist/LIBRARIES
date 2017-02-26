/*
 * DrawCircle - openGLCD API example
 *
 * Example of using:
 * Circle()
 *
 * Circle(xCenter,yCenter, radius, [color]);
 *
 *  Draw a circle of the given radius extending out radius pixels from
 *  the center pixel.
 *  The circle will fit inside a rectanglular area bounded by 
 *  x-radius,y-radius and x+radius,y+radius 
 *
 *  The diameter of the circle is radius*2 +1 pixels
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
	 * Draw Circles on the display shrinking every 4 pixels starting
	 * from the outsided and moving in.
	 */
	for(uint8_t i = 0; i < GLCD.Bottom/2; i +=4)
	{
		GLCD.DrawCircle(GLCD.CenterX,GLCD.CenterY, (GLCD.Bottom - 2*i)/2);
		delay(200);
	}
	delay(2000);
}
