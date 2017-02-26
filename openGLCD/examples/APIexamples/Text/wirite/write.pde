/*
 * write - openGLCD API example
 *
 * Example of using:
 * write()
 *
 * status = write(c)
 * c     - character to output
 *
 *
 * write() is primarily used by the Arduino Print class.
 * write() calls PutChar() to process the character.
 *
 * Returns:
 *  Pre 1.x
 *	there is no return value
 *  1.x and beyond
 *  	1 if a character was rendered, 0 if a character was not rendered
 *
 * Warning:
 * Carriage returns are swallowed to prevent println() from
 * printing the 0xd character if the font contains this character.
 * To actually print the 0xd character use PutChar().
 *
 * When UTF8 encoding support is enabled, the raw 8 bit character
 * codes 0xc2 and 0xc3 can no longer be sent as those are used for
 * UTF8 encoding.
 *
 * Note:
 *  multibyte wide UTF8 characters are not supported as the interface
 *  to write() is limited to 8 bit data.
 *
 *  UTF8 characters, can be used when the optional UTF8 library support is enabled.
 *  However since the write() interface is 8 bit data, each byte of the
 *  UTF8 multibyte character must be sent seperately.
 *  When UTF8 support enabled, writeUTF8() and PutChar() can be used to output
 *  UTF8 encoded characters, including multibyte wide UTF8 character codes 
 *
 *  For addtional information on utf8 character handling see writeUTF8()
 *
 * Created by Bill Perry 2014-09-06
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
	textArea[0].DefineArea(textAreaTOPLEFT, cp437font8x8);
	textArea[1].DefineArea(textAreaTOPRIGHT, cp437font8x8);
	textArea[2].DefineArea(textAreaBOTTOMLEFT, cp437font8x8);
	textArea[3].DefineArea(textAreaBOTTOMRIGHT, cp437font8x8);
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

	GLCD.DrawString(F("write"), gTextfmt_center, gTextfmt_center);
	delay(DELAY);

	GLCD.println(); // wrap to new line
	/*
	 * Output all the characters beween ASCII <SPACE> 0x20 and 0x7f
	 * to the default text area. Wraping/Scrolling will occur as needed.
	 */
	for(uint8_t c = 0x20; c < 0x80; c++)
	{
		GLCD.write(c);
		delay(50); // slow down for effect
	}

	delay(DELAY);
	ClearAreas();

	textArea[0].write(0x03); // cp437 heart symbol
	delay(500);
	textArea[0].println(" heart");
	textArea[0].write(0x01); // cp437 smile symbol
	delay(500);
	textArea[0].print(" smile");
	delay(300);

	textArea[1].write(0x0b); // cp437 male symbol
	delay(500);
	textArea[1].println(" boy");
	textArea[1].write(0x0c); // cp437 female symbol
	delay(500);
	textArea[1].print(" girl");
	delay(300);

	textArea[2].write(0x0d); // cp437 note symbol
	delay(500);
	textArea[2].println(" note");
	textArea[2].write(0x9b); // cp437 cent symbol
	delay(500);
	textArea[2].println(" cent");
	delay(300);

	textArea[3].write(0xe3); // cp437 pi symbol
	delay(500);
	textArea[3].println(" pi");
	textArea[3].write(0xea); // cp437 ohm symbol
	delay(500);
	textArea[3].print(" ohm");
	delay(300);

	delay(DELAY);
	ClearAreas();
}
