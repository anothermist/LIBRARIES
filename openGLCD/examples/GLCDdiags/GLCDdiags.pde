/*
 * GLCDdiags
 *
 * This sketch tests the memory and interface to the GLCD module as well as
 * report the current openGLCD library configuration to the serial port.
 *
 * It is recommended to run the sketch with a serial monitor
 * to be able to see the diagnostic messages.
 * When using the Arduino IDE, start the serial monitor afer
 * the upload finishes.
 *
 * While the sketch will work without a serial monitor connection,
 * on some USB based boards like the Teensy or Leonardo,
 * if there is no USB serial port connection,
 * the code will wait up to 10 seconds for a USB serial
 * connection before starting.
 *
 * The serial port is configured to 9600 baud.
 *
 * It also displays a set of visual screens on the GLCD that can aid in
 * diagnosing mis-configured/wired chip select lines.
 *
 * The memory associated with each chip is tested seperately.
 * Tests are performed starting on chip #0.
 * The tests go through a series of visual displays as the memory is tested.
 * The chip # under test as well as the x coordinate values are displayed: 
 * if everthing is working and configured properly, chip #0 will be on the left
 * and each increasing chip # will advance to the right.
 *
 * Status and error information is sent to the serial port while testing.
 *
 * The sketch peforms a few different memory tests but the main tests walk an
 * incrementing pattern through memory horizontally by incrementing through
 * pages column at a time (left to right) as well as vertically by incrementing
 * thorough pages a page at time (top to bottom).
 * 
 * NOTE:
 * This sketch is a diagnostic tool, as such,
 * it is not an example of library usage.
 * It uses internal openGLCD library information that is not needed
 * and should not normally used by sketches.
 * Sketches that use these undocumented capabilities may not work correctly
 * in future releases
 *   
 * 
 * 2013-06-15 bperrybap    - many updates and conversion to openGLCD
 * 2010-2013  bperrybap    - many updates & features
 * 2010.03.29 memargolis   - changes for gText class
 * 2010.03.27 memargolis   - changes for text wrappers
 * 2010.03.11 memargolis   - show chip numbers
 * 2010.03.05 memargolis   - print version and draw triangle
 * 2010.04.10 Bill Perry   - original creation
 * 	bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>
#include "include/glcd_errno.h"
#include "include/glcd_delay.h" // normal sketches don't need this.

/*
 * Nasty kludges for @#@#@ AVR progmem CRAP
 * PSTR() macro uses static char instead of const char
 * This matters on C++ so we can't use const on our progmem strings
 * because you can't mix const and non const in the same section.
 * It could be declared as "static" instead of "const" or
 * alternatively you can use a different section.
 * But we still need to redefine PROGMEM to keep from getting warnings
 * for for each and every one of our PSTR strings.
 */

#if defined(__AVR__)
#include <avr/pgmspace.h>
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif
#define PROGMEMDIAG __attribute__((section(".progmem.diag")))
#define P(name)   const char name[] PROGMEMDIAG   // declare a const string in AVR Progmem

#else
// The rest of the world is so much simpler and normal
#define P(name)   const char name[]   // declare a const string
#endif


/*
 * Macros to convert chip#s to upper and lower pixel coordinates.
 * x1,y1 is upper left pixel coordinate and x2,y2 is lower left right coordinate.
 */

#define chip2x1(chip) ((chip * CHIP_WIDTH) % DISPLAY_WIDTH)
#define chip2y1(chip) (((chip * CHIP_WIDTH)/DISPLAY_WIDTH) * CHIP_HEIGHT)
#define chip2x2(chip) ((chip2x1(chip) + CHIP_WIDTH) >= DISPLAY_WIDTH ? DISPLAY_WIDTH-1 : chip2x1(chip) + CHIP_WIDTH-1)
#define chip2y2(chip) ((chip2y1(chip) + CHIP_HEIGHT) >= DISPLAY_HEIGHT ? DISPLAY_HEIGHT-1 : chip2y1(chip) + CHIP_HEIGHT-1)


#define MAX_ERRORS 10 // maximum errors per indivual test

#ifdef _AVRIO_AVRIO_
int avrpin2arduinopin(avrpin_t avriopin);
void _SerialPrintPINstr(avrpin_t pin, const char *pinstr, uint8_t avrport,
	 uint8_t avrbit);

#define SerialPrintPINstr(x) \
  _SerialPrintPINstr(x, str(x), AVRIO_PIN2AVRPORT(AVRIO_PIN2AVRPIN(x)), AVRIO_PIN2AVRBIT(AVRIO_PIN2AVRPIN(x)))

#else

void _SerialPrintPINstr(const char *pinstr);
#define SerialPrintPINstr(x) _SerialPrintPINstr(str(x))
#endif

/*
 * Prototypes
 * The IDE will screw up inserting prototypes if the return type is not on the
 * same line as the rest of the function definition, so we will declare all
 * the prototypes here to avoid any issues.
 */
