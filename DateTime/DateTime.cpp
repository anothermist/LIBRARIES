/*
  DateTime.cpp - Arduino Date and Time library
  Copyright (c) Michael Margolis.  All right reserved.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

extern "C" {
  // AVR LibC Includes
}
//#include <string.h> // for memset
#include "DateTime.h"
#include <wiring.h>

//extern unsigned long _time;

#define LEAP_YEAR(_year) ((_year%4)==0)
static  byte monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

// private methods

void DateTimeClass::setTime(time_t time)
{
    // set the system time to the given time value (as seconds since Jan 1 1970)
    this->sysTime = time;  
	this->prevMillis = millis();
}


//******************************************************************************
//* DateTime Public Methods
//******************************************************************************

DateTimeClass::DateTimeClass()
{
   this->status = dtStatusNotSet;
}

time_t DateTimeClass::now()
{
  while( millis() - prevMillis >= 1000){
    this->sysTime++;
    this->prevMillis += 1000;
  }
  return sysTime;
}

void DateTimeClass::sync(time_t time) 
{
   setTime(time); 
   //status.isSynced = true;   // this will be set back to false if the clock resets 
   //status.isSet = true; // if this is true and isSynced is false then clock was reset using EEPROM -- TODO
   this->status = dtStatusSync;
}

boolean DateTimeClass::available()
{  
// refresh time components if clock is set (even if not synced), just return false if not set
   if(this->status != dtStatusNotSet) { 
      this->now(); // refresh sysTime   
      this->localTime(&this->sysTime,&Second,&Minute,&Hour,&Day,&DayofWeek,&Month,&Year)  ;     
	  return true;
   }
   else
      return false;
}
void DateTimeClass::localTime(time_t *timep,byte *psec,byte *pmin,byte *phour,byte *pday,byte *pwday,byte *pmonth,byte *pyear) {
// convert the given time_t to time components
// this is a more compact version of the C library localtime function

  time_t long epoch=*timep;
  byte year;
  byte month, monthLength;
  unsigned long days;
  
  *psec=epoch%60;
  epoch/=60; // now it is minutes
  *pmin=epoch%60;
  epoch/=60; // now it is hours
  *phour=epoch%24;
  epoch/=24; // now it is days
  *pwday=(epoch+4)%7;
  
  year=70;  
  days=0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch) {
    year++;
  }
  *pyear=year; // *pyear is returned as years from 1900
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  epoch -= days; // now it is days in this year, starting at 0
  //*pdayofyear=epoch;  // days since jan 1 this year
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    if (month==1) { // february
      if (LEAP_YEAR(year)) {
        monthLength=29;
      } else {
        monthLength=28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (epoch>=monthLength) {
      epoch-=monthLength;
    } else {
        break;
    }
  }
  *pmonth=month;  // jan is month 0
  *pday=epoch+1;  // day of month
}


time_t DateTimeClass::makeTime(byte sec, byte min, byte hour, byte day, byte month, int year ){
// converts time components to time_t 
// note year argument is full four digit year (or digits since 2000), i.e.1975, (year 8 is 2008)
  
   int i;
   time_t seconds;

   if(year < 69) 
      year+= 2000;
    // seconds from 1970 till 1 jan 00:00:00 this year
    seconds= (year-1970)*(60*60*24L*365);

    // add extra days for leap years
    for (i=1970; i<year; i++) {
        if (LEAP_YEAR(i)) {
            seconds+= 60*60*24L;
        }
    }
    // add days for this year
    for (i=0; i<month; i++) {
      if (i==1 && LEAP_YEAR(year)) { 
        seconds+= 60*60*24L*29;
      } else {
        seconds+= 60*60*24L*monthDays[i];
      }
    }

    seconds+= (day-1)*3600*24L;
    seconds+= hour*3600L;
    seconds+= min*60L;
    seconds+= sec;
    return seconds; 
}

// make one instance for DateTime class the user 
DateTimeClass DateTime = DateTimeClass() ;
