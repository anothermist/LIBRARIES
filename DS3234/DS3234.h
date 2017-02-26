/*
  DS3234.h - Arduino library support for the DS3234 SPI Bus RTC
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library has been made to easily interface and use the DS3234 RTC with the 
  Arduino and chipKit development boards. 
  This library makes use of the built-in hardware SPI port of the microcontroller 
  so there are some pin connections that are required (see the manual).

  You can find the latest version of the library at
	http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#ifndef DS3234_h
#define DS3234_h

#if defined(__AVR__)
	#include "Arduino.h"
	#include "hardware/avr/HW_AVR_defines.h"
#elif defined(__PIC32MX__)
	#include "WProgram.h"
	#include "hardware/pic32/HW_PIC32_defines.h"
	#define F(str) (str)
#elif defined(__arm__)
	#include "Arduino.h"
	#include "hardware/arm/HW_ARM_defines.h"
#endif

#define FORMAT_SHORT	1
#define FORMAT_LONG		2

#define FORMAT_LITTLEENDIAN	1
#define FORMAT_BIGENDIAN	2
#define FORMAT_MIDDLEENDIAN	3

#define MONDAY		1
#define TUESDAY		2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

class Time
{
public:
	uint8_t		hour;
	uint8_t		min;
	uint8_t		sec;
	uint8_t		date;
	uint8_t		mon;
	uint16_t	year;
	uint8_t		dow;

	Time();
};

class DS3234
{
public:
	DS3234(uint8_t ss_pin);
	void	begin();
	Time	getTime();
	void	setTime(uint8_t hour, uint8_t min, uint8_t sec);
	void	setDate(uint8_t date, uint8_t mon, uint16_t year);
	void	setDOW(uint8_t dow);

	char	*getTimeStr(uint8_t format=FORMAT_LONG);
	char	*getDateStr(uint8_t slformat=FORMAT_LONG, uint8_t eformat=FORMAT_LITTLEENDIAN, char divider='.');
	char	*getDOWStr(uint8_t format=FORMAT_LONG);
	char	*getMonthStr(uint8_t format=FORMAT_LONG);

	float	getTemp();

	void	poke(uint8_t addr, uint8_t value);
	uint8_t	peek(uint8_t addr);

private:
	uint8_t _ss_pin;
	uint8_t _burstArray[8];

	uint8_t	_readByte();
	void	_writeByte(uint8_t value);
	uint8_t	_readRegister(uint8_t reg);
	void 	_writeRegister(uint8_t reg, uint8_t value);
	void	_burstRead();
	uint8_t	_decode(uint8_t value);
	uint8_t	_decodeH(uint8_t value);
	uint8_t	_decodeY(uint8_t value);
	uint8_t	_encode(uint8_t vaule);

	void	_SPIstart(uint8_t rate = SPI_CLOCK_DEFAULT);
	void	_SPIwrite(byte data);
	byte	_SPIread(void);

};
#endif
