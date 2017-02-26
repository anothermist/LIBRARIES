/*
 * print - Print class API example
 *
 * Example of using:
 * print()
 * println()
 *
 * print() is not really part of the openGLCD API.
 * It is part of the Arduino Print class.
 * Arduino doesn't have any documentation on the Print class.
 * However, refer to the Serial library for a description of the Print class
 * capabilities.
 * http://www.arduino.cc/en/Serial/Print
 *
 * The print() and println() functions can be used with the default text area or
 * with user defined text areas.
 * i.e.
 * GLCD.print(...);
 * GLCD.println(...);
 * textarea.print(...);
 * textarea.println(...);
 *
 * Examples:
 *    GLCD.print(character);     // print the character at the current cursor position
 *    GLCD.print(integer);       // print the decimal value of the integer 
 *    GLCD.print(integer, DEC);  // as above 
 *    GLCD.print(integer, HEX);  // print the hexadecimal value of the integer 
 *    GLCD.print(integer, OCT) ; // print the octal value of the integer
 *    GLCD.print(integer, BIN) ; // print the binary value of the integer
 *    GLCD.print(float);         // print a floating point number using two decimal places
 *    GLCD.print(float, places); // print a floating point number using 'places' decimal places
 *    GLCD.print(string);        // print the string
 *    GLCD.print("string");      // print the literal string (consumes ram on AVR)
 *    GLCD.print(F("string"));   // print a string from AVR progrem space
 *    textarea.print(integer);   // print the decimal value of the integer in textarea
 *    textarea.print("string");   // print the literal string in textarea
 *
 * Note:
 *     println() works the same as print() but wraps the line after printing.
 *
 * Created by Bill Perry 2013-09-14
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
unsigned long val = 3735928559u; // a really big unsigned long

	/*
	 * Put text in center of display using default text area
	 */

	GLCD.DrawString(F("print"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();


	/*
	 * Text on left of top line of default text area
	 */
	GLCD.CursorTo(0,0);
	GLCD.print("hello");
	delay(DELAY/2);
	GLCD.CursorTo(2,1);
	GLCD.print(1.25, 4); // print with 4 decimal places

	delay(DELAY);

	GLCD.ClearScreen();
	/*
	 * Text on left of top line
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(0,0);
		textArea[i].print(F("hello"));
		delay(400);
	}
	delay(DELAY/2);

	/*
	 * Text in column 2 of line 1
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(2,1);
		textArea[i].print(1.25, 4); // print with 4 decimal places
		delay(400);
	}
	/*
	 * Wrap text and print something in hex
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].println();
		textArea[i].print(val, HEX);
		delay(400);
	}
	delay(DELAY);
	ClearAreas();
}
