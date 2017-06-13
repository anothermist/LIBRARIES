/* This is a simple paint sketch!

The example will read the current state of the touch screen and output it to the serial
port as raw X and Y coordinates, as the current position in pixels and a zone number.

Created by Rowboteer for the 400 x 240 TFT display for Arduino Mega: 13/5/15
*/

// Call up the GFX and driver libraries
#include "Adafruit_GFX_AS.h"
#include "Adafruit_R61581_AS.h"

// Call up touch screen library
#include <Touch_AS.h>

// Invoke custom TFT driver library
Adafruit_R61581_AS tft = Adafruit_R61581_AS();


// These are the pins used to interface between the 2046 touch controller and Arduino Mega
#define DOUT 3  /* Data out pin of touch screen */
#define DIN  4  /* Data in pin of touch screen */
#define DCS  5  /* Chip select pin of touch screen */
#define DCLK 6  /* Clock pin of touch screen */

/* Create an instance of the touch screen library */
Touch_AS Touch(DCS, DCLK, DIN, DOUT);

// These are the actual calibration values measured on my Screen #1
// Comment out these next 4 lines if running calibration
#define XMIN 220    /* Default raw Y axis measurement for Left of visible screen */
#define XMAX 3930 /* Default raw X axis measurement for Left of visible screen */
#define YMIN 280    /* Default raw Y axis measurement for bottom of visible screen */
#define YMAX 3800 /* Default raw Y axis measurement for top of visible screen */

// This is the screen size for the raw to coordinate transformation
#define XRES 400 /* Default screen resulution for X axis */
#define YRES 240 /* Default screen resulution for Y axis */

int ColorPaletteHigh = 30;
int color = R61581_WHITE;  //Paint brush color
unsigned int colors[10] = {R61581_RED, R61581_GREEN, R61581_BLUE, R61581_BLACK, R61581_CYAN, R61581_YELLOW, R61581_WHITE, R61581_MAGENTA, R61581_BLACK, R61581_BLACK};

void setup()
{
  tft.begin(R61581B);
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(R61581_BLACK);
  tft.setTextColor(R61581_GREEN);

  //Draw the pallet
  for (int i = 0; i < 10; i++)
  {
    tft.fillRect(i * 40, 0, 40, ColorPaletteHigh, colors[i]);
  }

  //Draw the clear screen button
  tft.setCursor(344, 7, 2); // x,y,font
  tft.setTextColor(R61581_WHITE);
  tft.print("Clear");
  tft.drawRect(0, 0, 399, 30, R61581_WHITE);

  // Plot the current colour in the screen clear box
  tft.fillRect(383, 9, 12, 12, color);

  // Define the raw ranges for correct coordinate conversion
  Touch.SetCal(XMIN, XMAX, YMIN, YMAX, 400, 240); // Raw xmin, xmax, ymin, ymax, width, height
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
    /* Read the current X and Y axis as co-ordinates at the last touch time*/
    // The values returned were captured when Pressed() was called!
    X_Coord = Touch.X();
    Y_Coord = Touch.Y();

    // Detect  paint brush color change
    if (Y_Coord < ColorPaletteHigh + 2)
    {
      if (X_Coord / 40 > 7)
      {
        // Clear the screen to current colour
        tft.fillRect(0, 30, 399, 239, color);
      }
      else
      {
        color = colors[X_Coord / 40];
        // Update the cuurent colour in the clear box
        tft.fillRect(383, 9, 12, 12, color);
      }
    }
    else
    {
      tft.fillCircle(X_Coord, Y_Coord, 2, color);
    }
  }
}

