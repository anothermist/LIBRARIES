
// ------> V E R Y   V E R Y   I M P O R T A N T !  <------
// This exemaple works only with a DS3231 RTC, which has alarms
// It is not possible to have it working with a DS1307 which lacks that feature


// ------> V E R Y   I M P O R T A N T !  <------
// CONNECTIONS:
//
// DS3231 SDA --> SDA
// DS3231 SCL --> SCL
// DS3231 VCC --> 5V (can be also 3.3V for DS3231)
// DS3231 GND --> GND
// DS3231 SQW --> DIGITAL PIN 2 or any other pin supporting an interrupt (See table here below)

// ------> For this example you *MUST* connect the DS3231 SQW signal to an interrupt pin!!!

// +-------------------+-----------------------------------+
// |                   |  I N T E R R U P T   N U M B E R  |
// |     B O A R D     +-----+-----+-----+-----+-----+-----+
// |                   |  0  |  1  |  2  |  3  |  4  |  5  |
// +-------------------+-----+-----+-----+-----+-----+-----+
// |Uno, Ethernet pins |  2  |  3  |     |     |     |     |
// +-------------------+-----+-----+-----+-----+-----+-----+
// |Mega2560      pins |  2  |  3  | 21  | 20  | 19  | 18  |
// +-------------------+-----+-----+-----+-----+-----+-----+
// |Leonardo      pins |  3  |  2  |  0  |  1  |  7  |     |
// +-------------------+-----+-----+-----+-----+-----+-----+

#define INTERRUPT_PIN 2



// ------> I M P O R T A N T !  <------
// DEBUG - Uncomment/comment the define here below if you want to have more or less information about what's going on.
// Beware that when DEBUG is defined the RTC is reset with the compile time at every run.
#define DEBUG


// ------> I M P O R T A N T !  <------
// Adjust SERIAL_SPEED to your needs:
// The default is 9600, but you probably can increase it in your IDE
// and set it here accordingly
#define SERIAL_SPEED 9600



// ------> I M P O R T A N T !  <------
// Uncomment the #define here below if you want to use
// the SoftwareWire library (... and remember to install it!)
// #define RTC_SOFTWARE_WIRE
#define SOFTWARE_WIRE_SDA SDA  // Or whatever other pin you use
#define SOFTWARE_WIRE_SCL SCL  // Or whatever other pin you use


// ------> I M P O R T A N T !  <------
// We can set our Time Zone and initialize the <time.h> library with it
// Initialization performed later, using the standard set_zone() function.
// set_zone() takes its argument as seconds (positive or negative)
// but using hours is easier (unless you live in a country using a fractional Time Zone)
#define MY_TIMEZONE 1   // I'm in Europe...
#define MY_TIMEZONE_IN_SECONDS (MY_TIMEZONE * ONE_HOUR)


//===============================================================================================================//


// Declared volatile so interrupt can safely modify them
// and other code can safely read and modify them
volatile unsigned long interruptCount = 0;
volatile bool interruptFlag = false;
unsigned long loops = 0;

// We NEED the standard C time library...
#include <time.h>

// This contains a function to convert the __DATE__ and __TIME__ macros to a time_t value
#include "RTCtimeUtils.h"

// Define which TWI/I2C API is used
#ifdef RTC_SOFTWARE_WIRE
  #include <SoftwareWire.h>
  #define myWire SoftwareWire
  myWire I2C(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL);
#else
  #include <Wire.h>
  #define myWire TwoWire
  #define I2C Wire
#endif

// Here where we instantiate our "Rtc" object
  #include <RtcDS3231.h>
  RtcDS3231<myWire> Rtc(I2C);

// Scheduler:
// We will have our loop() "freerunning" and we will print out our time only when an alarm is triggered.
// Having the time printed will thus be a demonstartion of alarms working.


