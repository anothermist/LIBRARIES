/*--------------------------------------------------------------------
 Copyright (c) 2016 Alorium Technology.  All right reserved.
 This library takes advantage of the high-performance ADC found
 on an XLR8 board. It is a 12-bit ADC (versus Arduino's 10bit)
 and can perform at higher sampling rates than Arduino.
 Written by Matt Weber (support@aloriumtech.com) of
 Alorium Technology (info@aloriumtech.com)
 
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with XLR8 NeoPixel.  If not, see
 <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------*/

#include "XLR8ADC.h"

// Similar to Arduino's SPI library, automatically create a single
//  instance that can be used to get at the XLR8ADC functions
XLR8adcClass XLR8ADC;
// Initialize any static class variables here. Since it wouldn't make
//  sense to have multiple instances of this, all class variables would
//  likely be static
// uint8_t XLR8adcClass::placeholder = 0;

void XLR8adcClass::begin() {
  // Assume that if you're using this library you probably want 12 bit mode
  set12bitMode();
}
void XLR8adcClass::setArduinoMode() {
  // Set the ADC to divide by 128, default Arduino operating speed, ~9.6KS/sec
  ADCSRA &= 0xF8;
  ADCSRA |= 0x07;
}
void XLR8adcClass::setXLR8Mode() {
  // Set the ADC to divide by 4, maximum XLR8 operating speed, ~254KSa/sec
  ADCSRA &= 0xF8;
  ADCSRA |= 0x02;
}
void XLR8adcClass::end() {
  // Assume that if you're returning to non-XLR8, you probably want 10 bit mode to match Arduino
  set10bitMode();
}
