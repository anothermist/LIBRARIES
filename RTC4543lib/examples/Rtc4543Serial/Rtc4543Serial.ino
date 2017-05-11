/*
 * RTC4543 example code.
 * This example code is in the public domain.
*/


#include <rtc4543lib.h>

const uint8_t dataPin = 5;
const uint8_t clkPin = 4;
const uint8_t wrPin = 12;
const uint8_t cePin = 14;

RTC4543 RTC(dataPin,clkPin, wrPin, cePin);

void setup() {
    Serial.begin(115200);

    // Set current time to RTC4543.
    // RTC.set(YY,MM,DD,W,hh,mm,ss);
    //RTC.set(16,5,31,3,15,9,0);
}

void loop(){
    uint8_t val;

    RTC.now(); //Get (Update) current date and time from RTC4543.

    val = RTC.getYear(RTC4543::IN_BIN); //Get Year in BIN.
    Serial.print("20");
    Serial.print(val);
    Serial.print("/");

    val = RTC.getMonth(RTC4543::IN_BIN);  //Get month in BIN.
    Serial.print(val);
    Serial.print("/");

    val = RTC.getDate(RTC4543::IN_BIN);  //Get date in BIN.
    Serial.print(val);
    Serial.print(" ");

    val = RTC.getDay(RTC4543::IN_BIN);  //Get day in BIN.
    Serial.print(val);
    Serial.print(" ");

    val = RTC.getHours(RTC4543::IN_BCD);  //Get hours in BCD.
    Serial.print(val,HEX);
    Serial.print(":");
    
    val = RTC.getMinutes(RTC4543::IN_BIN);  //Get minutes in BIN.
    Serial.print(val);
    Serial.print(":");
    
    val = RTC.getSeconds(RTC4543::IN_BIN);  //Get seconds in BIN.
    Serial.println(val);
    
    delay(10000);
}

