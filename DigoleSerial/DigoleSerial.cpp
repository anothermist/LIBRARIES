//Digole Digital Solutions: www.digole.com
#include "DigoleSerial.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/*
// Communication set up command
 * "SB":Baud (ascII bytes end with 0x00/0x0A/0x0D) -- set UART Baud Rate
 * "SI2CA":Address(1 byte <127) -- Set I2C address, default address is:0x27
 * "DC":1/0(1byte) -- set config display on/off, if set to 1, displayer will display current commucation setting when power on
// Text Function command
 * "CL": -- Clear screen--OK
 * "CS":1/0 (1 byte)-- Cursor on/off
 * "TP":x(1 byte) y(1 byte) -- set text position
 * "TT":string(bytes) end with 0x00/0x0A/0x0D -- display string under regular mode
// Graphic function command
 * "GP":x(1byte) y(1byte) -- set current graphic position
 * "DM":"C/!/~/&/|/^"(ASCII 1byte) -- set drawing mode--C="Copy",! and ~ = "Not", & = "And", | = "Or", ^ = "Xor"
 * "SC":1/0 (1byte) -- set draw color--only 1 and 0
 * "LN":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw line from x0,y0 to x1,y1,set new pot to x1,y1
 * "LT":x(1byte) y(1byte) -- draw line from current pos to x,y
 * "CC":x(1byte) y(1byte) ratio(byte) -- draw circle at x,y with ratio
 * "DP":x(1byte) y(1byte) Color(1byte) -- draw a pixel--OK
 * "DR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw rectangle, top-left:x0,y0; right-bottom:x1,y1
 * "FR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw filled rectangle, top-left:x0,y0; right-bottom:x1,y1
 */

// that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

//UART function
int DigoleSerialDisp::readInt(void)
{
    int a;
    a=read1()<<8;
    a+=read1();
    return a;
}
void DigoleSerialDisp::write2B(unsigned int v) {
    if (v < 255)
        write(v);
    else {
        write(255);
        write(v - 255);
    }
}

void DigoleSerialDisp::preprint(void) {
    //write((uint8_t)0);
    writeStr("TT");
}

/*----------Functions for Graphic LCD/OLED adapters only---------*/
void DigoleSerialDisp::drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) {
    uint8_t i = 0;
    if ((w & 7) != 0)
        i = 1;
    writeStr("DIM");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned int j = 0; j < h * ((w >> 3) + i); j++) {
        write(pgm_read_byte_near(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::setRot90(void) {
    writeStr("SD1");
}

void DigoleSerialDisp::setRot180(void) {
    writeStr("SD2");
}

void DigoleSerialDisp::setRot270(void) {
    writeStr("SD3");
}

void DigoleSerialDisp::undoRotation(void) {
    writeStr("SD0");
}

void DigoleSerialDisp::setRotation(uint8_t d) {
    writeStr("SD");
    write2B(d);
}

void DigoleSerialDisp::setContrast(uint8_t c) {
    writeStr("CT");
    write2B(c);
}

void DigoleSerialDisp::drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    writeStr("FR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t f) {
    writeStr("CC");
    write2B(x);
    write2B(y);
    write2B(r);
    write(f);
}

void DigoleSerialDisp::drawDisc(unsigned int x, unsigned int y, unsigned int r) {
    drawCircle(x, y, r, 1);
}

void DigoleSerialDisp::drawFrame(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    writeStr("DR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawPixel(unsigned int x, unsigned int y) {
    writeStr("DP");
    write2B(x);
    write2B(y);
}
void DigoleSerialDisp::drawPixel(unsigned int x, unsigned int y, uint8_t color) {
    writeStr("SC");
    write(color);
    writeStr("DP");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1) {
//    writeStr("\x1B\x10");
    writeStr("LN");
    write2B(x);
    write2B(y);
    write2B(x1);
    write2B(y1);
}

void DigoleSerialDisp::drawLineTo(unsigned int x, unsigned int y) {
    writeStr("LT");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawHLine(unsigned int x, unsigned int y, unsigned int w) {
    drawLine(x, y, x + w, y);
}

void DigoleSerialDisp::drawVLine(unsigned int x, unsigned int y, unsigned int h) {
    drawLine(x, y, x, y + h);
}

void DigoleSerialDisp::nextTextLine(void) {
    write((uint8_t) 0);
    writeStr("TRT");
}

void DigoleSerialDisp::setFont(uint8_t font) {
    writeStr("SF");
    write(font);
}

void DigoleSerialDisp::setColor(uint8_t color) {
    writeStr("SC");
    write(color);
}

void DigoleSerialDisp::directCommand(uint8_t d) {
    writeStr("MCD");
    write(d);
}

void DigoleSerialDisp::directData(uint8_t d) {
    writeStr("MDT");
    write(d);
}

void DigoleSerialDisp::moveArea(unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, char xoffset, char yoffset) {
    writeStr("MA");
    write2B(x0);
    write2B(y0);
    write2B(w);
    write2B(h);
    write(xoffset);
    write(yoffset);
}

void DigoleSerialDisp::drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
//    uint8_t i = 0;
    writeStr("EDIM1");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned long int j = 0; j < h * w; j++) {
        write(pgm_read_byte(bitmap + j));
        //        delay(5);
    }
}
void DigoleSerialDisp::drawBitmap65K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
//    uint8_t i = 0;
    writeStr("EDIM2");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned long int j = 0; j < h * w*2; j++) {
        write(pgm_read_byte(bitmap+j));
    }
}

void DigoleSerialDisp::drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
//    uint8_t i = 0;
    writeStr("EDIM3");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned long int j = 0; (j < h * w * 3); j++) {
        write(pgm_read_byte(bitmap + j));
    }
}

