// DateTime.pde
// example sketch for the DateTime library

#include <DateTime.h>
#include <DateTimeStrings.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

void setup(){
  Serial.begin(19200);
  pinMode(13,OUTPUT); // we flash the LED each second
}

void  loop(){
  unsigned long  prevtime;
  if( getPCtime()) {  // try to get time sync from pc
    Serial.print("Clock synced at: ");
    Serial.println(DateTime.now(),DEC);
  }
  if(DateTime.available()) { // update clocks if time has been synced
    digitalWrite(13,LOW);  // first flash the LED
    prevtime = DateTime.now();
    while( prevtime == DateTime.now() )  // wait for the second to rollover
        ;
    DateTime.available(); //refresh the Date and time properties
    digitalClockDisplay( );   // update digital clock

    // send our time to any app at the other end of the serial port
    Serial.print( TIME_HEADER,BYTE); // this is the header for the current time
    Serial.println(DateTime.now());
    digitalWrite(13,HIGH);
  }
  delay(100); 
}

boolean getPCtime() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    if( Serial.read() == TIME_HEADER ) {        
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        char c= Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      DateTime.sync(pctime);   // Sync Arduino clock to the time received on the serial port
      return true;   // return true if time message received on the serial port
    }  
  }
  return false;  //if no message return false
}

void digitalClockDisplay(){
  // digital clock display of current date and time
  Serial.print(DateTime.Hour,DEC);
  printDigits(DateTime.Minute);
  printDigits(DateTime.Second);
  Serial.print(" ");
  Serial.print(DateTimeStrings.dayStr(DateTime.DayofWeek));
  Serial.print(" ");
  Serial.print(DateTimeStrings.monthStr(DateTime.Month));
  Serial.print(" ");
  Serial.println(DateTime.Day,DEC); 
}

void printDigits(byte digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits,DEC);
}
