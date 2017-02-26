/*
 * CharWidth - openGLCD API example
 *
 * Example of using:
 * CharWidth()
 *
 * width = CharWidth(c)
 * c     - character to size
 *
 * Returns:
 *    The width in pixels of the given character including any 
 *    inter-character gap pixels following the character when the
 *    character is rendered on the display.
 *
 * Note:
 * The font for the character is the most recently selected font.
 * If there is no font selected or if the character code does not have
 * a valid definition in the selected font, 0 will be returned.
 *
 *  UTF8 character encoding is supported only if it is enabled in the library config file.
 *  When UTF8 encoding is enabled, CharWidth() will support receiving the UTF8 encoding
 *  as a single character or as multi character byte stream. 
 *  If the UTF8 encoding is sent as multiple bytes, only the last byte of the multi-byte
 *  sequence will return a a non zero width.
 *  All other bytes will return a width of zero.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use SystemFont in "default" text area
}

void loop()
{
int width;
char c;

	GLCD.ClearScreen();
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("CharWidth"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	
	GLCD.println(); // wrap to next line

	c = 'H'; // character to size

	GLCD.print('\'');
	GLCD.print(c);
	GLCD.print('\'');
	GLCD.println(" is");
	width = GLCD.CharWidth(c); 
	GLCD.print(width);
	GLCD.print(" pixels wide");

	delay(DELAY);
}
