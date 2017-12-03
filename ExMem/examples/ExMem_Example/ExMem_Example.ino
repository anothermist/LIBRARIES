// ExMem_Example.ino created for ExMem Library 12/3/2012. 

#include <ExMem.h> // Include at top of sketch.

/***************************************************************
*   You need to connect your hardware as per the ATmega1280/2560
* data sheet as well as the data sheet for the hardware you are
* using. Note: Your hardware must use parallel address/data bus
* (8 to 16 bit address bus and 8 bit data bus). You will also
* need a 74AHC573 transparent latch or similar to multiplex the
* Port A address/data bus of the arduino mega to your hardware.
* You can connect two or more hardware devices provided they use
* separate address ranges and/or proper logic control to select
* which device(s) are active at any given time. See the data
* sheets on how to connect the hardware. Note: If you only
* connect one hardware device set the upperLowerBoundry argument
* of the enable() function to 0x0000. This will create one
* sector from 0x0000 to 0xFFFF - 0x2200 (sector size = 0xDE00).
*
*   The example below enables the External Memory Interface
* module on the Arduino Mega1280/2560 for 16-bit address width,
* an upper and lower sectors with the upper/lower boundry at
* address 0x8000, no upper sector wait-states, and no lower
* sector wait-states. The external memory address ranges are as
* follows: Lower sector address range = 0x0000 to 
* 0x7FFF - 0x2200 (Lower sector size = 0x5E00), Upper sector
* address range = 0x8000 to 0xFFFF (Upper sector size = 0x8000).
* The address ranges start at 0x0000 up to the size of sector -1
* for each sector. More info can be found in the comments of the
* ExMem.h file.
***************************************************************/

void setup()
{
    Serial.begin(115200); // Start Serial I/O for the monitor
    ExMem.enable(16, 0x8000, 0, 0); // Enable ExMem Interface
	
	// Various data types to be written;
    byte a = 255;
    word b = 16300;
    float c = -12.3456789;
    long d = 1000000;
    char f = 'A';
    char* g = "Hello World!";
	
	// Variables to hold the data read
    byte h;
    word i;
    float j;
    long k;
    char m;
    char* n;
	
	// Examples:
	// ExMem.write(sector being written, address in sector, data being written);
	// WxMem.read(sector being read, address in sector, variable for to hold data);
	// Note: when using read you must supply an empty variable of the correct data
	// type to hold the data being read. Both read and write functions will return
	// the size for data type of the supplied variable, NOT the data.
	// See ExMem.h file comments for further details.
	
	// byte
	Serial.print(ExMem.write(LOWER, 0x0010, a));
    Serial.print(", ");
    Serial.print(ExMem.read(LOWER, 0x0010, h));
    Serial.print(", ");
    Serial.println(h, DEC);
	// word
    Serial.print(ExMem.write(LOWER, 0x0018, b));
    Serial.print(", ");
    Serial.print(ExMem.read(LOWER, 0x0018, i));
    Serial.print(", ");
    Serial.println(i, DEC);
	// float
    Serial.print(ExMem.write(LOWER, 0x0020, c));
    Serial.print(", ");
    Serial.print(ExMem.read(LOWER, 0x0020, j));
    Serial.print(", ");
    Serial.println(j, DEC);
	// long
    Serial.print(ExMem.write(LOWER, 0x0028, d));
    Serial.print(", ");
    Serial.print(ExMem.read(LOWER, 0x0028, k));
    Serial.print(", ");
    Serial.println(k, DEC);
	// char
    Serial.print(ExMem.write(LOWER, 0x0040, f));
    Serial.print(", ");
    Serial.print(ExMem.read(LOWER, 0x0040, m));
    Serial.print(", ");
    Serial.println(m);
	// char* (string literal)
    Serial.print(ExMem.write(LOWER, 0x0048, g));
    Serial.print(", ");
    Serial.print(ExMem.read(LOWER, 0x0048, n));
    Serial.print(", ");
    Serial.println(n);
}

void loop()
{
}