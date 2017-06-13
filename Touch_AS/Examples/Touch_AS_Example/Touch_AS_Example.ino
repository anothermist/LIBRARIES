/* FILE:    Touch_AS_Example1
   DATE:    10/06/14
   VERSION: 0.1
   ORIGINAL AUTHOR:  Andrew Davies

This is an example of how to use the Touch_AS library. It does not display
anything on the screen!

Open up the Serial Monitor Window from the IDE and set to 38400 baud to see the
information from screen contacts

By using a stylus at the screen extremes or touch zone limits this sketch can be
used to find the x and y minimum and maximum raw data reading values for the
calibration function SetCal().

The library provides the ability to read the current status of the touch screen.
It does not provide any functions for controlling the TFT screen itself but is
intended for providing touch screen support to any existing sketches or libraries.
However the library will work standalone as this example sketch will demonstrate.

The example will read the current state of the touch screen and output it to the serial
port as raw X and Y coordinates, as the current position in pixels and a zone number.

No information will be displayed on the TFT screen itself whilst this example is running.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.

Adapted by Rowboteer for the 400 x 240 TFT display for Arduin0 Mega 13/5/15
*/

// These are the pins used to interface between the 2046 touch controller and Arduino Mega
#define DOUT 3  /* Data out pin of touch screen */
#define DIN  4  /* Data in pin of touch screen */
#define DCS  5  /* Chip select pin of touch screen */
#define DCLK 6  /* Clock pin of touch screen */

// These are the default min and maximum values, set to 0 and 4095 to calibrate screen
#define XMIN 0    /* Default raw Y axis measurement for Left of visible screen */
#define XMAX 4095 /* Default raw X axis measurement for Left of visible screen */
#define YMIN 0    /* Default raw Y axis measurement for bottom of visible screen */
#define YMAX 4095 /* Default raw Y axis measurement for top of visible screen */

// This is the screen size for the raw to coordinate transformation
#define XRES 400 /* Default screen resulution for X axis */
#define YRES 240 /* Default screen resulution for Y axis */

// These are for dividing screen or part of screen into numbered zones (example is a 4 x 3 matrix of zones)
#define XZONES 4
#define YZONES 3

#include <Touch_AS.h>

/* Create an instance of the touch screen library */
Touch_AS Touch(DCS, DCLK, DIN, DOUT);

void setup()
{
  Serial.begin(38400);
  // Once calibrated these define the raw ranges for correct coordinate conversion
  Touch.SetCal(XMIN, XMAX, YMIN, YMAX, 400, 240); // Raw xmin, xmax, ymin, ymax, width, height

  // Reminder: my full screen[1] portrait raw range values are 220, 3930, 280, 3800
  // Touch.SetCal(220, 3930, 280, 3800, 400, 240); // Use pixel resolution coords
  
  // We can set the calibration values to any rectangular area on the screen
  // then divide up into a matrix of touch zones for for keypads and buttons
  // Touches outside the zone will not be registered
  //Touch.SetCal(220, 3930, 280, 3800, XZONES, YZONES); // Whole screen divided into X x Y zones
}

/* Main program */
void loop()
{
  int X_RawData;
  int Y_RawData;
  int X_Coord;
  int Y_Coord;

  /* Check if the touch screen is currently pressed*/
  // Raw and coordinate values are stored within library at this instant
  // for later retrieval by GetRaw and GetCoord functions.
  // This avoids getting duff values returned
  if (Touch.Pressed()) // Note this function updaes coordinates stored within library variables
  {
    /* Get the raw data for the X and Y axis at the last touch time*/
    // These are typically used for calibration purposes only
    X_RawData = Touch.RawX();
    Y_RawData = Touch.RawY();

    /* Read new raw data for the X and Y axis for touches anywhere on screen*/
    // These could be used to detect touches outside of a zone that would
    // not be detected by the Pressed function
    // These just read the screen raw values directly from the 2046 chip
    //X_RawData = Touch.RawX();
    //Y_RawData = Touch.RawY();

    /* Output the results to the serial port */
    // The values returned were captured when Pressed() was called!
    Serial.print("Raw x,y = ");
    Serial.print(X_RawData);
    Serial.print(",");
    Serial.print(Y_RawData);

    /* Read the current X and Y axis as co-ordinates at the last touch time*/
    // The values returned were captured when Pressed() was called!
    X_Coord = Touch.X();
    Y_Coord = Touch.Y();

    /* Output the results to the serial port */
    Serial.print(" : Coord x,y = ");
    Serial.print(X_Coord);
    Serial.print(",");
    Serial.print(Y_Coord);
    Serial.print(" : Zone = "); Serial.println(Touch.Zone());

    //while(Touch.Pressed()); // Wait for release
    delay(10);
  }
  delay(10);
}