void setup();
void loop();
void showGLCDconfig(void);
uint8_t lcdmemtest(void);
uint16_t getglcdspeed();
uint8_t lcdw1test(void);
uint8_t lcdrwseltest();
int lcdhpagetest(uint8_t x1, uint8_t x2, uint8_t spage, uint8_t epage,
	 uint8_t sval, uint8_t eval);
int lcdvpagetest(uint8_t x1, uint8_t x2, uint8_t spage, uint8_t epage,
	 uint8_t sval, uint8_t eval);
void SerialPrintP(PGM_P str );

/*
 * declare a string for a horizontal line in program memory
 */
P(hline) =  "--------------------------------------------------------------------\n";

P(GLCDlibName) = GLCD_GLCDLIB_NAMESTR;
P(GLCDlibVersion) = GLCD_GLCDLIB_VERSIONSTR;
P(GLCDlibBuild) = GLCD_GLCDLIB_BUILD_BUILDSTR;


#define xstr(s) str(s)
#define str(...) #__VA_ARGS__

/*
 * Function to print a simple Quoted string to serial port.
 * The string is automagically forced to live in AVR flash/program space.
 */

#define SerialPrintQ(str) SerialPrintP(PSTR(str))

/*
 * print Progmem string to the serial port
 * (have to insert carriage returns as serial port runs in 'raw' mode)
 *
 */

void SerialPrintP(PGM_P str )
{
  char c;
  PGM_P p = str;

  while ((c = pgm_read_byte(p++)))
  {
    if(c == '\n')
      Serial.print('\r');
    Serial.print(c);   
  }
}

#ifdef SERIALPRINTF

/*
 * Define a REAL printf since Arduino doesn't have one
 *
 * SerialPrintf() will automatically put the format string in AVR program space
 * 
 */

#define SerialPrintf(fmt, ...) _SerialPrintf(PSTR(fmt), ##__VA_ARGS__)

extern "C" {
  int serialputc(char c, FILE *fp)
  { 
      if(c == '\n')
        Serial.write('\r'); 
    Serial.write(c); 
  }
}


void
_SerialPrintf(const char *fmt, ...)
{
FILE stdiostr;
va_list ap;

  fdev_setup_stream(&stdiostr, serialputc, NULL, _FDEV_SETUP_WRITE);

  va_start(ap, fmt);
  vfprintf_P(&stdiostr, fmt, ap);
  va_end(ap);
}

/*
 * Define an eprintf() function for error output
 * (map it to the SerialPrintf() defined above)
 */
#define eprintf(...) SerialPrintf(__VA_ARGS__)

#endif //SERIALPRINTF


/*
 * GlcdPrintf() will automatically put the format string in AVR program space
 */
#define GlcdPrintf(fmt, ...) GLCD.Printf_P(PSTR(fmt), ##__VA_ARGS__)


void setup()
{
  delay(5);	// allow the hardware time settle
  Serial.begin(9600);

#if defined(CORE_TEENSY) || defined(__AVR_ATmega32U4__)
  do
  {
	// wait on USB serial port to be ready but timout out after 10 seconds
	if(millis() > 10000) // millis starts at 0 after reset
		break;
  } while(!Serial);

#endif

  SerialPrintQ("Serial initialized\n");
}

/*
 * attempt to show the library version information
 */
void
showlibinfo()
{
  /*
   * show Version info and Sequential ascii characters 
   */
  GLCD.ClearScreen();

  char strbuf[64];
  strcpy_P(strbuf, GLCDlibName);
  GLCD.DrawString(strbuf, gTextfmt_center, gTextfmt_row(0));
  strcpy_P(strbuf, GLCDlibVersion);
  GLCD.DrawString(strbuf, gTextfmt_center, gTextfmt_row(1));

  GLCD.CursorTo(0, 3);
  for(int i=0; i  < GLCD.Width / GLCD.CharWidth(' '); i++ )
  {
     GLCD.print(char('A' + i)); // show the ascii characters
  }
  delay(5000);

}


/*
 * Attempt to show some graphical information on
 * the display that will easily visually demonstrate
 * whether or not the chip select lines are properly
 * connected.
 */
void
showchipselscreen(void)
{
  GLCD.ClearScreen();
  /*
   * draw a triangle
   */
  for(int x = 0; x <= GLCD.Right; x++)
  {
     GLCD.DrawVLine( x, 0, map(x, 0, GLCD.Right, 1, GLCD.Height));
     delay(50); // delay is to allow seeing if chip selects are duplicated or overlapping
  }   
  delay(4000);
  // show chips
  GLCD.ClearScreen();
  for(int chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    // delay and flash is to allow seeing duplicate or overlapping chip selects
    for(uint8_t flash = 0; flash < 4; flash++)
    {
      GLCD.CursorToXY(chip2x1(chip), chip2y1(chip));
      if(flash & 1)
        GLCD.SetFontColor(BLACK);
      else
        GLCD.SetFontColor(WHITE);
      GLCD.print("Chip:");
      GLCD.print(chip);
      delay(350); 
    }
  }

  delay(5000);

}

