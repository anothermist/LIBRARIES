//Digole Digital Solutions: www.digole.com
#ifndef DigoleSerialDisp_h
#define DigoleSerialDisp_h

#include <inttypes.h>
#include "Print.h"
#include <Wire.h>
#include "Arduino.h"
//#include <avr/wdt.h>
// Communication set up command
// Text function command
// Graph function command
#define Serial_UART 0
#define Serial_I2C 1
#define Serial_SPI 2
#define _TEXT_ 0
#define _GRAPH_ 1
#ifdef FLASH_CHIP   //if writing to flash chip
#define _WRITE_DELAY 40
#else   //if writing to internal flash
#define _WRITE_DELAY 100
#endif
#ifndef Ver
#define Ver 40
#endif
class Digole {
public:

    virtual size_t read1(void);
    virtual void cleanBuffer(void);
//    virtual int readInt(void);
};

class DigoleSerialDisp : public Print, public Digole {
public:
#if defined(_Digole_Serial_UART_)

    DigoleSerialDisp(HardwareSerial *s, unsigned long baud) //UART set up
    {
        _mySerial = s;
        _Baud = baud;
    }

    size_t write(uint8_t value) {
        _mySerial->write((uint8_t) value);
        return 1; // assume sucess
    }

    void begin(void) {
        _mySerial->begin(9600);
        _mySerial->print("SB");
        _mySerial->println(_Baud);
        delay(100);
        _mySerial->begin(_Baud);
    }

    size_t read1(void) {
        int t;
//        wdt_disable();
        do {
            t = _mySerial->read();
        } while (t == -1);
        return t;
    }
    void cleanBuffer(void)  //clean UART receiving buffer
    {
        while(_mySerial->read()!=-1);
    }
#endif
#if defined(_Digole_Serial_I2C_)

    void begin(void) {
        _myWire->begin();
    }

    DigoleSerialDisp(TwoWire *s, uint8_t add) //U2C set up
    {
        _myWire = s;
        _I2Caddress = add;
    }

    size_t write(uint8_t value) {
        _myWire->beginTransmission(_I2Caddress);
        _myWire->write((uint8_t) value); //use yourself routing to send bulk of data, such as for…
        _myWire->endTransmission();
        return 1; // assume sucess
    }


    size_t read1(void) {
        int t;
//        wdt_disable();
        _myWire->requestFrom((int) _I2Caddress, 1);
        while (_myWire->available() == 0);
        t = _myWire->read(); //use yourself routing to read bulk of data, such as for…
        return t; // assume sucess
    }
    void cleanBuffer(void)
    {
    }
#endif
#if defined(_Digole_Serial_SPI_)

    void begin(void) {
        pinMode(_Clockpin, OUTPUT);
        pinMode(_Datapin, OUTPUT);
        pinMode(_SSpin, OUTPUT);
        digitalWrite(_SSpin, HIGH);
#if Ver==33 //sorry, we made wrong mode in Ver 3.3 firmware
        digitalWrite(_Clockpin, HIGH);
#else
        digitalWrite(_Clockpin, LOW);
#endif
        digitalWrite(_Datapin, LOW);
        if (_SIpin != 0) {
            pinMode(_SIpin, INPUT);
        }
    }

    DigoleSerialDisp(uint8_t pin_data, uint8_t pin_clock, uint8_t SS, uint8_t SI = 0) //spi set up
    {
        _Clockpin = pin_clock;
        _Datapin = pin_data;
        _SSpin = SS;
        _SIpin = SI;
        begin();
/*        pinMode(_Clockpin, OUTPUT);
        pinMode(_Datapin, OUTPUT);
        pinMode(_SSpin, OUTPUT);
        digitalWrite(_SSpin, HIGH);
#if Ver==33 //sorry, we made wrong mode in Ver 3.3 firmware
        digitalWrite(_Clockpin, HIGH);
#else
        digitalWrite(_Clockpin, LOW);
#endif
        digitalWrite(_Datapin, LOW);
        if (_SIpin != 0) {
            pinMode(_SIpin, INPUT);
        }
 */
    }

    size_t write(uint8_t value) {
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        shiftOut(_Datapin, _Clockpin, MSBFIRST, value);
#if Ver==33
        digitalWrite(_Clockpin, HIGH);
#endif
        digitalWrite(_SSpin, HIGH);
        return 1; // assume sucess
    }

