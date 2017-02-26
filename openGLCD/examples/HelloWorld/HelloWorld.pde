/*
 *  openGLCD Library - Hello World
 * 
 * This sketch prints "Hello World!" to the LCD
 * and shows the time in seconds since Arduino board was reset.
 * 
 *  2013-06-15 bperrybap   - updates for openGLCD
 *  2011-09-14 Bill Perry  - original creation
 *  	bperrybap@opensource.billsworld.billandterrie.com
 */

// include the library header
// no font headers have to be included
#include <openGLCD.h>

void setup()
{
  // Initialize the GLCD 
  GLCD.Init();

 // Select the font for the default text area
  GLCD.SelectFont(System5x7);

//  GLCD.print(F("hello, world!")); // keep string in flash on AVR boards with IDE 1.x
//  GLCD.Puts(F("hello, world!")); // Puts() supports F() with any version of IDE

  // print() below uses RAM on AVR boards but works
  // on any version of IDE with any processor
  // note: Same is true for Puts()
  GLCD.print("hello, world!"); 
}

void loop()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  GLCD.CursorTo(0, 1);

  // print the number of seconds since reset:
  GLCD.print(millis()/1000);
}
