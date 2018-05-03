# RTCtime

**An Arduino Real Time Clock library.**

This library makes using modules based on the DS1307 and DS3231 RTC really simple. It is compatible with and is meant to leverage the standard C time library `<time.h>` API (datatypes and functions) as defined in [ISO/IEC 9899 (C90)](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf) and implemented in the AVR Std C runtime libray since version 2.0.0 (_available in the Arduino IDE since version 1.6.10_).

This work started as a fork of the excelent [Rtc by Makuna](https://github.com/Makuna/Rtc) library, but it has been modified to support the Std C time API, while the original "Rtc by Makuna" uses a proprietary API.

**Please note that it is absolutely mandatory to have an underlying standard compliant C time library (`time.h`) as this library uses data types and functions which are defined there.** This probably limits the usefulness of this library to boards based on the AVR architecture for which such a library exists and it makes impossible to use it for architectures for which such a library is not availabe (_e.g. the ESP8266 or the Arduino DUE_)

Extra features (_such as temperature reading, calibration, alarms and memory storage_) are available when supported by the underlying hardware.

The SoftwareWire I2C bus library is supported.

## Notes
- **This is a library for getting/setting the time from/to hardware RTC clocks**, using an API compatible with the Std C time library and the epoch defined in its AVR implementation, `2000-01-01T00:00:00Z`.

- **This is not** a library of time functions.

- Time functions are already available in the AVR Standard C runtime library and you can access them using `#include <time.h>` since Arduino IDE version 1.6.10. Those on a previous version of the IDE can probably use the [Arduino_RTC_Library](https://github.com/feilipu/Arduino_RTC_Library) (_originally by Michael Duane and now maintained by Phillip Stevens_) as a Std. C time compliant library.

- When using the AVR `<time.h>` Std C time library, you should run your RTC clock as UTC, and then use the `set_zone()` function to set your current timezone. Same thing with DST and the `set_dst()` function.

- Two methods have been added, `SetTimeUX()` and `GetTimeUX()`, that uses the Unix epoch, `1970-01-01T00:00:00Z`, instead of the AVR one (`2000-01-01T00:00:00Z`). This can be useful when using time values based on the Unix epoch. **Do not use the this methods if you are interfacing with the normal AVR Std C time library**.

## Installation

### Install this library using the Arduino IDE (_prefered method_)
 - Using your Arduino IDE, open the Library Manager, search for "RTCtime by smz" and install.

### Install this library from GitHub (_alternative method_)
 - Create a directory named "RTCtime" in your Arduino\libraries folder.
 - Clone this project into that folder. 
 - It should now be available in your Arduino IDE "Include Library" list.

## Examples

 - There are a couple of commented examples in the `./examples` folder. You might try them and use them as a reference, until I write a more formal API documentation.

## Donating
If you like this library and you use it, please consider donating, but please __consider donating to the orignal devloper too__, as **he** did much of the work! 

[![Donate](http://img.shields.io/paypal/donate.png?color=yellow)](https://www.paypal.me/SergioManzi)