// SETUP
void setup() {

  // Setup Serial
  Serial.begin(SERIAL_SPEED);
  #ifdef DEBUG
    Serial.println(F("Setup started."));
  #endif

  // Setup RTC
  Rtc.Begin();
  set_zone(MY_TIMEZONE_IN_SECONDS);

  // Here we convert the __DATE__ and __TIME__ preprocessor macros to a "time_t value"
  // to initialize the RTC with it in case it is "older" than
  // the compile time (i.e: it was wrongly set. But your PC clock might be wrong too!)
  // or in case it is invalid.
  // This is *very* crude, it would be MUCH better to take the time from a reliable
  // source (GPS or NTP), or even set it "by hand", but -hey!-, this is just an example!!
  // N.B.: We always set the RTC to the compile time when we are debugging.
  #define COMPILE_DATE_TIME (__DATE__ " " __TIME__)
  char* compile_date_time = COMPILE_DATE_TIME;
  time_t compiled_time_t = str20ToTime(compile_date_time);  // str20ToTime converts a 20 chars date and time string to a time_t value (See it in RtcUtility.h)


  // Now we check the health of our RTC and in case we try to "fix it"
  // Common causes for it being invalid are:
  //    1) first time you ran and the device wasn't running yet
  //    2) the battery on the device is low or even missing


  // Check if the time is valid.
#ifndef DEBUG
  if (!Rtc.IsDateTimeValid())
#endif
  {
#ifndef DEBUG
    Serial.println(F("WARNING: RTC invalid time, setting RTC with compile time!"));
#else
    Serial.println(F("DEBUG mode: ALWAYS setting RTC with compile time!"));
#endif
    Rtc.SetTime(&compiled_time_t);
  }

  // Check if the RTC clock is running (Yes, it can be stopped, if you wish!)
  if (!Rtc.GetIsRunning())
  {
    Serial.println(F("WARNING: RTC was not actively running, starting it now."));
    Rtc.SetIsRunning(true);
  }

  // We finally get the time from the RTC into a time_t value (let's call it "now"!)
  time_t now = Rtc.GetTime();

  // As stated above, we reset it to the "Compile time" in case it is "older"
  if (now < compiled_time_t)
  {
    Serial.println(F("WARNING: RTC is older than compile time, setting RTC with compile time!"));
    Rtc.SetTime(&compiled_time_t);
  }

  // Turn off DS3231 32KHz pin
  Rtc.Enable32kHzPin(false);

  // Set the DS3231 SQW pin to be triggered whenever one of the two alarms expires
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);

  // Alarm 1 can be triggered at most once per second:
  // we will use it to print a timestamp every second
  DS3231AlarmOne alarm1(0, 0, 0, 0, DS3231AlarmOneControl_OncePerSecond);
  Rtc.SetAlarmOne(alarm1);

  // Alarm 2 can be triggered at most once per minute:
  // we will use it to print ambient temperature every minute (D3231 has a temperature sensor!)
  DS3231AlarmTwo alarm2(0, 0, 0, DS3231AlarmTwoControl_OncePerMinute);
  Rtc.SetAlarmTwo(alarm2);

  // Throw away any old alarm state before we run
  Rtc.LatchAlarmsTriggeredFlags();

  // Associate the interrupt service routine to a FALLING edge on the board pin connected to the DS3231 SQW pin
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), InterruptServiceRoutine, FALLING);


  #ifdef DEBUG
    Serial.println(F("Setup done."));
  #endif

  Serial.println("");
  // End of Setup
}



void InterruptServiceRoutine()
{
  // Since this interrupted any other running code, so we keep the code here at minimum
  // and especially avoid any communications calls within this routine
  // We just set a flag that will be checked later in the main loop (where real action will take place)
  // and increment a counter to be displayed there
  interruptCount++;
  interruptFlag = true;
}



// Main loop
void loop() {
  loops++;

  if (interruptFlag)  // Check if InterruptServiceRoutine() has set the flag
  {
    // Yes, An interrupt has occoured!

     // Reset the flag
    interruptFlag = false;

    // Get the DS3231 Alarm Flag
    // then allows for others to trigger again
    DS3231AlarmFlag alarm_flag = Rtc.LatchAlarmsTriggeredFlags();

    // See if an Alarm1 was triggered. If yes, we print out a timestamp
    if (alarm_flag & DS3231AlarmFlag_Alarm1)
    {
      // But first, to play it safe, we check if the clock is reliable (it should, as we got an alarm...)
      if (Rtc.IsDateTimeValid())
      {
      // We build and print a standard ISO timestamp with our *local* time
      struct tm local_tm;
      Rtc.GetLocalTime(&local_tm);                  // GetLocalTime() compiles a "struct tm" pointer with local time
      char local_timestamp[20];
      strcpy(local_timestamp, isotime(&local_tm));  // We use the standard isotime() function to build the ISO timestamp
      Serial.print(local_timestamp);

      // We print how many interrupts (Alarms) have happened so far...
      // And how many loops since the previous interrupt.
      Serial.print(F(" - Interrupt: "));
      Serial.print(interruptCount);
      Serial.print(F(" - Loops: "));
      Serial.println(loops);
      }
      else
      {
        // Battery on the device is low or even missing and the power line was disconnected
        Serial.println(F("RTC clock has failed!"));
      }
    }

    // See if an Alarm2 was triggered. If yes, we print out the temperature
    if (alarm_flag & DS3231AlarmFlag_Alarm2)
    {
      float temperature = Rtc.GetTemperature();
      Serial.print(F("Alarm two triggered, temperature is: "));
      Serial.print(temperature);
      Serial.print(F("C ("));
      Serial.print(temperature * 1.8 + 32);
      Serial.println(F("F)"));
    }

    loops = 0;
  }

  // End of the loop
}