    size_t read1(void) {
        int t = 0;
        char c;
//        wdt_disable();
        digitalWrite(_SSpin, HIGH);
        digitalWrite(_SSpin, HIGH);
        while (digitalRead(_SIpin) == LOW) ; //check to see the data is ready(1) or not(0)
        digitalWrite(_SSpin, LOW); //tell display module I will read data
        digitalWrite(_SSpin, LOW); //delay about 5us to wait module prepare data
        digitalWrite(_SSpin, LOW); 
        for (c = 8; c > 0; c = c - 1) {
            t <<= 1;
#if Ver==33
            digitalWrite(_Clockpin, LOW);
            if (digitalRead(_SIpin))
                t |= 1;
            digitalWrite(_Clockpin, HIGH);
#else
            digitalWrite(_Clockpin, HIGH);
            if (digitalRead(_SIpin))
                t |= 1;
            digitalWrite(_Clockpin, LOW);
#endif
        }

        //	t=shiftIn(_Datapin, _Clockpin, MSBFIRST);
        digitalWrite(_SSpin, HIGH);
        return t; // assume sucess
    }
    void cleanBuffer(void)
    {
    }
#endif

    /*---------fucntions for Text and Graphic LCD adapters---------*/
    void writeStr(const char *s) {
        int i = 0;
        while (s[i] != 0)
            write(s[i++]);
    }

    void disableCursor(void) {
        writeStr("CS0");
    }

    void enableCursor(void) {
        writeStr("CS1");
    }

    void clearScreen(void) {
        writeStr("CL");
    }

    void setI2CAddress(uint8_t add) {
        writeStr("SI2CA");
        write(add);
        _I2Caddress = add;
    }

    void displayConfig(uint8_t v) {
        writeStr("DC");
        write(v);
    }
    //print function

