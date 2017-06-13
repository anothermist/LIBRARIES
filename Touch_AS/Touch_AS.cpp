/* FILE:    HCTFT_Touch.h
   DATE:    10/06/14
   VERSION: 0.1
   AUTHOR:  Andrew Davies

Library for 2046 controller based TFT touch screen.

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

Adapted by Rowboteer for the 400 x 240 TFT display for Arduin Mega 13/5/15
Renamed Touch_AS
*/


#include "Arduino.h"
#include "Touch_AS.h"

/* Constructor to initialise the GPIO */
Touch_AS::Touch_AS(byte CS_Pin, byte Clk_Pin, byte Din_Pin, byte Dout_Pin)
{
  _CS = CS_Pin;
  _Clk = Clk_Pin;
  _Din = Din_Pin;
  _Dout = Dout_Pin;

  _xmin = XMIN;
  _xmax = XMAX;
  _ymin = YMIN;
  _ymax = YMAX;
  _xres = XRES;
  _yres = YRES;
  _xaxis = XAXIS;
  _yaxis = YAXIS;
  _xyflip = XYFLIP;

  pinMode(_CS, OUTPUT); 
  pinMode(_Clk, OUTPUT); 
  pinMode(_Din, OUTPUT); 
  pinMode(_Dout, INPUT); 
  
  digitalWrite(_CS, HIGH);  
  digitalWrite(_Clk, LOW);
  digitalWrite(_Din, LOW);
}

/* Detects if the touch screen is currently pressed. Returns TRUE if pressed */
boolean Touch_AS::Pressed(void)
{
  // Get the raw contact coordinates
  _xraw = _ReadAxis(_xaxis);
  _yraw = _ReadAxis(_yaxis);

  // Now double check the touch is still near the initial contact point
  // We are working with signed integers intentionally
  if (abs(_xraw - _ReadAxis(_xaxis)) > RAWERR) return false;
  if (abs(_yraw - _ReadAxis(_yaxis)) > RAWERR) return false;

  // Check values are in calibration range
  if((_yraw > _ymin && _yraw < _ymax) && (_xraw > _xmin && _xraw < _xmax))
  {
    _xcoord = constrain(map(_xraw, _xmin, _xmax, 0, _xres), 0, _xres);
    _ycoord = constrain(map(_yraw, _ymin, _ymax, 0, _yres), 0, _yres);
    return true; 
  }else
  {
    return false;
  }
}

/* Reads one of the axis (XAXIS or YAXIS) raw coordinates. */
int16_t Touch_AS::_ReadAxis(boolean Axis)
{
  int Data;
  
  digitalWrite(_CS, LOW);
  if (Axis)
  {
    _OutputData(0xD0);   
  }else
  {
    _OutputData(0x90);
  }

  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  Data = _ReadData();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_CS, HIGH); 
  digitalWrite(_Din, LOW);
  
  return Data;
}

/* Generates a pulse on the clock pin */
//void Touch_AS::_PulseClock(void)
//{
//  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW);
//}

/* Reads the raw data from the touch screen */
uint16_t Touch_AS::_ReadData(void)
{
  byte index;
  int Data;
  Data = 0;
  for (index = 12; index > 0; index--)
  {
    Data += digitalRead(_Dout) << (index-1);
    digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  }
  return Data;
}  

/* Writes to the touch screen's configuration register */
void Touch_AS::_OutputData(byte Data)
{
  byte index;
  
  for (index = 8; index > 0; index--)
  {
    digitalWrite(_Din, (Data >> (index -1)) & 1);
    digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  }
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t Touch_AS::ReadRawX(void)
{
  return _ReadAxis(_xaxis);;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t Touch_AS::ReadRawY(void)
{
  return _ReadAxis(_yaxis);;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t Touch_AS::RawX(void)
{
  return _xraw;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t Touch_AS::RawY(void)
{
  return _yraw;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t Touch_AS::X(void)
{
  return _xcoord;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t Touch_AS::Y(void)
{
  return _ycoord;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint32_t Touch_AS::Zone(void)
{
  return _xcoord + (uint32_t)_ycoord *_xres;
}

/* Set the screen resolution in pixels. */
void Touch_AS::Resolution(uint16_t xres, uint16_t yres)
{
  _xres = xres;
  _yres = yres;
}

/* Set the screen calibration values */
void Touch_AS::SetCal(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax, uint16_t xres, uint16_t yres)
{
  _xmin = xmin;
  _xmax = xmax;
  _ymin = ymin;
  _ymax = ymax;
  _xres = xres;
  _yres = yres;
}

/* Set the screen width and height resolution in pixels. */
// ##### NOT YET IMPLEMETED IN OTHER FUNCTIONS #####
/*
void Touch_AS::Orientation(byte orientation)
{
  switch(byte orientation) {
    case 0:
      _xaxis = 0;
      _yaxis = 1;
      _xyflip = 0;
      break;
    case 1:
      _xaxis = 1;
      _yaxis = 0;
      _xyflip = 0;
      break;
    case 2:
      _xaxis = 0;
      _yaxis = 1;
      _xyflip = 1;
      break;
    case 3:
      _xaxis = 1;
      _yaxis = 0;
      _xyflip = 1;
      break;
  }
}
*/