void DigoleSerialDisp::setTrueColor(uint8_t r, uint8_t g, uint8_t b) { //Set true color
//    uint8_t i = 0;
    writeStr("ESC");
    write(r);
    write(g);
    write(b);
}

void DigoleSerialDisp::drawStr(unsigned int x, unsigned int y, const char *s) {
    writeStr("TP");
    write2B(x);
    write2B(y);
    writeStr("TT");
    writeStr(s);
    write((uint8_t) 0);
}

void DigoleSerialDisp::setPrintPos(unsigned int x, unsigned int y, uint8_t graph) {
    if (graph == 0) {
        writeStr("TP");
        write2B(x);
        write2B(y);
    } else {
        writeStr("GP");
        write2B(x);
        write2B(y);
    }
}

void DigoleSerialDisp::setLCDColRow(uint8_t col, uint8_t row) {
    writeStr("STCR");
    write(col);
    write(row);
    writeStr("\x80\xC0\x94\xD4");
}

void DigoleSerialDisp::setTextPosAbs(unsigned int x, unsigned int y) {
    writeStr("ETP");
    write2B(x);
    write2B(y);
}

//----Touch screen functions-----//

void DigoleSerialDisp::calibrateTouchScreen(void) {
    writeStr("TUCHC");
}

void DigoleSerialDisp::readTouchScreen() {
    cleanBuffer();
    writeStr("RPNXYW");
}
void DigoleSerialDisp::readTouchScreen(int *x,int *y) {
    cleanBuffer();
    readTouchScreen();
    *x=readInt();
    *y=readInt();
}

void DigoleSerialDisp::readClick() //read a click on touch screen
{
    cleanBuffer();
    writeStr("RPNXYC");
}
void DigoleSerialDisp::readClick(int *x,int *y) //read a click on touch screen
{
    cleanBuffer();
    readClick();
    *x=readInt();
    *y=readInt();   
}
int DigoleSerialDisp::readBattery(void) {
    int c;
    cleanBuffer();
    writeStr("RDBAT");
//    delay(50);
    c = read1();
    c <<= 8;
    c |= read1();
    return c;
}

int DigoleSerialDisp::readAux(void) {
    int c;
    cleanBuffer();
    writeStr("RDAUX");
//    delay(50);
    c = read1();
    c <<= 8;
    c += read1();
    return c;
}

int DigoleSerialDisp::readTemperature(void) {
    int c;
    cleanBuffer();
    writeStr("RDTMP");
    delay(50);
    c = read1();
    c <<= 8;
    c |= read1();
//    c=readInt();
    return c;
}
//-----Flash memory functions----//

void DigoleSerialDisp::flashErase(unsigned long int addr, unsigned long int length) {
    writeStr("FLMER");
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(length >> 16);
    write(length >> 8);
    write(length);
}

void DigoleSerialDisp::flashReadStart(unsigned long int addr, unsigned long int len) {
    writeStr("FLMRD");
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(len >> 16);
    write(len >> 8);
    write(len);
}

void DigoleSerialDisp::setFlashFont(unsigned long int addr) {
    writeStr("SFF");
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
}

void DigoleSerialDisp::runCommandSet(unsigned long int addr) {
    writeStr("FLMCS");
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
}
//V3.3 functions

void DigoleSerialDisp::writeE2prom(unsigned int addr, unsigned int len, unsigned char *data) {
    unsigned char c;
    writeStr("WREP");
    write(addr >> 8);
    write(addr);
    write(len >> 8);
    write(len);
    for (unsigned int i = 0; i < len; i++) {
        c = data[i];
        write(c);
    }
}

void DigoleSerialDisp::readE2prom(unsigned int addr, unsigned int len) {
    writeStr("RDEP");
    write(addr >> 8);
    write(addr);
    write(len >> 8);
    write(len);
}

void DigoleSerialDisp::backLightOn(void) {
    writeStr("BL");
    write((uint8_t) 99);
}

void DigoleSerialDisp::backLightOff(void) {
    writeStr("BL");
    write((uint8_t) 0);
}

void DigoleSerialDisp::screenOnOff(uint8_t a) //turn screen on/off
{
    writeStr("SOO");
    write((uint8_t) a);
}

void DigoleSerialDisp::cpuOff(void) //put MCU in sleep, it will wake up when new data received
{
    writeStr("DNMCU");
}

void DigoleSerialDisp::moduleOff(void) //put whole module in sleep: Back light off, screen in sleep, MCU in sleep
{
    writeStr("DNALL");
}

void DigoleSerialDisp::backLightBrightness(uint8_t a) //set backlight brightness,0~100
{
    writeStr("BL");
    write((uint8_t) a);
}