void
loop()
{   // run over and over again

int lcount = 1; // total loop count
int lfcount = 0; //  fail count
unsigned int glcdspeed, kops, kops_fract;
int status;

  while(1)
  {
    /*
     * Dump GLCD config information *before* trying to talk to the GLCD
     * in case there is a problem talking to the GLCD.
     * This way ensures the GLCD information is always available on the serial port.
     */

    /*
     * dump the GLCD library configuration information to
     * the serial port.
     */
    showGLCDconfig();


    SerialPrintP(hline);
    SerialPrintQ("Diag Loop: ");
    Serial.println(lcount);

    SerialPrintQ("Initializing " GLCD_GLCDLIB_NAMESTR "\n");
#if ARDUINO >= 100
    Serial.flush(); // push out all buffered tx characters
#endif
    status = GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen

#ifndef GLCDCFG_NOINIT_CHECKS
    if(status) // did the initialization fail?
    {
	SerialPrintQ("GLCD initialization Failed: ");
	switch(status)
	{
		case GLCD_EBUSY:
			SerialPrintQ("BUSY wait Timeout");
			break;
		case GLCD_ERESET:
			SerialPrintQ("RESET wait Timeout");
			break;
	}
	SerialPrintQ(" (status code: ");
	Serial.print(status);
	Serial.println(')');
        lfcount++;
    	goto finished;
    }
#endif


    GLCD.SelectFont(System5x7, BLACK);

    SerialPrintQ("Displaying Library version Screen\n");
    showlibinfo();

    SerialPrintQ("Turning display ");
#ifdef glcdPinBL
    SerialPrintQ("& backlight ");
#endif
    SerialPrintQ("on/off\n");

    for(uint8_t x = 0; x < 5; x++)
    {
	GLCD.Off(); // turn off display pixels and backlight(if supported)
	delay(300);
	GLCD.On(); // turn on display pixels and backlight(if supported)
	delay(600);
    }
    delay(2000);
    

    SerialPrintQ("Displaying ChipSelect Screens\n");
    showchipselscreen();
    if( lcdmemtest())
    {
      /*
       * memory tests failed.
       */
      SerialPrintQ("TEST FAILED\n");
      lfcount++;
    }
    else
    {
      SerialPrintQ("TEST PASSED\n");

      /*
       * Diags report loop count on completion
       */
      GLCD.ClearScreen();
      GLCD.CursorTo(0,0);
      GLCD.print("Diag Loop:");
      GLCD.println(lcount);
      GLCD.print(lcount - lfcount);
      GLCD.print(" PASSED ");
      GLCD.print(lfcount);
      GLCD.println(" FAILED");

      /*
       * All GLCD tests passed so now
       * perform a GLCD "speed" test.
       */

      glcdspeed = getglcdspeed();
      /*
       * Calculate the speed in K operations/sec
       * Since the speed value reported is 10x the actual value,
       * Dividing by 100 gives the integer K ops/sec
       * Modulo 100 gives the factional K ops/sec
       */

      kops = glcdspeed/100;
      kops_fract = glcdspeed %100;

      GLCD.print("K SetDot/s: ");
      GLCD.print(kops);
      GLCD.print(".");
      GLCD.println(kops_fract);


      SerialPrintQ("GLCD.SetDot() speed (K ops/sec): ");
      Serial.print(kops);
      SerialPrintQ(".");
      Serial.println(kops_fract);
    }

finished:
    Serial.print("Test Count: ");
    Serial.print(lcount - lfcount);
    Serial.print(" PASSED ");
    Serial.print(lfcount);
    Serial.println(" FAILED");

    delay(5000);
    lcount++;
  }
}


