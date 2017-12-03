/***
  Using the EEMEM data attribute.
  Written By: Christopher Andrews
 
  Released using MIT licence.
 ***/

#include <EEPROM.h>
#include <IPAddress.h>

/***
  EEMEM is an attribute that can be used with static/global 
  variable declarations.
  
  What this does is tell the linker that the address for the variable
  is to reside in the EEPROM memory space. It can also allow assigning
  a default value, however the IDE has this particular feature disabled.
  
  Even though the address is located in the EEPROM, C++ does not know
  the difference between memory spaces and still needs to be accessed 
  as usual using the EEPROM library. The advantage however, is the 
  management of addressing. Your variables will not overlap from faulty
  indexing, and other libraries using EEMEM will not interfere with your
  application.
***/

//Two global variables marked with EEMEM data attribute.
int value EEMEM;
float fraction EEMEM;


struct Configuration{
  unsigned long ip;
  unsigned int timesRun;
};

//An instance of a structure using EEMEM.
Configuration eeConfig EEMEM;


void setup() {

  Serial.begin(9600);
  
  // Wait for serial port to connect. Needed for Leonardo only.
  while (!Serial) {}
  
  // Read value from EEPROM using the address assigned automatically
  // when the declaration is used with EEMEM.
  int result;
  EEPROM.get(&value, result);
  
  float frac;
  EEPROM.get(&fraction, frac);
  
  //Write to the address assigned to fraction.
  frac = 3.14f;
  EEPROM.put(&fraction, frac);
  EEPROM.put(&fraction, 3.14f);
  
  /***
    Using a structure with EEMEM.
  ***/
  
  IPAddress ip(192, 168, 1, 1);
  
  //Save an IP Address. An IPAddress object can cast to a uint32_t
  //or unsigned long. This gives a raw 4-Octet value to save.
  EEPROM.put(&eeConfig.ip, (unsigned long) ip); 

  //Read the value and assign the result directly to the IPAddress
  //object (It accepts uint32_t values as an input).
  uint32_t result;
  ip = EEPROM.get(&eeConfig.ip, result);
  
  Serial.print("IP address is: ");
  Serial.println(ip);
}

void loop() {
 /** Empty Loop **/
}
