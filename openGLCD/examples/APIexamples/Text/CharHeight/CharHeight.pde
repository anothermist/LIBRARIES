/*
 * CharHeight - openGLCD API example
 *
 * Example of using:
 * CharHeight()
 *
 * height = CharHeight(c)
 * c     - character to size
 *         The character code 0 (zero) can be used to retrieve the
 *	   rendered size of all characters in the font.
 *
 * Returns:
 *   The height in pixels of the given character font including any
 *   inter-character gap pixels below the character when the character
 *   is rendered on the display.
 *
 * Note:
 *   The font for the character is the most recently selected font and 
 *   while individual character glyphs will actually render different pixel heights,
 *   the returned value is actually the height of the font row.
 *   i.e a '.' (period) will return the same height as a '@' (at sign).
 *
 *   The value 0 (zero) can be used instead of an actual character value to
 *   return the size of the font.
 *
 *   If there is no font selected or if the character code does not have
 *   a valid definition in the selected font, 0 will be returned.
 *
 * See Also:
 *  CharWidth()
 *  StringWidth()
 *  GetAreaProp()
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
int height;
char c;

	GLCD.ClearScreen();
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("CharHeight"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	
	GLCD.println(); // wrap to next line


	/*
	 * Note:
	 * All characters will return the same height,
	 */
	c = 'H'; // character to size

	GLCD.print('\'');
	GLCD.print(c);
	GLCD.print('\'');
	GLCD.println(" is");
	height = GLCD.CharHeight(c); 
	GLCD.print(height);
	GLCD.print(" pixels high");

	delay(DELAY);
}
