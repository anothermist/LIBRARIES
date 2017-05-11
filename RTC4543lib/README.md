# Project
A simple Arduino library controling the rial time clock RTC-4543. 
This library is designed to easily access to the device from Arduino based board including tiny core based board and ESP8266.

## Manual Deployment

Place RTC4543lib folder under Arduino sketchbook libraries folder. The current sketchbook folder is found by Arduino IDE property dialog. If there is not already a libraries folder in sketchbook folder, create it.

## API

#### RTC4543(uint8_t dataPin, uint8_t clkPin, uint8_t wrPin, uint8_t cePin)
Initialize the library with the pin number.

#### void set(uint8_t yy,uint8_t mo,uint8_t dd, uint8_t ww,uint8_t hh,uint8_t mi,uint8_t ss)
Set date and time to RTC-4543.

#### uint8_t isRunning()
Return 0x00 if RTC detected power failure.

#### void now()
Update current date time from RTC-4543

#### uint8_t getHours(uint8_t form)
form: IN_BCD | IN_BIN
```C++
getHours(RTC4543::IN_BCD);
```
Return Hour in specified form.
12 in BCD: 00010010
12 in BIN: 00001100

#### uint8_t getMinutes(uint8_t form)

#### uint8_t getSeconds(uint8_t form)

#### uint8_t getYear(uint8_t form)

#### uint8_t getMonth(uint8_t form)

#### uint8_t getDate(uint8_t form)

#### uint8_t getDay(uint8_t form)


## Example

This code is tested on Attiny45V + RTC4543 and on ESP8266.

```C++
#include <rtc4543lib.h>

const PROGMEM uint8_t dataPin = 1;
const PROGMEM uint8_t clkPin = 0;
const PROGMEM uint8_t wrPin = 2;
const PROGMEM uint8_t cePin = 4;

/* ESP8266
const uint8_t dataPin = 5;
const uint8_t clkPin = 4;
const uint8_t wrPin = 12;
const uint8_t cePin = 14;
*/

RTC4543 RTC(dataPin,clkPin, wrPin, cePin);

void setup() {
	uint8_t ss;
	if(!RTC.isRunning()){
		// Set (or adjust) time. (yy,MM,dd,ww,hh,mm,ss)
		// 2015.Mar.31 Tue 20:03:00
		//RTC.set(15,3,31,3,20,3,0);
	}
	RTC.now(); //Get (Update) current date and time from RTC4543
	ss = RTC.getSeconds(RTC4543::IN_BCD);  //Get seconds in BCD. use IN_BIN for bin value.
    
    /* Some codes telling the time.
    */
}

```

## License
license.txt
