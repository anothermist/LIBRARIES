/*
 * Printf - openGLCD API example
 *
 * Example of using:
 * Printf()
 *
 * Printf(format, ...)
 *
 * print formatted data
 *
 * format - Format string that contains text or optional embedded format tags
 *    ... - Depending on the format string,
 *          the function may expect a sequence of additional arguments
 *
 * Writes a sequence of data formatted as the format argument specifies.
 * After the format parameter, the function expects at least as many additional
 * arguments as specified in the format string.
 * The format string supports all standard printf() formating % tags.
 * See: http://www.cplusplus.com/reference/cstdio/printf/
 *
 * Printf() can be used with the default text area or
 * with user define text areas.
 * i.e.
 * GLCD.Printf(...);
 * textarea.Printf(...);
 *
 * Examples:
 *
 *    GLCD.Printf("Hello World");
 *    GLCD.Printf(F("Hello World")); // put string in flash memory
 *    textarea.Printf("Hello World\n"); // print string with newline
 *    GLCD.Printf("x=%02d", x);      // 2 digits decimal with zero fill
 *    GLCD.Printf("x=%2d", x);       // 2 digits decimal with <space> fill
 *    GLCD.Printf("hexval=%02x", x); // 2 hex digits with zero fill
 *    GLCD.Printf("val=%d", val);    // decimal number
 *    GLCD.Printf(F("val=%d"), val); // decimal number with format string in AVR flash
 *    testarea.Printf("%8s", "hello");    // right justify in 8 character field
 *    testarea.Printf("%-8s", "hello");   // left justify in 8 character field
 *    testarea.Printf("%6.4f", floatval); // 6 total characters, and 4 decimals
 *
 *
 * Note:
 *  By default the xxprintf() routines, which Printf() uses, have no floating
 *  point support in AVR enviornments.  (Works for ARM and PIC32 by default)
 *  In order to enable floating point support for AVR,
 *  a linker option must be changed. Currently,
 *  the Arduino IDE does not support modifying the linker options.
 *
 *  AVR places string constants in RAM. To save space on AVR systems the F()
 *  macro can be used to force the string constant to remain only in flash.
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

	GLCD.DrawString(F("Printf"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);
	GLCD.ClearScreen();


	/*
	 * Text on left of top line of default text area
	 */
	GLCD.CursorTo(0,0);
	// simple string with newline
	GLCD.Printf("hello\n");
	delay(DELAY/2);

	// string in 8 character field right adjusted
	// also puts format string in fash on AVR
	GLCD.Printf(F("%8s\n"), "world");

	// print a float in a 4 character field with 2 decimal digits
 	// will not work on AVR - left as example to show what happens on AVR
	// will print ? on AVR
	GLCD.Printf("%4.2f", 1.2);

	delay(DELAY);

	GLCD.ClearScreen();

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].Printf("%-8s\n", "hello"); // left justify in 8 character field
		delay(400);
	}
	delay(DELAY/2);

	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].Printf("%8s\n", "world"); // right justify in 8 character field
		delay(400);
	}
	delay(DELAY/2);
	/*
	 * print something in hex
	 */
	for(uint8_t i = 0; i < 4; i++)
	{
		textArea[i].Printf("%lx",val); // long as lower case hex 'X' for uppercase
		delay(400);
	}
	delay(DELAY);
	ClearAreas();
}
