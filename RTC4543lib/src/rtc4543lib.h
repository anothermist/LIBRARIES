#ifndef _RTC4543lib_H_
#define _RTC4543lib_H_

#include <Arduino.h>

class RTC4543{
public:
  enum  {DATA_READ, DATA_WRITE};
  enum  {IN_BCD, IN_BIN};
  
  RTC4543(uint8_t dataPin, uint8_t clkPin, uint8_t wrPin, uint8_t cePin);
  void set(uint8_t yy,uint8_t mo,uint8_t dd, uint8_t ww,uint8_t hh,uint8_t mi,uint8_t ss);
  uint8_t isRunning();
  void    now();
  uint8_t getHours(uint8_t form);
  uint8_t getMinutes(uint8_t form);
  uint8_t getSeconds(uint8_t form);
  uint8_t getYear(uint8_t form);
  uint8_t getMonth(uint8_t form);
  uint8_t getDate(uint8_t form);
  uint8_t getDay(uint8_t form);
private:
  uint8_t _bcdHours;
  uint8_t _bcdMinutes;
  uint8_t _bcdSeconds;
  uint8_t _bcdYear;
  uint8_t _bcdMonth;
  uint8_t _bcdDate;
  uint8_t _bcdDay;
  
  uint8_t _dataPin;
  uint8_t _clkPin;
  uint8_t _wrPin;
  uint8_t _cePin;
  
  void  open(uint8_t wr);
  uint8_t readBits(uint8_t size);
  void  writeBits(uint8_t data, uint8_t size);
  void  close();
};

#endif
