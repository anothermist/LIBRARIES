/*
 * ClearArea - openGLCD API example
 *
 * Example of using:
 * ClearArea()
 *
 * ClearArea();
 * Clear Text area with the current font background color and
 * home the cursor to the upper left corner of the text area.
 *
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

/*
 * Create a text area for each of the 4 quadrants
 */
gText textArea[4];

void setup()
{
	Serial.begin(9600);

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
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].ClearArea();
		delay(500);
	}
}
	

void loop()
{
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.ClearScreen();
	GLCD.DrawString(F("ClearArea"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();

	
	/*
	 * print 2 lines in each text area
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		for(int line = 0; line < 2; line ++)
		{
			textArea[i].print("line ");
			textArea[i].println(line);
			delay(500);
		}
	}

	delay(DELAY);

	/*
	 * clear each text area
	 */

	ClearAreas();

	delay(DELAY);
}