    void println(const __FlashStringHelper *v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(const String &v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(const char v[]) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(char v) {
        preprint();
        Print::println(v);
        Print::println("\x0dTRT");
    }

    void println(unsigned char v, int base = DEC) {
        preprint();
        Print::println(v, base);
        Print::println("\x0dTRT");
    }

    void println(int v, int base = DEC) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(unsigned int v, int base = DEC) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(long v, int base = DEC) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(unsigned long v, int base = DEC) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(double v, int base = 2) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(const Printable& v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void println(void) {
        write((uint8_t) 0);
        writeStr("TRT");
    }

    void print(const __FlashStringHelper *v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(const String &v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(const char v[]) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(char v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(unsigned char v, int base = DEC) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(int v, int base = DEC) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(unsigned int v, int base = DEC) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(long v, int base = DEC) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(unsigned long v, int base = DEC) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(double v, int base = 2) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(const Printable& v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }
    void preprint(void);
    int readInt(void);
    /*----------Functions for Graphic LCD/OLED adapters only---------*/
    //the functions in this section compatible with u8glib
    void drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
	void drawBitmap65K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void setTrueColor(uint8_t r, uint8_t g, uint8_t b);
    void setRot90(void);
    void setRot180(void);
    void setRot270(void);
    void undoRotation(void);
    void setRotation(uint8_t);
    void setContrast(uint8_t);
    void drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t = 0);
    void drawDisc(unsigned int x, unsigned int y, unsigned int r);
    void drawFrame(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void drawPixel(unsigned int x, unsigned int y, uint8_t color);
    void drawPixel(unsigned int x, unsigned int y);
    void drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1);
    void drawLineTo(unsigned int x, unsigned int y);
    void drawHLine(unsigned int x, unsigned int y, unsigned int w);
    void drawVLine(unsigned int x, unsigned int y, unsigned int h);
    //-------------------------------
    //special functions for our adapters
    //void uploadStartScreen(int lon, const unsigned char *data); //upload start screen
    void setFont(uint8_t font); //set font, availale: 6,10,18,51,120,123, user font: 200-203
    void nextTextLine(void); //got to next text line, depending on the font size
    void setColor(uint8_t); //set color for graphic function
    void backLightOn(void); //Turn on back light
    void backLightOff(void); //Turn off back light
    void screenOnOff(uint8_t); //turn screen on/off
    void cpuOff(void); //put MCU in sleep, it will wake up when new data received
    void moduleOff(void); //put whole module in sleep: Back light off, screen in sleep, MCU in sleep
    void backLightBrightness(uint8_t); //set backlight brightness,0~100

    void directCommand(uint8_t d); //send command to LCD drectly
    void directData(uint8_t d); //send data to LCD drectly
    void moveArea(unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, char xoffset, char yoffset); //move a area of screen to another place
    void drawStr(unsigned int x, unsigned int y, const char *s);
    void setPrintPos(unsigned int x, unsigned int y, uint8_t = 0);
    void setLCDColRow(uint8_t col, uint8_t row);
    void setTextPosAbs(unsigned int x, unsigned int y);
    /*-----Touch screen functions---*/
    void calibrateTouchScreen(void);
    void readTouchScreen(void);
    void readTouchScreen(int *x,int *y);
    void readClick(void);
    void readClick(int *x,int *y);
    int readBattery(void);
    int readAux(void);
    int readTemperature(void);

    /*-----Flash memory functions---*/
    void flashErase(unsigned long int addr, unsigned long int length);
    void flashReadStart(unsigned long int addr, unsigned long int len);
    void setFlashFont(unsigned long int addr);
    void runCommandSet(unsigned long int addr);
    size_t read(void);

    void write2B(unsigned int v);
    //--- new function on V3.3 firmware ----//
    void writeE2prom(unsigned int addr, unsigned int len, unsigned char *data);
    void readE2prom(unsigned int addr, unsigned int len);

    //--- new function on V3.0 firmware ----//

    void setBgColor(uint8_t color) //set current color as background
    {

#if Ver>32
        writeStr("BGC");
        write(color);
#else
        writeStr("SC");
        write(color);
        writeStr("BGC");
#endif
    }

    void setDrawWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        writeStr("DWWIN");
        write2B(x);
        write2B(y);
        write2B(width);
        write2B(height);
    }

    void resetDrawWindow(void) {
        writeStr("RSTDW");
    }

    void cleanDrawWindow(void) {
        writeStr("WINCL");
    }
    //---end of V3.0 functions

    void displayStartScreen(uint8_t m) {
        writeStr("DSS");
        write(m);
    } //display start screen

    void setMode(uint8_t m) {
        writeStr("DM");
        write(m);
    } //set display mode

    void setTextPosBack(void) {
        writeStr("ETB");
    } //set text position back to previous, only one back allowed

    void setTextPosOffset(char xoffset, char yoffset) {
        writeStr("ETO");
        write(xoffset);
        write(yoffset);
    }

    void setLinePattern(uint8_t pattern) {
        writeStr("SLP");
        write(pattern);
    }

    void setLCDChip(uint8_t chip) { //only for universal LCD adapter
        writeStr("SLCD");
        write(chip);
    }

    void setBackLight(uint8_t bl) {
        writeStr("BL");
        write(bl);
    }

    void digitalOutput(uint8_t x) {
        writeStr("DOUT");
        write(x);
    }

    void flushScreen(uint8_t bl) {
        writeStr("FS");
        write(bl);
    }

    void downloadStartScreen(int lon, const unsigned char *data) {
        int j;
        unsigned char b;
        uint8_t c;
        writeStr("SSS");
        lon++;
        write((uint8_t) (lon / 256));
        write((uint8_t) (lon % 256));
        delay(300);
        b = 0;
        for (j = 0; j < (lon - 1); j++) {
            c = pgm_read_byte_near(data + j);
            write(c);
            if ((++b) == 64) {
                b = 0, delay(_WRITE_DELAY);
            }
        }
        write(255); //indicater of end of it
        delay(_WRITE_DELAY);
    }

    void downloadUserFont(int lon, const unsigned char *data, uint8_t sect) {
        uint8_t c;
        unsigned char b;
        writeStr("SUF");
        write(sect);
        delay(300);
#if Ver>32
        write((uint8_t) (lon / 256));
        write((uint8_t) (lon % 256));
#else
        write((uint8_t) (lon % 256));
        write((uint8_t) (lon / 256));
#endif
        b = 0;
        for (int j = 0; j < lon; j++) {
            c = pgm_read_byte_near(data + j);
            write(c);
            if ((++b) == 64) {
                b = 0, delay(_WRITE_DELAY);
            }
        }
    }

    void flashWrite(unsigned long int addr, unsigned long int len, const unsigned char *data) {
        unsigned char c, b;
        unsigned long int i;
        writeStr("FLMWR");
        write(addr >> 16);
        write(addr >> 8);
        write(addr);
        write(len >> 16);
        write(len >> 8);
        write(len);
        b = 0;
        for (i = 0; i < len; i++) {
            c = pgm_read_byte(data + i);
            write(c);
            if ((++b) == 64) {
                b = 0, delay(_WRITE_DELAY);
            }
        }
        delay(_WRITE_DELAY);
#ifdef FLASH_CHIP
        //check write memory done
        while (read1() != 17);
#endif
    }

    void flashWrite(unsigned long int addr, unsigned long int len, unsigned char *data) {
        unsigned char c, b;
        unsigned long int i;
        writeStr("FLMWR");
        write(addr >> 16);
        write(addr >> 8);
        write(addr);
        write(len >> 16);
        write(len >> 8);
        write(len);
        b = 0;
        for (i = 0; i < len; i++) {
            c = data[i];
            write(c);
            if ((++b) == 64) {
                b = 0, delay(_WRITE_DELAY);
            }
        }
#ifdef FLASH_CHIP
        //check write memory done
        while (read1() != 17);
#endif
    }
    void manualCommand(unsigned char c)
    {
        writeStr("MCD");
        write(c);
    }
    void manualData(unsigned char d)
    {
        writeStr("MDT");
        write(d);
    }
    void setSPIMode(unsigned char mode) {
        if (mode < 4) {
            writeStr("SPIMD");
            write(mode);
        }
    }
private:
    unsigned long _Baud;
    HardwareSerial *_mySerial;
    uint8_t _I2Caddress;
    TwoWire *_myWire;
    uint8_t _Clockpin;
    uint8_t _Datapin;
    uint8_t _SSpin;
    uint8_t _SIpin;
    uint8_t _Comdelay;
};

#endif
