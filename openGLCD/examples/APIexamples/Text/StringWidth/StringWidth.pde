/*
 * StringWidth - openGLCD API example
 *
 * Example of using:
 * StringWidth()
 *
 * pixelwidth = StringWidth(str)
 * str     - string to size
 *
 * Returns:
 *  the width in pixels of the sum of all the characters
 *  in the the string pointed to by str.
 *  str can be a pointer to any string type.
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

char *str = "hello";

void loop()
{
int width;

	GLCD.ClearScreen();
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("StringWidth"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	
	GLCD.println(); // wrap to next line

	GLCD.print('"');
	GLCD.print(str);
	GLCD.print('"');
	GLCD.println(" is");
	width = GLCD.StringWidth(str); //works for all string types
	GLCD.print(width);
	GLCD.print(" pixels wide");

	delay(DELAY);
}
