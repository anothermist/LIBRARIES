/*
 * Off - openGLCD API example
 *
 * Example of using:
 * On()
 * Off()
 *
 * status = On();
 * status = Off();
 *
 * On() turns on the display pixels, turns on backlight if configured
 * Off() turns off the display pixels, turns off backlight if configured
 *
 * Returns:
 *    0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

void setup()
{

	GLCD.Init();
	GLCD.SelectFont(System5x7);
	GLCD.DrawString("On/Off test", gTextfmt_center, gTextfmt_center);
}

void loop(void)
{
	int status = GLCD.On(); // turn on pixels, turn on backlight (if configured)
	if(status != GLCD_ENOERR)
	{
		// failed, code here handles error, if needed
	}

	delay(2000);

	status = GLCD.Off(); // turn off pixels, turn off backlight (if configured)
	if(status != GLCD_ENOERR)
	{
		// failed, code here handles error, if needed
	}
	delay(1000);
}
