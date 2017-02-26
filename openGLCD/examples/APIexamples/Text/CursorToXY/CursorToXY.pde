/*
 * CursorToXY - openGLCD API example
 *
 * Example of using:
 * CursorToXY()
 *
 * CursorToXY(x, y);
 * x   - X pixel coordinate
 * y   - Y pixel coordinate
 * 
 * Position the text cursor to a pixel based x,y coordinate.
 * It sets the cursor position for future text output.
 * x and y are zero based positions relative to the upper
 * left corner of the text area.
 *
 * Examples:
 *
 *   GLCD.CursorToXY(GLCD.CenterX, GLCD.CenterY); // position cursor to center of display
 *   GLCD.CursorToXY(5,8);     // position cursor 5 pixels from left, 8 pixels from top in default text area
 *   textarea.CursorToXY(10,2);// position cursor 10 pixels from left,2 pixels from top in text area
 *   textarea.CursorToXY(0,0); // position cursor upper left corner in text area
 *
 * Note:
 *   For most uses, DrawString() is a better alternative for
 *   positioning text.
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
	for(uint8_t i=0; i < 4; i++)
	{
		textArea[i].ClearArea();
	}
}
	

void loop()
{
	GLCD.ClearScreen();
	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("CursorToXY"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();


	/*
	 * slide text along top sing default text area
	 */

	for(uint8_t x=0; x< 20; x++)
	{
		GLCD.ClearScreen();
		GLCD.CursorToXY(x,0);
		GLCD.print(F("Hello"));
		delay(100);
	}
	

	delay(1000);
	/*
	 * Slid text vertically down left side using default text area
	 */

	for(uint8_t y=0; y< 20; y++)
	{
		GLCD.ClearScreen();
		GLCD.CursorToXY(0,y);
		GLCD.print(F("Hello"));
		delay(100);
	}

	delay(1000);
	GLCD.ClearScreen();

	for(uint8_t x = 0; x< 20; x++)
	{
		ClearAreas();
		/*
		 * Slide Text at  0,0 of each text area
		 * note left is any of these:
		 */
		for(uint8_t i = 0; i < 4; i++)
		{
			textArea[i].CursorToXY(0,0);
			textArea[i].print(F("Hello"));
		}

		/*
		 * draw text at y=8
		 */
		for(uint8_t i = 0; i < 4; i++)
		{
			textArea[i].CursorToXY(x,8);
			textArea[i].print(F("Hello"));
		}
		delay(200);
	}

	delay(DELAY);
	ClearAreas();
}
