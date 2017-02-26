/*
 * GetAreaProp - openGLCD API example
 *
 * Example of using:
 * GetAreaProp()
 *
 * propval = GetAreaProp(property);
 *
 *  Get Text Area property
 *
 * property   - a member of gTextProp_t
 *
 * Examples:
 *   height = GLCD.GetAreaProp(gTextProp_FontHeight);// font rendered height
 *     cols = GLCD.GetAreaProp(gTextProp_cols);      // text columns
 *     rows = GLCD.GetAreaProp(gTextProp_rows);      // text rows
 *     cols = textarea.GetAreaProp(gTextProp_cols);  // text columns
 *     rows = textarea.GetAreaProp(gTextProp_rows);  // text rows
 *      x1 = textarea.GetAreaProp(gTextProp_x1);     // upper left x coordinate
 *      y1 = textarea.GetAreaProp(gTextProp_y1);     // upper left y coordinate
 *      x2 = textarea.GetAreaProp(gTextProp_x2);     // lower right x coordinate
 *      y2 = textarea.GetAreaProp(gTextProp_y2);     // lower right y coordinate
 *   width = textarea.GetAreaProp(gTextProp_FontWidth);// rendered fixed width
 *   minC  = textarea.GetAreaProp(gTextProp_minC);   // minimum font character code
 *   maxC  = textarea.GetAreaProp(gTextProp_maxC);   // maximum font character code

 *
 * Note:
 *   The value returned for gTextProp_cols is based on the fixed width 
 *   maximum size of the font. 
 *   If a variable font is used, the true number of
 *   actual columns possible will vary depending on the characters rendered.
 *
 *   gTextProp_FontWidth and gTextProp_FontHeight return rendered sizes
 *   in pixels which include any inter character padding pixels.
 *
 * Created by Bill Perry 2013-08-11
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

	/*
	 * Set inverse colors on 2 quadrants to allow seeing separation
	 * from other text areas.
	 */
	textArea[1].SetFontColor(WHITE);
	textArea[2].SetFontColor(WHITE);
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

	GLCD.DrawString(F("GetAreaProp"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
	 * print the Default text area properties
	 */
	GLCD.print("x1: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_x1));
	delay(DELAY/2);

	GLCD.print("y1: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_y1));
	delay(DELAY/2);

	GLCD.print("x2: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_x2));
	delay(DELAY/2);

	GLCD.print("y2: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_y2));
	delay(DELAY/2);

	GLCD.print("cols: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_cols));
	delay(DELAY/2);

	GLCD.print("rows: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_rows));
	delay(DELAY/2);

	GLCD.print("FontW: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_FontWidth));
	delay(DELAY/2);

	GLCD.print("FontH: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_FontHeight));
	delay(DELAY/2);

	GLCD.print("minC: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_minC));
	delay(DELAY/2);

	GLCD.print("maxC: ");
	GLCD.println(GLCD.GetAreaProp(gTextProp_maxC));
	delay(DELAY/2);


	delay(DELAY);

	ClearAreas();
	/*
	 * print out text area properties for
	 * each text area
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].print(F("x1: "));
		textArea[i].println(textArea[i].GetAreaProp(gTextProp_x1));
		delay(DELAY/8);
	}
	delay(DELAY/2);

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].print(F("y1: "));
		textArea[i].println(textArea[i].GetAreaProp(gTextProp_y1));
		delay(DELAY/8);
	}
	delay(DELAY/2);

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].print(F("x2: "));
		textArea[i].println(textArea[i].GetAreaProp(gTextProp_x2));
		delay(DELAY/8);
	}
	delay(DELAY/2);

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].print(F("y2: "));
		textArea[i].println(textArea[i].GetAreaProp(gTextProp_y2));
		delay(DELAY/8);
	}
	delay(DELAY/2);

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].print(F("cols: "));
		textArea[i].println(textArea[i].GetAreaProp(gTextProp_cols));
		delay(DELAY/8);
	}
	delay(DELAY/2);

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].print(F("rows: "));
		textArea[i].println(textArea[i].GetAreaProp(gTextProp_rows));
		delay(DELAY/8);
	}

	delay(DELAY*2);
}
