/*
 * Streaming - Streaming class library 
 *
 * Example of using:
 * <<
 *
 * Streaming is not part of the openGLCD API.
 * It can be added by installing Mikal Hart's Arduino Streaming libary.
 * For those that want to use C++ streaming i/o
 * further documentation including a download image for the Streaming library
 * can be found here:
 * http://arduiniana.org/libraries/streaming/
 *
 * Streaming can be used on the GLCD object as well as text areas.
 *
 * Examples:
 *
 *    GLCD << character;             // print the character at the current cursor position
 *    GLCD << integer;               // print the decimal value of the integer 
 *    GLCD << _DEC(integer);         // as above 
 *    GLCD << _HEX(integer);         // print the hexadecimal value of the integer 
 *    GLCD << _OCT(integer);         // print the octal value of the integer
 *    GLCD << _BIN(integer);         // print the binary value of the integer
 *    GLCD << float;                 // print a floating point number using two decimal places
 *    GLCD << _FLOAT(float, places); // print a floating point number using 'places' decimal places
 *    GLCD << string;                // print the string
 *    GLCD << "string";              // print the literal string (consumes ram on AVR)
 *    GLCD << F("string");           // print a string from AVR progrem space
 *    textarea << F("string");       // print a string from AVR progrem space
 *    textarea << integer;           // print the decimal value of the integer in textarea
 *    textarea << "string";          // print the literal string in textarea
 *
 *    GLCD << "Counter: " << counter;
 *    textarea << "Byte value: " << _HEX(byteval) << endl; // sends newline as well
 *
 * NOTE:
 * 	using the Arduino F() macro may create compiler warnings.
 *
 * Created by Bill Perry 2014-06-06
 * bperrybap@opensource.billsworld.billandterrie.com
 *  
 */

#include <openGLCD.h>
#include <Streaming.h> // Streaming library for streaming support (must download and install)

#ifndef ARDUINO_STREAMING
#error Arduino Streaming library required for this example
#endif

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

	GLCD.DrawString(F("Streaming"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();

	/*
 	 * output "line" with numbers to default text area
	 */
	for(uint8_t i = 0; i < 10; i++)
	{
		GLCD << F("line ") << i << endl; // print and wrap line
		delay(400);
	}
	delay(DELAY);

	GLCD.ClearScreen();
	/*
	 * Text on left of top line of default text area
	 */
	GLCD.CursorTo(0,0);
	GLCD << "hello";
	delay(DELAY/2);

	/*
	 * indent just a little on next line
	 */
	GLCD.CursorTo(2,1);
	GLCD << _FLOAT(1.25, 4); // print with 4 decimal places

	delay(DELAY);

	GLCD.ClearScreen();
	/*
	 * Text on left of top line
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(0,0);
		textArea[i] << F("hello ") << i; // print hello followed by text area number
		delay(400);
	}
	delay(DELAY/2);

	/*
	 * Text in column 2 of line 1
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].CursorTo(2,1);
		textArea[i] << _FLOAT(1.25, 4); // print with 4 decimal places & wrap line
		delay(400);
	}
	/*
	 * Wrap text and print something in hex
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i] << endl << _HEX(val);
		delay(400);
	}
	delay(DELAY);
	ClearAreas();
}
