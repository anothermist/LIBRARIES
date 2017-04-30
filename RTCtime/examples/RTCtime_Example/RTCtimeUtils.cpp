#include <time.h>


int StringToInt(const char* pString)
{
    int value = 0;

    // skip leading 0 and spaces
    while ('0' == *pString || *pString == ' ')
    {
        pString++;
    }

    // calculate number until we hit non-numeral char
    while ('0' <= *pString && *pString <= '9')
    {
        value *= 10;
        value += *pString - '0';
        pString++;
    }
    return value;
}


// This function takes a 20 chars long textual (English) representation of a date-time value
// of the kind "Feb 28 2017 00:38:05" and convert it to a standard time_t value
// This can be useful to convert the concatenation of the __DATE__ and __TIME__
// compilation date and time strings to a time_t value.
//
// Example:
//
//  #define COMPILE_DATE_TIME (__DATE__ " " __TIME__)
//  time_t compiletime = str20ToTime(COMPILE_DATE_TIME);
//
// I'm not happy with this function name/implementation and it might change in the future...
extern time_t str20ToTime(const char* date)
{
  struct tm tm_time;

  // Get the month  (N.B.: Jan=0, Feb=1, etc...)
  switch (date[0])
  {
  case 'J':
    if ( date[1] == 'a' )
	    tm_time.tm_mon = 0;
	  else if ( date[2] == 'n' )
	    tm_time.tm_mon = 5;
	  else
	    tm_time.tm_mon = 6;
    break;
  case 'F':
    tm_time.tm_mon = 1;
    break;
  case 'A':
    tm_time.tm_mon = date[1] == 'p' ? 3 : 7;
    break;
  case 'M':
    tm_time.tm_mon = date[2] == 'r' ? 2 : 4;
    break;
  case 'S':
    tm_time.tm_mon = 8;
    break;
  case 'O':
    tm_time.tm_mon = 9;
    break;
  case 'N':
    tm_time.tm_mon = 10;
    break;
  case 'D':
    tm_time.tm_mon = 11;
    break;
  }

  // Get the day
  tm_time.tm_mday = (int8_t) StringToInt(date + 4);

  // Get the year
  int16_t year =  (int16_t) StringToInt(date + 7);
  tm_time.tm_year = year - 1900;

  // Get the time
  tm_time.tm_hour = (int8_t) StringToInt(date + 12);
  tm_time.tm_min  = (int8_t) StringToInt(date + 15);
  tm_time.tm_sec  = (int8_t) StringToInt(date + 18);
  tm_time.tm_wday = 0;
  tm_time.tm_isdst = 0;

  return mktime(&tm_time);
}
