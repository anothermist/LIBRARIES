/*
 * Puts - openGLCD API example
 *
 * Example of using:
 * Puts()
 *
 * Puts(str);
 *
 * Output a NULL terminated character string
 *
 * str   - character string
 *	   str can be any string type
 *
 * Examples:
 *    GLCD.Puts(F("Hello World"));
 *    GLCD.Puts("Hello World");
 *    textarea.Puts(F("Hello World"));
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 *  
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

/*
 * Create a text area for each of the 4 quadrants
 */
gText textArea[4];

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use system in "default" text area
	/*
	 * Create the text area regions in each quadrant and assign font
	 */
	textArea[0].DefineArea(textAreaTOPLEFT, System5x7);
	textArea[1].DefineArea(textAreaTOPRIGHT, System5x7);
	textArea[2].DefineArea(textAreaBOTTOMLEFT, System5x7);
	textArea[3].DefineArea(textAreaBOTTOMRIGHT, System5x7);
}

void ClearAreas()
{

	/*
	 * Clear all the text areas
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].ClearArea();
	}
}
	

void loop()
{
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("Puts"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();


	/*
	 * Text on left of top line of default text area
	 */
	GLCD.CursorTo(0,0);
	GLCD.Puts("hello");
	delay(DELAY/2);
	GLCD.CursorTo(2,1);
	GLCD.Puts("hello");

	delay(DELAY);

	GLCD.ClearScreen();
	/*
	 * Text on left of top line
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(0,0);
		textArea[i].Puts(F("hello"));
		delay(400);
	}
	delay(DELAY/2);

	/*
	 * Text in column 2 of line 1
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(2,1);
		textArea[i].Puts(F("hello"));
		delay(400);
	}
	delay(DELAY);
	ClearAreas();
}