uint8_t
lcdmemtest(void)
{
  uint8_t errors = 0;

  SerialPrintQ("Walking 1s data test\n");

  errors = lcdw1test();
  if(errors)
    return(errors);

  SerialPrintQ("Wr/Rd Chip Select Test\n");

  errors = lcdrwseltest();
  if(errors)
    return(errors);

  GLCD.ClearScreen();

  SerialPrintQ("Testing GLCD memory pages\n");

  for(uint8_t chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    uint8_t col = chip2x1(chip);
    uint8_t ecol = chip2x2(chip);

    if(col >= CHIP_WIDTH)
      GLCD.CursorToXY(0,chip2y1(chip));
    else
      GLCD.CursorToXY(CHIP_WIDTH,chip2y1(chip));
    GLCD.print("Chip:");
    GLCD.print((int)chip);

    /*
     * Assumes font is 8 pixels high
     */
    if(col >= CHIP_WIDTH)
      GLCD.CursorToXY(0,chip2y1(chip)+8);
    else
      GLCD.CursorToXY(CHIP_WIDTH,chip2y1(chip)+8);
    GLCD.print((int)col);
    GLCD.print('-');
    GLCD.print((int)ecol);
    delay(500);

//  SerialPrintf("Horizontal Page Test Chip: %d Pixels %d-%d\n", chip, col, ecol);

    SerialPrintQ("Horizontal Page Test Chip: ");
    Serial.print((int)chip);
    SerialPrintQ(" Pixels ");
    Serial.print((int)col);
    Serial.print('-');
    Serial.println((unsigned int)ecol);

    errors += lcdhpagetest(col, ecol, chip2y1(chip)/8, (chip2y2(chip)+1)/8 - 1, 0, 255);


//  SerialPrintf("Vertical Page Test Chip: %d Pixels %d-%d\n", chip, col, ecol);

    SerialPrintQ("Vertical Page Test Chip: ");
    Serial.print((int)chip);
    SerialPrintQ(" Pixels ");
    Serial.print((int)col);
    Serial.print('-');
    Serial.println((int)ecol);

    errors += lcdvpagetest(col, ecol, chip2y1(chip)/8, (chip2y2(chip)+1)/8 - 1, 0, 255);
    GLCD.ClearScreen();

    col += CHIP_WIDTH;
    ecol += CHIP_WIDTH;
    if(ecol > GLCD.Right)
      ecol = GLCD.Right;
  }


  GLCD.CursorTo(0,0);
  GLCD.print("Full Display");
  GLCD.CursorTo(0,1);
  GLCD.print((int)0);
  GLCD.print('-');
  GLCD.print((int)GLCD.Right);
  delay(1000);

//SerialPrintf("Full Module Horizontal Page Test:Pixels %d-%d\n",  0, GLCD.Right);

  SerialPrintQ("Full Module Horizontal Page Test:Pixels ");
  Serial.print(0);
  Serial.print('-');
  Serial.println((int)GLCD.Right);

  errors += lcdhpagetest(0, GLCD.Right, 0, GLCD.Bottom/8, 0, 255);

//SerialPrintf("Full Module Vertical Page Test:Pixels %d-%d\n",  0, GLCD.Right);

  SerialPrintQ("Full Module Vertical Page Test:Pixels ");
  Serial.print(0);
  Serial.print('-');
  Serial.println((int)GLCD.Right);

  errors += lcdvpagetest(0, GLCD.Right, 0, GLCD.Bottom/8, 0, 255);

  GLCD.ClearScreen();

  return(errors);
}

/*
 * Walk a bit through a single memory location to see if
 * basic reads/writes work.
 */

uint8_t
lcdw1test(void)
{
  uint8_t errors = 0;
  uint8_t rdata;

  for(uint8_t pat = 1;  pat != 0; pat <<= 1)
  {
    GLCD.GotoXY(0,0);
    GLCD.WriteData(pat);
    GLCD.GotoXY(0,0);
    rdata = GLCD.ReadData();

    if(rdata != pat)
    {
//    eprintf(" Compare error: %x != %x\n", rdata, pat);
      SerialPrintQ(" Compare error: ");
      Serial.print((unsigned int)rdata, HEX);
      SerialPrintQ(" != ");
      Serial.println((unsigned int)pat, HEX);

      errors++;
    }
  }
  return(errors);
}

/*
 * LCD read/write chip select test.
 * This test attempts to detect chip select issues by writing the chip number
 * to the lowest page of memory for each chip.
 * This is done incrementing and decrementing.
 * It must be done both ways because when chip selects are wrong, it is possible
 * to write to more than 1 chip at a time. To catch this, you have write do the operation
 * more tha once. Once by writing incrementing addresses and then decrementing addresses.
 */

uint8_t
lcdrwseltest()
{
  uint8_t errors = 0;
  uint8_t rdata; // read data


  for(uint8_t chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    GLCD.WriteData(chip);
  }
  for(uint8_t chip = 0; chip < glcd_CHIP_COUNT; chip++)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    rdata = GLCD.ReadData();
    if(rdata != chip)
    {
//    eprintf(" Compare error: chip:%d %x != %x\n", chip, rdata, chip);
      SerialPrintQ(" Compare error: chip:");
      Serial.print((int)chip);
      Serial.print(' ');
      Serial.print((unsigned int)rdata, HEX);
      SerialPrintQ(" != ");
      Serial.println((unsigned int)chip, HEX);
      errors++;
    }
  }

  for(int chip = glcd_CHIP_COUNT - 1; chip >= 0; chip--)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    GLCD.WriteData(chip);
  }
  for(int chip = glcd_CHIP_COUNT - 1; chip >= 0; chip--)
  {
    GLCD.GotoXY(chip2x1(chip), chip2y1(chip));
    rdata = GLCD.ReadData();
    if(rdata != chip)
    {
//    eprintf(" Compare error: chip:%d  %x != %x\n", chip, rdata, chip);
      SerialPrintQ(" Compare error: chip:");
      Serial.print((int)chip);
      Serial.print(' ');
      Serial.print((unsigned int)rdata, HEX);
      SerialPrintQ(" != ");
      Serial.println((unsigned int)chip, HEX);
      errors++;
    }
  }

  return(errors);
}


