/*
 * cp437
 *
 * This sketch demonstrates using the cp437 font in place of graphics.
 * For a a description of the cp437 font see:
 * http://en.wikipedia.org/wiki/Code_page_437
 *
 * 2013-09-15 Bill Perry  - original creation
 *	bperrybap@opensource.billsworld.billandterrie.com
 */

#include "openGLCD.h"

#define DELAY 3000

/*
 * Some cp437 constants for ease of use
 */

const char dULcorner = 0xc9; // double upper left corner
const char dLLcorner = 0xc8; // double lower left corner
const char dURcorner = 0xbb; // double upper right corner
const char dLRcorner = 0xbc; // double lower right corner
const char dVline = 0xba;    // double vertical line
const char dHline = 0xcd;    // double horizontal line
const char dLtee = 0xcc;     // double left Tee
const char dRtee = 0xb9;     // double right Tee

const char ULcorner = 0xda; // single upper left corner
const char LLcorner = 0xc0; // single lower left corner
const char URcorner = 0xbf; // single upper right corner
const char LRcorner = 0xd9; // single lower right corner
const char Vline = 0xb3;    // single vertical line
const char Hline = 0xc4;    // single horizontal line


gText fontarea; // a text area for showing the font

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(cp437font8x8);
}

void loop()
{
int cols, rows;

	GLCD.ClearScreen();

	GLCD.DrawString("CP437 Font", gTextfmt_center, gTextfmt_center);
	delay(DELAY/2);

	// draw box with simple astrix (would work with any font)
	DrawTextBox(GLCD, '*', '*', '*', '*', '*', '*');
	delay(DELAY/2);

	// draw box with cp437 single lines
	DrawTextBox(GLCD,
	 ULcorner, URcorner, LLcorner, LRcorner, Hline, Vline);

	// draw box with cp437 double lines
	DrawTextBox(GLCD,
	 dULcorner, dURcorner, dLLcorner, dLRcorner, dHline, dVline);

	/*
	 * Extra stuff for larger displays
	 */
#if DISPLAY_HEIGHT >= 64
	/*
	 * Now add some horizontal lines 
	 */

	/*
	 * Get the number of columns and rows in the text area
	 */
	cols = GLCD.GetAreaProp(gTextProp_cols);
	rows = GLCD.GetAreaProp(gTextProp_rows);

	GLCD.CursorTo(0, rows/4);
	GLCD.print(dLtee);
	for(int c = 1; c < cols-1; c++)
	{
		GLCD.print(dHline);
		delay(80);
	}
	GLCD.print(dRtee);

	GLCD.CursorTo(0, rows * 3/4 -1);
	GLCD.print(dLtee);
	for(int c = 1; c < cols-1; c++)
	{
		GLCD.print(dHline);
		delay(80);
	}
	GLCD.print(dRtee);

	/*
 	 * define the font display text area
	 * Should be line just below lower horizontal line
	 * area is 8 pixels (1 character from left edge)
	 * 2 characters smaller than full width (accounts for border)
	 * and is 1 row tall.
	 */
	fontarea.DefineArea(8, GLCD.Height*3/4, cols -2, 1, cp437font8x8);
	
	/*
	 * print all the characters in the font
	 */
	for(int c = 0; c < 256; c++)
	{
		if(c != '\n') // skip newline
			fontarea.print((char)c);
		delay(200);
	}
	
#endif
	delay(DELAY);
}

/*
 * Draw a text border around a text area
 * Must pass in the characters to use
 */

void DrawTextBox(gText &textarea, char ulcorner, char urcorner, char llcorner, char lrcorner, char hline, char vline)
{
int cols, rows;

	/*
	 * Get the number of columns and rows in the text area
	 */
	cols = textarea.GetAreaProp(gTextProp_cols);
	rows = textarea.GetAreaProp(gTextProp_rows);

	// draw the upper left corner, top edge, and upper right corner
	textarea.CursorTo(0,0);
	textarea.print(ulcorner);
	for(int c = 1; c < cols-1; c++)
	{
		textarea.print(hline);
		delay(80);
	}
	textarea.print(urcorner);
	
	// draw the right edge
	for(int r = 1; r < rows-1; r++)
	{
		textarea.CursorTo(cols -1, r);
		textarea.print(vline);
		delay(80);
	}

	// draw the left edge and lower left corner
	for(int r = 1; r < rows-1; r++)
	{
		textarea.CursorTo(0, r);
		textarea.print(vline);
		delay(80);
	}

	textarea.CursorTo(0, rows-1);
	textarea.print(llcorner);

	// draw bottome edge and lower right corner
	for(int c = 1; c < cols-1; c++)
	{
		textarea.print(hline);
		delay(80);
	}
	textarea.print(lrcorner);
}
