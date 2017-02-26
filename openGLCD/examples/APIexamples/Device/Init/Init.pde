/*
 *  Init - openGLCD API example
 * 
 * example of using:
 * Init()
 *
 * Low level h/w initialization of library and display.
 *
 * status = Init([mode]);
 *
 * invert - Specifices whether display is in normal mode or inverted mode.
 *
 * This should be called prior to any other graphic library function.
 * It does all the needed initializations including taking care of the
 * low level hardware initalization of the display device.
 *
 * The optional mode parameter specifies if the display should be run in a
 * normal mode, or inverted mode.
 * It does not refer to whether the display uses light or dark pixels.
 *
 * In "normal" or NON_INVERTED mode,
 * pixels are turned on for PIXEL_ON  or BLACK and
 * pixels are turned turned off for PIXEL_OFF or WHITE.
 *
 * In "inverted" or INVERTED mode,
 * pixels are turned off for PIXEL_ON or BLACK and
 * pixels are turned on for ::PIXEL_OFF or ::WHITE.
 *
 * NON_INVERTED mode clears the display with all pixels off and drawing with
 * PIXEL_ON or BLACK will turn on pixels.
 *
 * INVERTED mode clears the display with all pixels on and
 * drawing with PIXEL_OFF or WHITE will turn on pixels.
 *
 * The terminology of BLACK and WHITE is from the perspective of a display
 * that uses black pixels on a light background.
 * For example, if a pixel is set to BLACK and used on a
 * black pixel on light background display, you get a black pixel.
 * However, if the display is a white pixel on dark background display,
 * then setting a pixel to BLACK turns on a white pixel on the display.
 *
 * Because of this, it can be less confusing to use PIXEL_ON and PIXEL_OFF
 * vs BLACK and WHITE.
 *
 * Upon successful completion of the initialization: 
 * - the display pixels will be enabled
 * - the backlight will be turned on (if configured)
 * - the entire display will be cleared
 * - the x,y postion will be set to 0,0
 *
 * Returns:
 *    0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * Note:
 *    This function can also be called to re-initliaze the display hardware.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000

void setup()
{
	/*
	 * normally the GLCD is initalized once in setup()
	 * However, this demo initalizes it multiple times in loop()
	 * for demonstration purposes.
	 */
}

void loop()
{

	// Initialize the GLCD using default/"NON_INVERTED"
	// with no argument is the same as using NON_INVERTED
	GLCD.Init();
	// Select the font for the default text area
	GLCD.SelectFont(System5x7);

	GLCD.DrawString("Init", gTextfmt_center, gTextfmt_center);
	delay(DELAY/2);
	GLCD.DrawString("NON_INVERTED", gTextfmt_center, gTextfmt_center);

	// color is option but this shows that "BLACK" means pixel on
	// and how it interacts with "normal" vs "inverted" initialization
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY-10, GLCD.Right, GLCD.Top, BLACK);
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY+10, GLCD.Right, GLCD.Bottom, BLACK);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY-10, GLCD.Left, GLCD.Top, PIXEL_ON);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY+10, GLCD.Left, GLCD.Bottom, PIXEL_ON);
	delay(DELAY/2);
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY-10, GLCD.Right, GLCD.Top, WHITE);
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY+10, GLCD.Right, GLCD.Bottom, WHITE);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY-10, GLCD.Left, GLCD.Top, PIXEL_OFF);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY+10, GLCD.Left, GLCD.Bottom, PIXEL_OFF);
	delay(DELAY/2);

	// Initialize the GLCD using "INVERTED"
	GLCD.Init(INVERTED);
	// Select the font for the default text area
	GLCD.SelectFont(System5x7);

	GLCD.DrawString("Init", gTextfmt_center, gTextfmt_center);
	delay(DELAY/2);
	GLCD.DrawString("INVERTED", gTextfmt_center, gTextfmt_center);

	// color is option but this shows that "BLACK" means pixel on
	// and how it interacts with "normal" vs "inverted" initialization
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY-10, GLCD.Right, GLCD.Top, BLACK);
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY+10, GLCD.Right, GLCD.Bottom, BLACK);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY-10, GLCD.Left, GLCD.Top, PIXEL_ON);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY+10, GLCD.Left, GLCD.Bottom, PIXEL_ON);
	delay(DELAY/2);
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY-10, GLCD.Right, GLCD.Top, WHITE);
	GLCD.DrawLine(GLCD.Left, GLCD.CenterY+10, GLCD.Right, GLCD.Bottom, WHITE);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY-10, GLCD.Left, GLCD.Top, PIXEL_OFF);
	GLCD.DrawLine(GLCD.Right, GLCD.CenterY+10, GLCD.Left, GLCD.Bottom, PIXEL_OFF);

	delay(DELAY);
}
