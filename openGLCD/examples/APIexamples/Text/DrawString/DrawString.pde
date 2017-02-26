/*
 * DrawString - openGLCD API example
 *
 * Example of using:
 * DrawString()
 *
 * Drawstring(str, hpos, vpos, [erase_option]);
 *
 * Output a NULL terminated character string with
 * horizontal and vertical positioning. 
 *
 * str   - character string
 * hpos  - specifies the horizontal positioning.
 * vpos  - specifies the vertical positioning.
 * erase - optional parameter that specifies which of the remaing pixels
 *         on the text line will be erased before the characters are rendered.
 *         It is one of the values from eraseLine_t and defaults to eraseNONE
 *
 * hpos/vpos can specify:
 * - an x/y pixel position relative to the upper left corner of the text area
 * - a formatting type from gTextfmt_t
 * - the type gTextfmt_col(col) to specify a text column relative
 *   to the left of the text area
 * - the type gTextfmt_row(row) to specify a text row/line relative 
 *   to the top of the text area
 *
 * When hpos & vpos are not a formatting type, 
 * the value is a zero based pixel coordinate relative to
 * the upper left corner of the text area.
 * 
 * Examples:
 *	GLCD.DrawString("Hello", 20, 10, eraseFULL_LINE);		 	// print at pixel coordinate x=20, y=10 relative to top left corner of text area
 *	GLCD.DrawString("Hello", gTextfmt_left, gTextfmt_row(2), eraseFULL_LINE);	// left adjust on row=2 (3rd line from top) 
 *	GLCD.DrawString("Hello", gTextfmt_col(20), gTextfmt_row(2), eraseFULL_LINE); 	// text column=20, row=2
 *	GLCD.DrawString("Hello", gTextfmt_col(col), gTextfmt_row(row), eraseFULL_LINE); // text column=col, row=row
 *	GLCD.DrawString("Hello", gTextfmt_center, gTextfmt_center);			// center on display
 *	GLCD.DrawString("Hello", gTextfmt_center, gTextfmt_current);			// center on current line of display
 *	GLCD.DrawString("Hello", gTextfmt_center, gTextfmt_bottom);			// center on bottom line of display
 *	GLCD.DrawString("Hello", gTextfmt_right, gTextfmt_top);				// right adjust on top line of display
 *	GLCD.DrawString(F("Hello"), gTextfmt_right, gTextfmt_top);			// right adjust on top line of display
 *	GLCD.DrawString_P(PSTR("Hello"), gTextfmt_right, gTextfmt_top);			// right adjust on top line of display
 *	textarea.DrawString(F("Hello"), gTextfmt_right, gTextfmt_top);			// right adjust on top line of display
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
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use system in "default" text area
	/*
	 * Create the text area regions in each quadrant and assign font
	 */
	textArea[0].DefineArea(textAreaTOPLEFT, System5x7);
	textArea[1].DefineArea(textAreaTOPRIGHT, System5x7);
	textArea[2].DefineArea(textAreaBOTTOMLEFT, System5x7);
	textArea[3].DefineArea(textAreaBOTTOMRIGHT, System5x7);

	/*
	 * Set inverse colors on 2 quadrants to allow seeing separation
	 * from other text areas.
	 */

	textArea[1].SetFontColor(WHITE);
	textArea[2].SetFontColor(WHITE);
}

void FillTopLine()
{
int cols = GLCD.GetAreaProp(gTextProp_cols); // get text colums in default text area

	/*
	 * Top line of Default Text area with
	 * 0-9 column numbers
	 */

	GLCD.CursorTo(0,0);
	for(int col = 0; col < cols; col++)
	{
		GLCD.print(col % 10); // colunum number modulo 10 to get 0-9 numbers
		delay(50);
	}
	delay(DELAY/5);
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
	GLCD.ClearScreen();

	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("DrawString"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);


	GLCD.ClearScreen();

	/*
	 * Demonstrate erase options on top line of
	 * default text area
	 */

	FillTopLine();
	GLCD.DrawString(F("Hello"), gTextfmt_center, gTextfmt_row(0)); // default of eraseNONE
	delay(DELAY);

	FillTopLine();
	GLCD.DrawString(F("Hello"), gTextfmt_center, gTextfmt_row(0), eraseNONE); // same as default
	delay(DELAY);


	FillTopLine();
	GLCD.DrawString(F("Hello"), gTextfmt_center, gTextfmt_row(0), eraseTO_EOL); // erase other text to end of line
	delay(DELAY);
	
	FillTopLine();
	GLCD.DrawString(F("Hello"), gTextfmt_center, gTextfmt_row(0), eraseFROM_BOL); // erase other text from beggining of line
	delay(DELAY);
	
	FillTopLine();
	GLCD.DrawString(F("Hello"), gTextfmt_center, gTextfmt_row(0), eraseFULL_LINE); // erase all other text on entire line
	delay(DELAY);


	/*
	 * Now demonstrate hpos/vpos alignment options
	 * (Note: not using erase options, since areas area cleared each time)
	 */

	ClearAreas();

	/*
	 * Text on left of top line
	 * note left is any of these:
	 * gTextfmt_left
	 * gTextfmt_col(0)
	 * 0
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_left, gTextfmt_top);
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text on center of top line
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_center, gTextfmt_top);
	}
	delay(DELAY);
	ClearAreas();


	/*
	 * Text on right of top line
	 * Note: Top line is any of these
	 * gTextfmt_top
	 * gTextfmt_row(0)
	 * 0
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_right, gTextfmt_top);
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text on right of bottom line
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_right, gTextfmt_bottom);
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text on left of bottom line
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_left, gTextfmt_bottom);
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text centered on line 1 (second line from top)
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_center, gTextfmt_row(1));
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text in column 1 vertically centered
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_col(1), gTextfmt_center);
	}
	delay(DELAY);
	ClearAreas();


	/*
	 * Text centered in text area
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].DrawString(F("hello"), gTextfmt_center, gTextfmt_center);
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text centered horizontally but using pixel adjustments vertically
	 */
	for(uint8_t y = 0; y < 8; y +=1)
	{
		for(uint8_t i = 0; i < 4; i++)
		{
			textArea[i].ClearArea();
			textArea[i].DrawString(F("hello"), gTextfmt_center, y);
			delay(200);
		}
	}
	delay(DELAY);
	ClearAreas();

	/*
	 * Text aligned using pixel adjustments horizontally and vertically
	 */
	for(uint8_t p = 0; p < 8; p +=1)
	{
		for(uint8_t i = 0; i < 4; i++)
		{
			textArea[i].ClearArea();
			textArea[i].DrawString(F("hello"), p, p);
			delay(400);
		}
	}
	delay(DELAY);
	ClearAreas();
}
