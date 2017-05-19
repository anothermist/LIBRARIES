/*
  DateTimeStrings.h - Arduino library for day and month strings
  Copyright (c) Michael Margolis.  All right reserved.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef DateTimeStrings_h
#define DateTimeStrings_h

#include <inttypes.h>
#include <DateTime.h> 

#define dt_MAX_STRING_LEN 9 // length of longest string (excluding terminating null)

class DateTimeStringsClass
{
private:
	char buffer[dt_MAX_STRING_LEN+1];
public:
	DateTimeStringsClass();
	char* monthStr(byte month);
	char* dayStr(byte day);
};

extern DateTimeStringsClass DateTimeStrings;  // make an instance for the user

#endif /* DateTimeString_h */