/*
 * Walk incrementing values through incrementing memory locations.
 * 
 * A value starting at sval ending at eval will be walked through memory
 * pages horizontally.
 * The starting x location will be filled in with sval and the value will
 * incremented through all locations to be tested. Values are written through
 * incrementing x values and when the maximum x value is reached on a row/page,
 * writing is continued down on the next row/page.
 *
 * All the values are read and compared to expected values.
 *
 * Then process starts over again by incrementing the starting value.
 * This repeats until the starting value reaches the ending value.
 *
 * Each memory location will tested with an incrementing value evel-sval+1 times.
 *
 * If sval is 0 and eval is 255, every memory location will be tested for every value.
 *
 */


int
lcdhpagetest(uint8_t x1, uint8_t x2, uint8_t spage, uint8_t epage, uint8_t sval, uint8_t eval)
{
  uint8_t x;
  uint8_t data;
  uint8_t rdata;
  uint8_t page;
  uint8_t errors = 0;

  /*
   * perform each interation of test across memory with an incrementing pattern
   * starting at sval and bumping sval each iteration.
   */
  do
  {
    /*
     * write out all glcd memory pages
     * sequentially through incrementing columns (x values)
     */

    data = sval;
    for(page = spage; page <= epage; page++)
    {

      GLCD.GotoXY(x1, page * 8);
      for(x = x1; x <= x2; x++)
      {
        /*
	 * GotoXY() is intentially not done here in the loop to 
         * let the hardware bump its internal address.
         * This ensures that the glcd code and hardware are
         * properly tracking each other.
         */
        GLCD.WriteData(data);
        data++;
      }
    }

    /*
     * Now go back and verify the pages
     */

    data = sval;
    for(page = spage; page <= epage; page++)
    {

      for(x = x1; x<= x2; x++)
      {
        /*
	 * Reads don't auto advance X
         */
        GLCD.GotoXY(x, page * 8);
        rdata = GLCD.ReadData();

        if(data != rdata)
        {
//        eprintf(" Verify error: (%d,%d) %x!=%x\n", x, spage*8, data, rdata);
          SerialPrintQ(" Verify error: (");
          Serial.print((unsigned int) x);
          Serial.print(',');
          Serial.print((unsigned int) (spage*8));
          SerialPrintQ(") ");
          Serial.print((unsigned int)data, HEX);
          SerialPrintQ("!=");
          Serial.println((unsigned int)rdata, HEX);

          if(++errors > MAX_ERRORS)
            return(errors);
        }
        data++;
      }
    }
  } 
  while(sval++ != eval);
  return(errors);
}

/*
 * Walk incrementing values through vertical memory page locations.
 * 
 * A value starting at sval ending at eval will be walked through memory pages
 * Vertically.
 * The starting x location will be filled in with sval and the value will
 * incremented through all memory pages to be tested. Values are written through
 * incrementing row/page values and when the maximum row/page value is reached,
 * writing is continued at the top page of the next column/x location.
 *
 * All the values are read and compared to expected values.
 *
 * Then process starts over again by incrementing the starting value.
 * This repeats until the starting value reaches the ending value.
 *
 * Each memory location will tested with an incrementing value evel-sval+1 times.
 *
 * If sval is 0 and eval is 255, every memory location will be tested for every value.
 *
 */


int
lcdvpagetest(uint8_t x1, uint8_t x2, uint8_t spage, uint8_t epage, uint8_t sval, uint8_t eval)
{
  uint8_t x;
  uint8_t data;
  uint8_t rdata;
  uint8_t page;
  uint8_t errors = 0;

  /*
   * perform each interation of test across memory with an incrementing pattern
   * starting at sval and bumping sval each iteration.
   */
  do
  {
    /*
     * write out all glcd memory pages
     * sequentially through incrementing columns (x values)
     */

    data = sval;
    for(x = x1; x <= x2; x++)
    {
      for(page = spage; page <= epage; page++)
      {
        GLCD.GotoXY(x, page * 8);
        GLCD.WriteData(data);
        data++;
      }
    }

    /*
     * Now go back and verify the pages
     */

    data = sval;
    for(x = x1; x<= x2; x++)
    {
      for(page = spage; page <= epage; page++)
      {
        GLCD.GotoXY(x, page * 8);
        rdata = GLCD.ReadData();

        if(data != rdata)
        {
//        eprintf(" Verify error: (%d,%d) %x!=%x\n", x, spage*8, data, rdata);

          SerialPrintQ(" Verify error: (");
          Serial.print((unsigned int) x);
          Serial.print(',');
          Serial.print((unsigned int) (spage*8));
          SerialPrintQ(") ");
          Serial.print((unsigned int)data, HEX);
          SerialPrintQ("!=");
          Serial.println((unsigned int)rdata, HEX);

          if(++errors > MAX_ERRORS)
            return(errors);
        }
        data++;
      }
    }
  } 
  while(sval++ != eval);
  return(errors);
}

/*
 * Dump various configuration information out
 * the serial port.
 */

