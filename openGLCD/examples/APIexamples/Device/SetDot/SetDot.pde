/*
 * SetDot - openGLCD API example
 *
 * Example of using:
 * SetDot()
 *
 * SetDot(x,y, color);
 *
 * sets the color of a pixel on the display.
 *
 * Note: Currently on monochorome displays
 *  PIXEL_ON or BLACK means "ON" and PIXEL_OFF or WHITE means "OFF".
 *
 * When display mode is INVERTED, color meanings reverse.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7);
}

void loop(void)
{
	GLCD.SetDisplayMode(NON_INVERTED);
	GLCD.ClearScreen();
	GLCD.DrawString("SetDot NON_INVERTED", gTextfmt_center, gTextfmt_center);

	for(uint8_t y = 0; y< 8; y +=2)
	{
		for(uint8_t x = 0; x <= GLCD.Right; x++)
		{
			GLCD.SetDot(x,y, PIXEL_ON); // turn on pixel
			delay(10);
		}
	}
	for(uint8_t y = 0; y< 8; y +=2)
	{
		for(uint8_t x = 0; x <= GLCD.Right; x++)
		{
			GLCD.SetDot(x,y, PIXEL_OFF); // turn off pixel
			delay(10);
		}
	}
	delay(2000);


	GLCD.SetDisplayMode(INVERTED);
	GLCD.ClearScreen();
	GLCD.DrawString("SetDot INVERTED", gTextfmt_center, gTextfmt_center);

	for(uint8_t y = 0; y< 8; y +=2)
	{
		for(uint8_t x = 0; x <= GLCD.Right; x++)
		{
			GLCD.SetDot(x,y, PIXEL_ON); // turn on pixel
			delay(10);
		}
	}
	for(uint8_t y = 0; y< 8; y +=2)
	{
		for(uint8_t x = 0; x <= GLCD.Right; x++)
		{
			GLCD.SetDot(x,y, PIXEL_OFF); // turn off pixel
			delay(10);
		}
	}
	delay(2000);
}
