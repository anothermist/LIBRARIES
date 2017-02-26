/*
 * writeUTF8 - openGLCD API example
 *
 * Example of using:
 * writeUTF8()
 *
 * status = writeUTF8(c)
 * c     - character to output
 *
 * writeUTF8() can be used to output UTF8 encoded characters.
 * UTF8 data can be sent as single multibyte wide UTF8 character codes or
 * as a multi byte stream using multiple calls to writeUTF8().
 * writeUTF8() calls PutChar() to process the character.
 * For more information on UTF8 and UTF8 font see here:
 *  http://en.wikipedia.org/wiki/UTF-8
 *  http://www.fileformat.info/info/charset/UTF-8/list.htm
 *
 * Returns:
 *  Pre 1.x
 *	there is no return value
 *  1.x and beyond
 *  	1 if a character was processed, 0 if a character was not processed
 *
 * Note:
 *  This function only exists when the optional UTF8 library support is enabled.
 *  Unlike write(), this function does not swallow carriage returns.
 *
 *  Use of multi-character UTF8 characters may gnerate warnings when compiling.
 *
 *  Operating systems are not consistent when how to entering utf8/unicode characters
 *  from the keyboard.
 *  On linux you can enter UTF8/Unicode characters from the keyboard using:
 *  <Shift><Ctrl><u> XXXX <enter>
 *  Where XXXX is non encoded hex byte from the U+XXXX code point
 *  i.e. 00a9 is the copyright symbol: ©
 *  and 00f7 is the division sign: ÷
 *
 * Warning:
 *  When UTF8 encoding support is enabled, the raw 8 bit character codes 0xc2 and 0xc3
 *  can no longer be sent as those are used for UTF8 encoding.
 *
 * Created by Bill Perry 2014-09-06
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

#define DELAY 3000 // ms delay between examples

#define exFONT utf8font10x16 // UTF8 font up to U+00FF

/*
 * Create a text area for each of the 4 quadrants
 */
gText textArea[4];

/*
 * text area to display utf8 example font
 */
gText FontArea;

void setup()
{
	GLCD.Init();
	GLCD.SelectFont(System5x7); // use system in "default" text area

	FontArea.DefineArea(textAreaFULL, exFONT);  // for showing utf8 font

	/*
	 * Create the text area regions in each quadrant and assign font
	 */
	textArea[0].DefineArea(textAreaTOPLEFT, exFONT);
	textArea[1].DefineArea(textAreaTOPRIGHT, exFONT);
	textArea[2].DefineArea(textAreaBOTTOMLEFT, exFONT);
	textArea[3].DefineArea(textAreaBOTTOMRIGHT, exFONT);

	Serial.begin(9600);
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

	GLCD.DrawString(F("writeUTF8"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);

#ifndef GLCDCFG_UTF8
	/*
	 * Display message if UTF8 is not enabled
	 */
	GLCD.DrawString(F("(unsupported)"), gTextfmt_center, gTextfmt_bottom, eraseFULL_LINE);
	while(1); // halt catch fire.... (UTF8 support not enabled)
#else

#if 0
	/*
	 * Output all the font codes (characters) beween ASCII <SPACE> 0x20 and 0xfb
	 * to the font text area. Wraping/Scrolling will occur as needed.
	 */

	delay(DELAY);
	FontArea.ClearArea();
	for(uint8_t c = 0x20; c < 0xfb; c++)
	{
		if(c == 0xc2 || c == 0xc3 )
			continue; // skip over UTF8 encoding markers

		FontArea.writeUTF8(c);
		delay(40); // slow down for effect
	}

	delay(DELAY);
#endif

	ClearAreas();

	/*
	 * There are multiple ways to send UTF8 characters.
	 */

	/*
 	 * As a UTF8 multibyte encoded literal character
	 * (must enter using special keyboard sequence)
	 * Can be used with writeUTF8() or PutChar()
	 */
	textArea[0].writeUTF8('µ'); // Micro Sign U+00b5 (c2b5)
	delay(500);
	textArea[0].println(" micro");
	textArea[0].PutChar('÷'); // Division Sign U+00f7 (c3b7)
	delay(500);
	textArea[0].print(" divide");
	delay(300);

	/*
	 * As a UTF8 multibyte character inside a string literal
	 * (must enter using special keyboard sequence)
	 * works with with or without using F() macro
	 * Supported by print(), println(), Puts(), DrawString()
	 */
	textArea[1].println("¢ cent"); // Cent Sign U+00a2 (c2a2)
	delay(500);
	textArea[1].Puts(F("¥ Yen")); // Yen Sign U+00a5 (c2a5)
	delay(500);
	delay(300);

	/*
	 * As 2 seperate UTF encoded bytes.
	 * supported by print(), PutChar(), write(), and writeUTF8()
	 * print() requires a cast to char
	 * easiest to use write(), PutChar(), or writeUTF8()
	 *
	 * Send each byte of the 2 byte encoded sequence seperately,
	 * upper byte first
	 */
	textArea[2].write(0xc3); // Latin Capital letter "thorn" U+00de (c39e)
	textArea[2].write(0x9e); // Latin Capital letter "thorn" U+00de (c39e)
	delay(500);
	textArea[2].println(" thorn");
	textArea[2].PutChar(0xc2); // Half symbol U+00bd (c2bd)
	textArea[2].print((char)0xbd); // Half symbol U+00bd (c2bd)
	delay(500);
	textArea[2].println(" half");
	delay(300);

	/*
	 * using the UTF8 code point which is the same as font character index
	 *
	 * supported by print(), PutChar(), write(), and writeUTF8()
	 * print() requires a cast to char
	 * easiest to use write() or writeUTF8()
	 *
	 */
	textArea[3].write(0xb1); // plus/minus sign U+00b1 (c2b1)
	delay(500);
	textArea[3].println(" pminus");
	textArea[3].print((char)0xf0); // Latin small leter eth U+00f0 (c3b0)
	delay(500);
	textArea[3].print(" eth");
	delay(300);
#endif
	delay(DELAY);
	ClearAreas();
}
