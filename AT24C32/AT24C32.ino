#include <Wire.h>
#include "AT24C32N.h"

EEPROM_AT24C32N rtcEeprom;
#define AT24C32N_ADDRESS  0x50

uint16_t StartP;
//----------------------------------------------------
void setup ()
{
Wire.begin(4,5); // SDA, SCL
Serial.begin(115200);
Serial.println("");
Serial.println("");
Serial.println("Started");
delay(1000);
StartP = 5;
}
//----------------------------------------------------
void loop ()
{
uint16_t VarLen;
char VarToRead[70];
char ToWrite[] = "abcdefghijklmonpqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
uint8_t lp;

VarLen = strlen(ToWrite);

Serial.printf("About to write [%s] which is [%d] bytes long at memory address [%d]\n",ToWrite,VarLen,StartP);

rtcEeprom.WriteBytes(StartP,VarLen,ToWrite);
memset(VarToRead,0,70);
rtcEeprom.RetrieveBytes(VarToRead, VarLen, StartP, true);
Serial.printf("Wrote    : [%s]\nReceived : [%s]",ToWrite,VarToRead);

if(strcmp(ToWrite,VarToRead)!=0)
  {
  Serial.print("FAIL FAIL ");
  for (lp=0;lp<VarLen;lp++)
    {
    if (ToWrite[lp]!=VarToRead[lp]) break;
    }
  Serial.printf("at position %d, lp/32=%d, lp+address=%d\n",lp,(lp % 32), ((lp + StartP) % 32));
  }
else Serial.println("\n");

Serial.println("\n");
StartP+=22;if (StartP>4000) StartP=4;
Serial.printf("\n");
delay(10000);
}
//----------------------------------------------------
/*
 * To add Serial.printf() to your arduino do the following
 * Find all the Print.h files in your arduino folders
 * Edit each of them, as there will be more than one
 * Add in the following line...
        size_t printf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));
 * above this line
        size_t print(const __FlashStringHelper *);
 * Then edit each Print.cpp, (found in the SAME folder as the Print.h file) and add in this funtion...
size_t Print::printf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    char temp[64];
    char* buffer = temp;
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len > sizeof(temp) - 1) {
        buffer = new char[len + 1];
        if (!buffer) {
            return 0;
        }
        va_start(arg, format);
        vsnprintf(buffer, len + 1, format, arg);
        va_end(arg);
    }
    len = write((const uint8_t*) buffer, len);
    if (buffer != temp) {
        delete[] buffer;
    }
    return len;
}
* add it after the end of the function "Print::write"
* Save them all, re-start Arduino, That's it, you're done. You can now use Serial.printf :)
* The code was copied from the ESP8266 code - many thanks to all those Gurus
*/
