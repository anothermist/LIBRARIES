/* FILE:    HCTFT_Touch.h
   DATE:    10/06/14
   VERSION: 0.1
   AUTHOR:  Andrew Davies

Library header for 2046 controller based TFT touch screen.

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

Adapted by Rowboteer for the 400 x 240 TFT display for Arduin0 Mega 14/5/15
Renamed Touch_AS
*/

#ifndef Touch_AS_h
#define Touch_AS_h

#include "Arduino.h"
#include "Calibration.h"

// Define as landscape
#define YAXIS 0
#define XAXIS 1

// When a touch is detected the library samples x and y twice
// The two x and two y values must be within RAWERR of each other
// otherwise the touch is rejected as noisy.
// Define +/- raw coord error limit for a press
// A value of about 10 seems good to avoid spurious coords
#define RAWERR 10

// Coordinate inversion
#define XYFLIP 0

class Touch_AS
{
  public:
  Touch_AS(byte CS_Pin, byte Clk_Pin, byte Din_DIO, byte Dout_DIO);
  boolean Pressed(void);
  uint16_t ReadRawX(void);
  uint16_t ReadRawY(void);
  uint16_t RawX(void);
  uint16_t RawY(void);
  uint16_t X(void);
  uint16_t Y(void);
  uint32_t Zone(void);
  void Resolution(uint16_t xres, uint16_t yres);
  void SetCal(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax, uint16_t xres, uint16_t yres);
  //void Orientation(byte orientation);

  private:
  int16_t _ReadAxis(boolean Axis);
  byte _CS;
  byte _Clk;
  byte _Din;
  byte _Dout;
  uint16_t _xmin, _xmax, _ymin, _ymax, _xres, _yres;
  int16_t _xraw, _yraw, _xcoord, _ycoord;
  boolean _xaxis, _yaxis, _xyflip;
  
  void _PulseClock(void);
  uint16_t _ReadData(void);
  void _OutputData(byte Data);
  };

#endif