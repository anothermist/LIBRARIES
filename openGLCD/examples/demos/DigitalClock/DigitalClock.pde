/*
 * DigitalClock.pde
 *
 * DigitalClock clock using the openGLCD library
 *
 * Currently gets intial date & time from compiled date time.
 * This will make the time look correct when the sketch
 * is uploaded from the IDE.
 * Obviously it won't be correct when the board is reset
 * or powered up later.
 *
 * Requires Time library.
 * download Time library from:
 *  http://www.arduino.cc/playground/Code/Time
 *  http://www.pjrc.com/teensy/td_libs_Time.html (recommended)
 *
 * 2013.11.30 bperrybap        - initial creation
 */

 
#include <openGLCD.h>    // openGLCD library   
#include <Time.h>

void setup()
{
	GLCD.Init(); // initialize the display
	GLCD.SelectFont(lcdnums14x24);	// LCD looking font
//	GLCD.SelectFont(fixednums15x31);// larger font

	setDateTime(); // set date/time
}

void  loop()
{  
static time_t  prevtime;
char buf[10];

	if( prevtime != now() ) // if 1 second has gone by, update display
	{
		prevtime = now();   // save the last update time

		// format the time in a buffer
		snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour(), minute(), second());

		// draw the formatted string centered on the display
  		GLCD.DrawString(buf, gTextfmt_center, gTextfmt_center);
	}
}


tmElements_t tm;

/*
 * Set DateTime for Time library
 */
void setDateTime(void)
{
	getDate(__DATE__); // get compiled date
	getTime(__TIME__); // get compiled time

	// set Time library DateTime to extracted date & time
	setTime(tm.Hour,tm.Minute,tm.Second, tm.Day, tm.Month, tm.Year);
}

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool getTime(const char *str)
{
int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
char Month[12];
int Day, Year;
uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++)
  {
    if (strcmp(Month, monthName[monthIndex]) == 0)
      break;
  }
  if (monthIndex >= 12)
    return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