void
showGLCDconfig(void)
{
#ifdef ARDUINO
  SerialPrintP(hline);
  SerialPrintQ("Reported Arduino Revision: ");
#if ARDUINO > 158 // ARDUINO rev format changed after 1.5.8 to #.##.## (breaks after 3.x.x for 16 int bit calc)
  Serial.print(ARDUINO/10000);
  Serial.print('.');
  Serial.print((ARDUINO%10000)/100);
  Serial.print('.');
  Serial.println((ARDUINO%10000)%100);
#elif ARDUINO >= 100 // 1.0.0 to 1.5.8 uses rev format #.#.#
  Serial.print(ARDUINO/100);
  Serial.print('.');
  Serial.print((ARDUINO%100)/10);
  Serial.print('.');
  Serial.println((ARDUINO%100)%10);
#else
  SerialPrintQ("0.");
  Serial.println(xstr(ARDUINO)); // print the raw string vs as a number
#endif

#endif

  SerialPrintQ("F_CPU: ");
  Serial.println(F_CPU);

  SerialPrintP(hline);
  SerialPrintQ("Lib Configuration: ");
  SerialPrintP(GLCDlibName);
  SerialPrintQ(" ver: ");
  SerialPrintP(GLCDlibVersion);
  SerialPrintQ(" build: ");
  SerialPrintP(GLCDlibBuild);
  Serial.println();
#ifdef GLCD_GLCDLIB_DATESTR
  SerialPrintQ("Lib build date: ");
  SerialPrintQ(GLCD_GLCDLIB_DATESTR);
  Serial.println();
#endif
#ifdef GLCD_GLCDLIB_BUILDSTR
  SerialPrintQ("Lib build number: ");
  SerialPrintQ(GLCD_GLCDLIB_BUILDSTR);
  Serial.println();
#endif

/*
 * ifdef to support manual config files vs auto config files 
 */

#ifdef glcd_ConfigName
  SerialPrintQ("Config File:");
  SerialPrintQ(glcd_ConfigName);
#else
  SerialPrintQ("Panel Configuration:");
  SerialPrintQ(glcd_PanelConfigName);
  Serial.println();
  SerialPrintQ("Pin Configuration:");
  SerialPrintQ(glcd_PinConfigName);
#endif
  Serial.println();
  SerialPrintP(hline);

  SerialPrintQ("GLCD:");
  SerialPrintQ(glcdDev_DeviceName);

//SerialPrintf("DisplayWidth:%d DisplayHeight:%d\n", GLCD.Width, GLCD.Height);
  SerialPrintQ(" DisplayWidth:");
  Serial.print((int)GLCD.Width);
  SerialPrintQ(" DisplayHeight:");
  Serial.println((int)GLCD.Height);

//SerialPrintf("Chips:%d", glcd_CHIP_COUNT);
  SerialPrintQ("Chips:");
  Serial.print(glcd_CHIP_COUNT);


//SerialPrintf(" ChipWidth:%3d ChipHeight:%2d\n", CHIP_WIDTH, CHIP_HEIGHT);
  SerialPrintQ(" ChipWidth:");
  Serial.print(CHIP_WIDTH);
  SerialPrintQ(" ChipHeight:");
  Serial.println(CHIP_HEIGHT);

#ifdef glcdPinCSEL1
  SerialPrintQ(" CSEL1:");
  SerialPrintPINstr(glcdPinCSEL1);
#endif
#ifdef glcdPinCSEL2
  SerialPrintQ(" CSEL2:");
  SerialPrintPINstr(glcdPinCSEL2);
#endif
#ifdef glcdPinCSEL3
  SerialPrintQ(" CSEL3:");
  SerialPrintPINstr(glcdPinCSEL3);
#endif
#ifdef glcdPinCSEL4
  SerialPrintQ(" CSEL4:");
  SerialPrintPINstr(glcdPinCSEL4);
#endif

#if defined(glcdPinCSEL1) || defined(glcdPinCSEL2) || defined(glcdPinCSEL3) || defined(glcdPinCSEL4)
  Serial.println();
#endif


#ifdef glcdPinRES
  SerialPrintQ(" RES:");
  SerialPrintPINstr(glcdPinRES);
#endif
  SerialPrintQ(" RW:");
  SerialPrintPINstr(glcdPinRW);

  SerialPrintQ(" DI:");
  SerialPrintPINstr(glcdPinDI);

#ifdef glcdPinEN
  SerialPrintQ(" EN:");
  SerialPrintPINstr(glcdPinEN);
#endif

#ifdef glcdPinE1
  SerialPrintQ(" E1:");
  SerialPrintPINstr(glcdPinE1);
#endif
#ifdef glcdPinE2
  SerialPrintQ(" E2:");
  SerialPrintPINstr(glcdPinE2);
#endif
#ifdef glcdPinE3
  SerialPrintQ(" E3:");
  SerialPrintPINstr(glcdPinE3);
#endif
#ifdef glcdPinE4
  SerialPrintQ(" E4:");
  SerialPrintPINstr(glcdPinE4);
#endif
  Serial.println();

//  SerialPrintf(" D0:%s", GLCDdiagsPIN2STR(glcdPinData0));
  SerialPrintQ(" D0:");
  SerialPrintPINstr(glcdPinData0);

  SerialPrintQ(" D1:");
  SerialPrintPINstr(glcdPinData1);

  SerialPrintQ(" D2:");
  SerialPrintPINstr(glcdPinData2);

  SerialPrintQ(" D3:");
  SerialPrintPINstr(glcdPinData3);

  Serial.println();

  SerialPrintQ(" D4:");
  SerialPrintPINstr(glcdPinData4);

  SerialPrintQ(" D5:");
  SerialPrintPINstr(glcdPinData5);

  SerialPrintQ(" D6:");
  SerialPrintPINstr(glcdPinData6);

  SerialPrintQ(" D7:");
  SerialPrintPINstr(glcdPinData7);

  Serial.println();

//  SerialPrintf("Delays: tDDR:%d tAS:%d tDSW:%d tWH:%d tWL: %d\n",
//  GLCD_tDDR, GLCD_tAS, GLCD_tDSW, GLCD_tWH, GLCD_tWL);

#ifdef _delayNanoseconds
  SerialPrintQ("Delays: tDDR:");
  Serial.print(GLCD_tDDR);
  SerialPrintQ(" tAS:");
  Serial.print(GLCD_tAS);
  SerialPrintQ(" tDSW:");
  Serial.print(GLCD_tDSW);
  SerialPrintQ(" tWH:");
  Serial.print(GLCD_tWH);
  SerialPrintQ(" tWL:");
  Serial.println(GLCD_tWL);
#else
  SerialPrintQ("Delays: Using Arduino compatible delay functions\n");
#endif


#ifdef glcd_CHIP0
  SerialPrintQ("ChipSelects:\n");
  SerialPrintQ(" CHIP0:(");
  SerialPrintQ(xstr(glcd_CHIP0));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP1
  SerialPrintQ(" CHIP1:(");
  SerialPrintQ(xstr(glcd_CHIP1));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP2
  SerialPrintQ(" CHIP2:(");
  SerialPrintQ(xstr(glcd_CHIP2));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP3
  SerialPrintQ(" CHIP3:(");
  SerialPrintQ(xstr(glcd_CHIP3));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP4
  SerialPrintQ(" CHIP4:(");
  SerialPrintQ(xstr(glcd_CHIP4));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP5
  SerialPrintQ(" CHIP5:(");
  SerialPrintQ(xstr(glcd_CHIP5));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP6
  SerialPrintQ(" CHIP6:(");
  SerialPrintQ(xstr(glcd_CHIP6));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP7
  SerialPrintQ(" CHIP7:(");
  SerialPrintQ(xstr(glcd_CHIP7));
  SerialPrintQ(")\n");
#endif
#ifdef glcd_CHIP8
  SerialPrintQ(" CHIP8:(");
  SerialPrintQ(xstr(glcd_CHIP8));
  SerialPrintQ(")\n");
#endif

  SerialPrintQ("Data mode: ");

#ifdef _AVRIO_AVRIO_
  /*
   * Show AVRIO GLCD data mode
   *
   * Requires getting down and dirty and mucking around done
   * in avrio land.
   */

  /*
   * First check for full 8 bit mode
   *
   */
  if(AVRDATA_8BIT(glcdPinData0, glcdPinData1, glcdPinData2, glcdPinData3,
  glcdPinData4, glcdPinData5, glcdPinData6, glcdPinData7))
  {
    /*
     * full 8 bit mode
     */
    SerialPrintQ("byte");
  }
  else
  {
    SerialPrintQ("\n d0-d3:");
    if(AVRDATA_4BITHI(glcdPinData0, glcdPinData1, glcdPinData2, glcdPinData3) ||
      AVRDATA_4BITLO(glcdPinData0, glcdPinData1, glcdPinData2, glcdPinData3))
    {
      SerialPrintQ("nibble mode");
#ifndef GLCDCFG_ATOMIC_IO
      SerialPrintQ("-Non-Atomic");
#else
      SerialPrintQ("-disabled (ATOMIC MODE)"); // for now this "knows" avrio disabled nibbles when in atomic mode.
#endif
    }
    else
    {
      SerialPrintQ("bit i/o");
    }

    SerialPrintQ("\n d4-d7:");

    if(AVRDATA_4BITHI(glcdPinData4, glcdPinData5, glcdPinData6, glcdPinData7) ||
      AVRDATA_4BITLO(glcdPinData4, glcdPinData5, glcdPinData6, glcdPinData7))
    {
      SerialPrintQ("nibble mode");
#ifndef GLCDCFG_ATOMIC_IO
      SerialPrintQ("-Non-Atomic");
#else
      SerialPrintQ("-disabled (ATOMIC MODE)"); // for now this "knows" avrio disabled nibbles when in atomic mode.
#endif
    }
    else
    {
      SerialPrintQ("bit i/o");
    }
  }

#else
/*
 * FIXME: need to autodect teensy3 core
 */
#if defined(GLCDIO_DIGITALXXXFAST)
    SerialPrintQ("digitalxxxFast"); // "fast" write/read mode
#else
    SerialPrintQ("Core-Code"); // working in core-code compability mode
#endif
#endif
    Serial.println();

  /*
   * Show backlight support
   */
  SerialPrintQ("Backlight: ");
#ifdef glcdPinBL
  SerialPrintPINstr(glcdPinBL);
  SerialPrintQ(" ActiveLevel: ");
  if(glcd_BLactlevel)
	SerialPrintQ("HIGH");
  else
	SerialPrintQ("LOW");
#else
  SerialPrintQ("<Not configured>");
#endif
  Serial.println();

  /*
   * show no scroll down if disabled.
   */

#ifdef GLCDCFG_NO_SCROLLDOWN
  SerialPrintQ("NO Down Scroll\n");
#endif

  /*
   * show READ CACHE if enabled
   */
#ifdef GLCDCFG_READ_CACHE
  SerialPrintQ("READ CACHE enabled\n");
#endif

  /*
   * show if UTF8 support is enabled
   */
#ifdef GLCDCFG_UTF8
  SerialPrintQ("UTF8 support enabled\n");
#endif

  /*
   * show if NO_PRINTF
   */
#ifdef GLCDCFG_NO_PRINTF
  SerialPrintQ("NO_PRINTF\n");
#endif


  /*
   * show if NO_SCROLLDOWN
   */
#ifdef GLCDCFG_NO_SCROLLDOWN
  SerialPrintQ("NO_SCROLLDOWN\n");
#endif

  /*
   * show if NODEFER_SCROLL
   */
#ifdef GLCDCFG_NODEFER_SCROLL
  SerialPrintQ("NODEFER_SCROLL\n");
#endif


  /*
   * show if init selftest disabled
   */
#ifdef GLCDCFG_NOINIT_CHECKS
  SerialPrintQ("NOINIT_CHECKS\n");
#endif

  /*
   * show if forcing CORECODE mode
   */
#ifdef GLCDCFG_FORCE_CORECODE
  SerialPrintQ("FORCE CORECODE mode\n");
#endif


}

