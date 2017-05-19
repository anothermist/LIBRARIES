/*
  DateTime.h - Arduino library for date and time functions
  Copyright (c) Michael Margolis.  All right reserved.


  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef DateTime_h
#define DateTime_h

#include <inttypes.h>
//#include <wiring.h> // next two typedefs replace <wiring.h> here (fixed for rel 0012)
typedef uint8_t byte;  
typedef uint8_t boolean;

typedef unsigned long time_t;

/*==============================================================================*/
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)
#define DAYS_PER_WEEK (7L)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52L)
#define SECS_YR_2000  (946681200UL)
 
/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define dayOfWeek(_time_)  (( _time_ / SECS_PER_DAY + 4)  % DAYS_PER_WEEK) // 0 = Sunday
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  // this is number of days since Jan 1 1970
#define elapsedSecsToday(_time_)  (_time_ % SECS_PER_DAY)   // the number of seconds since last midnight 
#define previousMidnight(_time_) (( _time_ / SECS_PER_DAY) * SECS_PER_DAY)  // time at the start of the given day
#define nextMidnight(_time_) ( previousMidnight(_time_)  + SECS_PER_DAY ) // time at the end of the given day 
#define elapsedSecsThisWeek(_time_)  (elapsedSecsToday(_time_) +  (dayOfWeek(_time_) * SECS_PER_DAY) )   

// todo add date math macros
/*============================================================================*/

typedef enum {
	  dtSunday, dtMonday, dtTuesday, dtWednesday, dtThursday, dtFriday, dtSaturday
} dtDays_t;

typedef enum {dtStatusNotSet, dtStatusSet, dtStatusSync
}  dtStatus_t ;

class DateTimeClass
{
private:
	time_t sysTime;  // this is the internal time counter as seconds since midnight Jan 1 1970 (aka unix time)  
	unsigned long prevMillis;
	void setTime(time_t time);
public:
	DateTimeClass();
	void sync(time_t time); // set internal time to the given value 
	time_t now(); // return the current time as seconds since Jan1 1970
	boolean available();  // returns false if not set, else refreshes the Date and Time properties and returns true
	dtStatus_t status;
	byte Hour;
	byte Minute;
	byte Second;
	byte Day;
	byte DayofWeek; // Sunday is day 0 
	byte Month; // Jan is month 0
	byte Year;  // the Year minus 1900 
	
	// functions to convert to and from time components (hrs, secs, days, years etc) to time_t  
	void localTime(time_t *timep,byte *psec,byte *pmin,byte *phour,byte *pday,byte *pwday,byte *pmonth,byte *pyear); // extracts time components from time_t
	time_t makeTime(byte sec, byte min, byte hour, byte day, byte month, int year ); // returns time_t from components
};

extern DateTimeClass DateTime;  // make an instance for the user


#endif /* DateTime_h */

