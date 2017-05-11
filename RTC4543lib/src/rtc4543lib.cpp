/*
  rtc4543lib.cpp - RTC4543 library
  Copyright (c) 2015 Masami Yamakawa.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program. If not, see [http://www.gnu.org/licenses/].
*/
#include "RTC4543lib.h"

#define BCD2BIN(val) (val - 6 * (val >> 4))
#define BIN2BCD(val) (val + 6 * (val / 10))
#define RTC_SIGNAL_DELAY  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")

#if defined(RTC_SIGNAL_DELAY) && defined(F_CPU) && (F_CPU <= 1000000L)
  #undef RTC_SIGNAL_DELAY
  #define RTC_SIGNAL_DELAY __asm__("nop\n\t")
#endif

RTC4543::RTC4543(uint8_t dataPin, uint8_t clkPin, uint8_t wrPin, uint8_t cePin){
  _dataPin  = dataPin;
  _clkPin   = clkPin;
  _wrPin    = wrPin;
  _cePin    = cePin;
  
  pinMode(_clkPin,OUTPUT);
  digitalWrite(_clkPin,LOW);

  pinMode(_wrPin,OUTPUT);
  digitalWrite(_wrPin,LOW);

  pinMode(_cePin,OUTPUT);
  digitalWrite(_cePin,LOW);
}

void RTC4543::now(){
  open(DATA_READ);

  _bcdSeconds = readBits(8) & 0x7F;
  _bcdMinutes = readBits(8) & 0x7F;
  _bcdHours   = readBits(8) & 0x3F;
  _bcdDay     = readBits(4) & 0x07;
  _bcdDate    = readBits(8) & 0x3F;
  _bcdMonth   = readBits(8) & 0x1F;
  _bcdYear    = readBits(8);
  
  close();
}

void RTC4543::set(uint8_t yy,uint8_t mo,uint8_t dd, uint8_t ww,uint8_t hh,uint8_t mi,uint8_t ss){
  open(DATA_WRITE);
  
  writeBits(BIN2BCD(ss),8);
  writeBits(BIN2BCD(mi),8);
  writeBits(BIN2BCD(hh),8);
  writeBits(BIN2BCD(ww),4);
  writeBits(BIN2BCD(dd),8);
  writeBits(BIN2BCD(mo) & 0x7F,8);
  writeBits(BIN2BCD(yy),8);
  
  close();
}

uint8_t RTC4543::getHours(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdHours);
  else
    return _bcdHours;
}

uint8_t RTC4543::getMinutes(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdMinutes);
  else
    return _bcdMinutes;
}

uint8_t RTC4543::getSeconds(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdSeconds);
  else
    return _bcdSeconds;
}

uint8_t RTC4543::getYear(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdYear);
  else
    return _bcdYear;
}

uint8_t RTC4543::getMonth(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdMonth);
  else
    return _bcdMonth;
}

uint8_t RTC4543::getDate(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdDate);
  else
    return _bcdDate;
}

uint8_t RTC4543::getDay(uint8_t form){
  if(form == IN_BIN)
    return BCD2BIN(_bcdDay);
  else
    return _bcdDay;
}

uint8_t RTC4543::isRunning(){
  uint8_t data = 0;
  
  open(DATA_READ);
  data = readBits(8);
  close();
  return ~data & 0x80;
}

void RTC4543::open(uint8_t wr){
  digitalWrite(_cePin, LOW);
  digitalWrite(_clkPin, LOW);
  RTC_SIGNAL_DELAY;
  
  if(wr){ // wr != 0 Write
    pinMode(_dataPin, OUTPUT);
    digitalWrite(_wrPin, HIGH);    
  }
  else{ // wr == 0 Read
    pinMode(_dataPin, INPUT);
    digitalWrite(_wrPin, LOW);
  }
  
  RTC_SIGNAL_DELAY;
  digitalWrite(_cePin, HIGH);
  RTC_SIGNAL_DELAY;
}

void RTC4543::close(){
  digitalWrite(_clkPin, LOW);
  digitalWrite(_cePin, LOW);
}

uint8_t RTC4543::readBits(uint8_t size){
  uint8_t data = 0;
  
  size = size > 8 ? 8 : size;
  
  for(int i = 0; i < size; i++){
    digitalWrite(_clkPin,HIGH);
    RTC_SIGNAL_DELAY;
    data |= (digitalRead(_dataPin)) << i;
    digitalWrite(_clkPin,LOW);
    RTC_SIGNAL_DELAY;
  }
  
  return data;
}

void RTC4543::writeBits(uint8_t data, uint8_t size){
  
  size = size > 8 ? 8: size;
  
  for(int i = 0; i < size; i++){
    digitalWrite(_dataPin,(data >> i) & 0x01);
    RTC_SIGNAL_DELAY;
    digitalWrite(_clkPin, HIGH);
    RTC_SIGNAL_DELAY;
    digitalWrite(_clkPin, LOW);
    RTC_SIGNAL_DELAY;
  }
}
