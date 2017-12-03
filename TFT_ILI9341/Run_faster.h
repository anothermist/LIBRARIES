// Uncomment the F_AS_T #define for ATmega328 (UNO, Nano, Mini pro) or ATmega2560 (MEGA) processors.
// The library then uses direct PORTB or PORTL access and delay based SPI waits for speed.

// For ATmega328 boards the TFT chip select (CS) must be on pin 10 and data/command (DC) on pin 9
// For ATmega2560 boards the TFT chip select (CS) must be on pin 47 and data/command (DC) on pin 48

// Comment out the F_AS_T #define for other processors (32u4 etc) or to use different
// pins for CS and DC on the ATmega328/2560 processors.

#define F_AS_T


// If you are using the ATmega2560 processor then uncomment the following line,
// or comment out this next line for ATmega328 processor.

//#define MEGA_TFT_ILI9341
