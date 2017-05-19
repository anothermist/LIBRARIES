/*
  DateTimeStrings.cpp - Arduino Date and Time library
  Copyright (c) Michael Margolis.  All right reserved.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

  Version 1.0b 12 Nov 2008
*/

// uncomment one of these month and day defines as appropriate for your application
#define dt_LONG_MONTH_STRINGS true
#define dt_LONG_DAY_STRINGS true
//#define dt_SHORT_DAY_STRINGS 
//#define dt_SHORT_MONTH_STRINGS  

extern "C" {
  // AVR LibC Includes
	#include <avr/pgmspace.h>
}

#include "DateTimeStrings.h"
#include <string.h>

// if you change the long strings below, make sure none are greater then the constant dt_MAX_STRING_LEN defined in DateTimeStrings.h
// the short strings for each day or month must be exactly 3 characters

#define dt_SHORT_STR_LEN  3 // the length of short strings

#if defined dt_LONG_MONTH_STRINGS 
char monthStr0[] PROGMEM = "January";
char monthStr1[] PROGMEM = "February";
char monthStr2[] PROGMEM = "March";
char monthStr3[] PROGMEM = "April";
char monthStr4[] PROGMEM = "May";
char monthStr5[] PROGMEM = "June";
char monthStr6[] PROGMEM = "July";
char monthStr7[] PROGMEM = "August";
char monthStr8[] PROGMEM = "September";
char monthStr9[] PROGMEM = "October";
char monthStr10[] PROGMEM = "November";
char monthStr11[] PROGMEM = "December";

PGM_P monthNames_P[] PROGMEM = 
{
    monthStr0,monthStr1,monthStr2,monthStr3,monthStr4,monthStr5,
	monthStr6,monthStr7,monthStr8,monthStr9,monthStr10,monthStr11
};

#elif defined dt_SHORT_MONTH_STRINGS 
char monthNames_P[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";
#endif

#if defined dt_LONG_DAY_STRINGS 
char dayStr0[] PROGMEM = "Sunday";
char dayStr1[] PROGMEM = "Monday";
char dayStr2[] PROGMEM = "Tuesday";
char dayStr3[] PROGMEM = "Wednesday";
char dayStr4[] PROGMEM = "Thursday";
char dayStr5[] PROGMEM = "Friday";
char dayStr6[] PROGMEM = "Saturday";

PGM_P dayNames_P[] PROGMEM = 
{
    dayStr0,dayStr1,dayStr2,dayStr3,dayStr4,dayStr5,dayStr6
};

#elif defined dt_SHORT_DAY_STRINGS
char dayNames_P[] PROGMEM = "SunMonTueWedThrFriSat";
#endif


//******************************************************************************
//* DateTime Public Methods
//******************************************************************************

DateTimeStringsClass::DateTimeStringsClass()
{
   buffer[dt_MAX_STRING_LEN] = 0;  // ensure buffer is null terminated
}

char* DateTimeStringsClass::monthStr(byte month)
{
#if defined dt_SHORT_DAY_STRINGS 
 //  strncpy_P(buffer, (PGM_P)pgm_read_word(&(monthNames_P[(month*dt_SHORT_STR_LEN)])),dt_SHORT_STR_LEN );  // todo !!!!
   for (int i=0; i < dt_SHORT_STR_LEN; i++)      
      buffer[i] = pgm_read_byte(&(monthNames_P[i+ (month*dt_SHORT_STR_LEN)]));  
   buffer[dt_SHORT_STR_LEN] = 0;
#else 
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(monthNames_P[month])));
#endif
	return buffer;
}

char* DateTimeStringsClass::dayStr(byte day) 
{
#if defined dt_SHORT_DAY_STRINGS 
 //  strncpy_P(buffer, (PGM_P)pgm_read_word(&(dayNames_P[(day*dt_SHORT_STR_LEN)])),dt_SHORT_STR_LEN );  // todo !!!!

   for (int i=0; i < dt_SHORT_STR_LEN; i++)      
      buffer[i] = pgm_read_byte(&(dayNames_P[i+(day*dt_SHORT_STR_LEN)]));  
   buffer[dt_SHORT_STR_LEN] = 0; 

#else 
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(dayNames_P[day])));
#endif
   return buffer;
}

// make one instance for the user 
DateTimeStringsClass DateTimeStrings = DateTimeStringsClass() ;