#ifdef _AVRIO_AVRIO_
// figure out arduino pin from raw pin info

int avrpin2arduinopin(avrpin_t avriopin)
{

	for(int pin = 0; pin < NUM_DIGITAL_PINS; pin++)
	{
		if(AVRIO_PIN2AVRPIN(pin) == avriopin)
			return(pin);
	}

	return(0); // this should never happen
}


/*
 * The avrio version of the pin string also contain
 * the AVR port and bit number of the pin.
 * The format is AVRPIN_Pb where P is the port A-Z 
 * and b is the bit number within the port 0-7
 */
void _SerialPrintPINstr(avrpin_t pin, const char *pinstr, uint8_t avrport, uint8_t avrbit)
{

  /*
   * Check to see if Ardino pin# is used or
   * if AVRPIN #s are used.
   */
  if(pin >= AVRIO_PIN(AVRIO_PORTA, 0))
  {
//  SerialPrintf("0x%x", pin);

	Serial.print(avrpin2arduinopin(pin));
  }
  else
  {
//  SerialPrintf("%s", pinstr);
    Serial.print(pinstr);
  }

//SerialPrintf("(AVRPIN_%c%d)", pin, 'A'-AVRIO_PORTA+avrport, avrbit);

  SerialPrintQ("(PORT");
  Serial.print((char)('A' - AVRIO_PORTA+avrport));
  Serial.print((int)avrbit);
  Serial.print(')');

}
#else
void _SerialPrintPINstr(const char *pinstr)
{
  Serial.print(pinstr);
}
#endif


/*
 * This function returns a composite "speed" of the glcd
 * by returning the SetDot() speed in 1/10 operations/sec.
 * i.e. return value is 1/10 the number of SetDot() calls
 * per second.
 */
uint16_t getglcdspeed()
{
uint16_t iter = 0;
unsigned long startmillis;

	  startmillis = millis();

	  while(millis() - startmillis < 1000) // loop for 1 second
	  {
	    /*
	     * Do 10 operations to minimize the effects of the millis() call
	     * and the loop.
	     *
	     * Note: The pixel locations were chosen to ensure that a
	     * a set colum and set page operation are needed for each SetDot()
	     * call.
	     * The intent is to get an overall feel for the speed of the GLD
	     * as each SetDot() call will do these operations to the glcd:
	     * - set page
	     * - set column
	     * - read byte (dummy read)
	     * - read byte (real read)
	     * - set column (set column back for write)
	     * - write byte
	     */

	    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
	    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
	    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
	    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
	    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
	    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
	    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
	    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
	    GLCD.SetDot(GLCD.Right, GLCD.Bottom, WHITE);
	    GLCD.SetDot(GLCD.Right-1, GLCD.Bottom-9, WHITE);
	    iter++;
	  }

	  return(iter);
}
