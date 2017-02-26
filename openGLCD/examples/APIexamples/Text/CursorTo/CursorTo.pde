/*
 * CursorTo - openGLCD API example
 *
 * Example of using:
 * CursorTo()
 *
 * CursorTo(col, row);
 * col  - 0 based text column
 * row  - 0 based text row
 * 
 * Position the cursor to a character based column and row.
 * It Sets the cursor position for future text output.
 * Column and Row are zero based positions relative to the upper
 * left corner of the text area ased on the size of the currently
 * selected font.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000

/*
 * Create a text area for each of the 4 quadrants
 */
gText textArea[4];

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use SystemFont in "default" text area
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
	for(uint8_t i; i < 4; i++)
	{
		textArea[i].ClearArea();
	}
}
	

void loop()
{
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("CursorTo"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();


	/*
	 * Text at col 0, row 0 using default text area
	 */

	GLCD.CursorTo(0,0);
	GLCD.print(F("Hello"));

	delay(1000);
	/*
	 * Text at col 2, row 1 using default text area
	 */

	GLCD.CursorTo(2,1);
	GLCD.print(F("Hello"));
	delay(2000);
	
	GLCD.ClearScreen();

	/*
	 * Text at col 0, row 0 of each text area
	 * note left is any of these:
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(0,0);
		textArea[i].print(F("Hello"));
		delay(300);
	}

	/*
	 * Text at column 2 row 1 of each text area
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(2,1);
		textArea[i].print(F("Hello"));
		delay(200);
	}

	delay(DELAY);
	ClearAreas();
}
