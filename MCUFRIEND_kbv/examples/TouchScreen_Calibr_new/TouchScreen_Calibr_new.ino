// .kbv:  previous sketch wasted too much SRAM with const strings
// .kbv:  replace all UTFT print const strings with F(string)
// .kbv:  correct UTFTGLUE settextcursor() for String print

//  https://forum.arduino.cc/index.php?topic=473885.msg3245748#msg3245748
//  file attached 03 May 2017 21:15 BST


// TouchScreen_Calibr_kbv for MCUFRIEND UNO Display Shields
// adapted by David Prentice
// for Adafruit's <TouchScreen.h> Resistive Touch Screen Library
// from Henning Karlsen's original program.  Many Thanks.

// UTouch_Calibration (C)2013-2014 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program can be used to calibrate the touchscreen
// of the display modules.
//
// It is assumed that the display module is connected to an
// appropriate shield or that you know how to change the pin
// numbers in the setup.
//
// Instructions will be given on the display.

//#define TOUCH_ORIENTATION  LANDSCAPE
#define TOUCH_ORIENTATION  PORTRAIT
#define TITLE "TouchScreen.h Calibration"

#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>            //we are using UTFT display methods
UTFTGLUE myGLCD(0x9341, A2, A1, A3, A4, A0);

// MCUFRIEND UNO shield shares pins with the TFT.   Due does NOT work

//int XP = 6, XM = A2, YP = A1, YM = 7;  //most common configuration (dead Due)
int XP = 7, XM = A1, YP = A2, YM = 6;  //most common configuration (WHITE Due)

//#include "TouchScreen_kbv.h"         //Local Library
//TouchScreen_kbv myTouch(XP, YP, XM, YM, 300);
#include <TouchScreen.h>         //Global Library
TouchScreen myTouch(XP, YP, XM, YM, 300);
TSPoint tp;                      //Touchscreen_due branch uses Point

void readResistiveTouch(void)
{
    tp = myTouch.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
}

bool ISPRESSED(void)
{
    // .kbv this was too sensitive !!
    // now touch has to be stable for 50ms
    int count = 0;
    bool state, oldstate;
    while (count < 10) {
        readResistiveTouch();
        state = tp.z > 180;  // .kbv was >20 && < 1000
        if (state == oldstate) count++;
        else count = 0;
        oldstate = state;
        delay(5);
    }
    return oldstate;
}

void showpoint(void)
{
    Serial.print("\r\nx="); Serial.print(tp.x);
    Serial.print(" y="); Serial.print(tp.y);
    Serial.print(" z="); Serial.print(tp.z);
}

// ************************************
// DO NOT EDIT ANYTHING BELOW THIS LINE
// ************************************

// Declare which fonts we will be using
#if defined(SmallFont)         //.kbv new GLUE defines FreeSmallFont
#undef SmallFont               //.kbv lose it
#define SmallFont NULL         //.kbv use System Font
#else
extern uint8_t SmallFont[];    //.kbv
#endif

uint32_t cx, cy;
uint32_t rx[8], ry[8];
int32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center, swapxy;
uint32_t calx, caly, cals;
char buf[13];

void showpins(int A, int D, int value, const char *msg)
{
    char buf[40];
    sprintf(buf, "%s (A%d, D%d) = %d", msg, A - A0, D, value);
    Serial.println(buf);
}

boolean diagnose_pins()
{
    int i, j, value, Apins[2], Dpins[2], Values[2], found = 0;
    //    Serial.begin(9600);
    Serial.println("Making all control and bus pins INPUT_PULLUP");
    Serial.println("Typical 30k Analog pullup with corresponding pin");
    Serial.println("would read low when digital is written LOW");
    Serial.println("e.g. reads ~25 for 300R X direction");
    Serial.println("e.g. reads ~30 for 500R Y direction");
    Serial.println("");
    for (i = A0; i < A5; i++) pinMode(i, INPUT_PULLUP);
    for (i = 2; i < 10; i++) pinMode(i, INPUT_PULLUP);
    for (i = A0; i < A4; i++) {
        for (j = 5; j < 10; j++) {
            pinMode(j, OUTPUT);
            digitalWrite(j, LOW);
            value = analogRead(i);   // ignore first reading
            value = analogRead(i);
            if (value < 100) {
                showpins(i, j, value, "Testing :");
                if (found < 2) {
                    Apins[found] = i;
                    Dpins[found] = j;
                    Values[found] = value;
                    found++;
                }
            }
            pinMode(j, INPUT_PULLUP);
        }
    }
    if (found == 2) {
        Serial.println("Diagnosing as:-");
        int idx = Values[0] < Values[1];
        for (i = 0; i < 2; i++) {
            showpins(Apins[i], Dpins[i], Values[i],
                     (Values[i] < Values[!i]) ? "XM,XP: " : "YP,YM: ");
        }
        XM = Apins[!idx]; XP = Dpins[!idx]; YP = Apins[idx]; YM = Dpins[idx];
//        myTouch = TouchScreen_kbv(XP, YP, XM, YM, 300); //Local library
        myTouch = TouchScreen(XP, YP, XM, YM, 300);     //Global library
        return true;
    }
    Serial.println("BROKEN TOUCHSCREEN");
    return false;
}

void setup()
{
    Serial.begin(9600);
    Serial.println(TITLE);
    bool ret = true;
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) 
    ret = diagnose_pins();
#else
    Serial.println("Only AVR can diagnose Touch Pins");
    Serial.println("Other targets use default Pins");
    char buf[40];
    sprintf(buf, "Touch Pin Wiring XP=%d XM=A%d YP=A%d YM=%d",
            XP, XM - A0, YP - A0, YM);
    Serial.println(buf);
#endif
    digitalWrite(A0, HIGH);
    pinMode(A0, OUTPUT);
    myGLCD.InitLCD(TOUCH_ORIENTATION);
    myGLCD.clrScr();
    myGLCD.setFont(SmallFont);
    dispx = myGLCD.getDisplayXSize();
    dispy = myGLCD.getDisplayYSize();
    text_y_center = (dispy / 2) - 6;
    if (ret == false) {
        myGLCD.print(F("BROKEN TOUCHSCREEN"), CENTER, dispy / 2);
        while (1);
    }
}

void drawCrossHair(int x, int y)
{
    myGLCD.drawRect(x - 10, y - 10, x + 10, y + 10);
    myGLCD.drawLine(x - 5, y, x + 5, y);
    myGLCD.drawLine(x, y - 5, x, y + 5);
}

void readCoordinates()
{
    int iter = 5000;
    int failcount = 0;
    int cnt = 0;
    uint32_t tx = 0;
    uint32_t ty = 0;
    boolean OK = false;

    while (OK == false)
    {
        myGLCD.setColor(255, 255, 255);
        myGLCD.print(F("*  PRESS  *"), CENTER, text_y_center);
        while (ISPRESSED() == false) {}
        myGLCD.print(F("*  HOLD!  *"), CENTER, text_y_center);
        cnt = 0;
        iter = 400;
        do
        {
            readResistiveTouch();
            // showpoint(tp);
            if (tp.z > 20 && tp.z < 1000)
            {
                tx += tp.x;
                ty += tp.y;
                cnt++;
            }
            else
                failcount++;
        } while ((cnt < iter) && (failcount < 10000));
        if (cnt >= iter)
        {
            OK = true;
        }
        else
        {
            tx = 0;
            ty = 0;
            cnt = 0;
        }
        if (failcount >= 10000)
            fail();
    }

    cx = tx / iter;
    cy = ty / iter;
}

void calibrate(int x, int y, int i)
{
    myGLCD.setColor(255, 255, 255);
    drawCrossHair(x, y);
    myGLCD.setBackColor(255, 0, 0);
    readCoordinates();
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(F("* RELEASE *"), CENTER, text_y_center);
    myGLCD.setColor(80, 80, 80);
    drawCrossHair(x, y);
    rx[i] = cx;
    ry[i] = cy;
    Serial.print("\r\ncx="); Serial.print(cx);
    Serial.print(" cy="); Serial.print(cy);
    while (ISPRESSED() == true) {}
}

void waitForTouch()
{
    while (ISPRESSED() == true) {}
    while (ISPRESSED() == false) {}
    while (ISPRESSED() == true) {}
}

void toHex(uint32_t num)
{
    buf[0] = '0';
    buf[1] = 'x';
    buf[10] = 'U';
    buf[11] = 'L';
    buf[12] = 0;
    for (int zz = 9; zz > 1; zz--)
    {
        if ((num & 0xF) > 9)
            buf[zz] = (num & 0xF) + 55;
        else
            buf[zz] = (num & 0xF) + 48;
        num = num >> 4;
    }
}

void startup()
{
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(0, 0, dispx - 1, 13);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.drawLine(0, 14, dispx - 1, 14);
    myGLCD.print(TITLE, CENTER, 1);
    myGLCD.setBackColor(0, 0, 0);

    myGLCD.print(F("#define NUMSAMPLES 3 in Library"), LEFT, 18);
    myGLCD.print(F("Use a stylus or something"), LEFT, 30);
    myGLCD.print(F("similar to touch as close"), LEFT, 42);
    myGLCD.print(F("to the center of the"), LEFT, 54);
    myGLCD.print(F("highlighted crosshair as"), LEFT, 66);
    myGLCD.print(F("possible. Keep as still as"), LEFT, 78);
    myGLCD.print(F("possible and keep holding"), LEFT, 90);
    myGLCD.print(F("until the highlight is"), LEFT, 102);
    myGLCD.print(F("removed. Repeat for all"), LEFT, 114);
    myGLCD.print(F("crosshairs in sequence."), LEFT, 126);
    myGLCD.print(F("Touch screen to continue"), CENTER, 162);

    waitForTouch();
}

void showNumI(char *msg, uint32_t val, int x, int y)
{
    myGLCD.print(msg, x, y);
    myGLCD.printNumI(val, x + 50, y);
}

void done()
{
    uint16_t TS_LEFT, TS_RT, TS_TOP, TS_BOT, TS_WID, TS_HT, TS_SWAP;
    int16_t tmp;
    char buf[60];
    myGLCD.clrScr();
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(0, 0, dispx - 1, 13);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.drawLine(0, 14, dispx - 1, 14);
    myGLCD.print(TITLE, CENTER, 1);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.print(F("To use the new calibration"), LEFT, 30);
    myGLCD.print(F("settings you must map the values"), LEFT, 42);
    myGLCD.print(F("from Point p = ts.getPoint() e.g. "), LEFT, 54);
    myGLCD.print(F("x = map(p.x, LEFT, RT, 0, tft.width());"), LEFT, 66);
    myGLCD.print(F("y = map(p.y, TOP, BOT, 0, tft.height());"), LEFT, 78);
    myGLCD.print(F("swap p.x and p.y if diff ORIENTATION"), LEFT, 90);

    //.kbv show human values
    TS_LEFT = (calx >> 14) & 0x3FFF;
    TS_RT   = (calx >>  0) & 0x3FFF;
    TS_TOP  = (caly >> 14) & 0x3FFF;
    TS_BOT  = (caly >>  0) & 0x3FFF;
    TS_WID  = ((cals >> 12) & 0x0FFF) + 1;
    TS_HT   = ((cals >>  0) & 0x0FFF) + 1;
    TS_SWAP = (cals >> 31);
    int y = 120;
    Serial.println("");
    sprintf(buf, "Sketch is %s      %d x %d",
            TOUCH_ORIENTATION ? "LANDSCAPE" : "PORTRAIT", TS_WID, TS_HT);
    myGLCD.print(buf, 0, y);
    Serial.println(buf);
    if (TOUCH_ORIENTATION == LANDSCAPE) { //always show PORTRAIT first
        tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;
        tmp = TS_WID, TS_WID = TS_HT, TS_HT = tmp;
    }
    sprintf(buf, "PORTRAIT CALIBRATION     %d x %d", TS_WID, TS_HT);
    myGLCD.print(buf, 0, y += 24);
    Serial.println(buf);
    sprintf(buf, "x = map(p.x, LEFT=%d, RT=%d, 0, %d)", TS_LEFT, TS_RT, TS_WID);
    myGLCD.print(buf, 0, y += 12);
    Serial.println(buf);
    sprintf(buf, "y = map(p.y, TOP=%d, BOT=%d, 0, %d)", TS_TOP, TS_BOT, TS_HT);
    myGLCD.print(buf, 0, y += 12);
    Serial.println(buf);
    sprintf(buf, "Touch Pin Wiring XP=%d XM=A%d YP=A%d YM=%d",
            XP, XM - A0, YP - A0, YM);
    myGLCD.print(buf, 0, y += 24);
    Serial.println(buf);

    tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;
    tmp = TS_WID, TS_WID = TS_HT, TS_HT = tmp;

    sprintf(buf, "LANDSCAPE CALIBRATION     %d x %d", TS_WID, TS_HT);
    myGLCD.print(buf, 0, y += 24);
    Serial.println(buf);
    sprintf(buf, "x = map(p.y, LEFT=%d, RT=%d, 0, %d)", TS_LEFT, TS_RT, TS_WID);
    myGLCD.print(buf, 0, y += 12);
    Serial.println(buf);
    sprintf(buf, "y = map(p.x, TOP=%d, BOT=%d, 0, %d)", TS_TOP, TS_BOT, TS_HT);
    myGLCD.print(buf, 0, y += 12);
    Serial.println(buf);
}

void fail()
{
    myGLCD.clrScr();
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(0, 0, dispx - 1, 13);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.drawLine(0, 14, dispx - 1, 14);
    myGLCD.print(F("Touch Calibration FAILED"), CENTER, 1);
    myGLCD.setBackColor(0, 0, 0);

    myGLCD.print(F("Unable to read the position"), LEFT, 30);
    myGLCD.print(F("of the press. This is a"), LEFT, 42);
    myGLCD.print(F("hardware issue and can"), LEFT, 54);
    myGLCD.print(F("not be corrected in"), LEFT, 66);
    myGLCD.print(F("software."), LEFT, 78);
    myGLCD.print(F("check XP, XM pins with a multimeter"), LEFT, 102);
    myGLCD.print(F("check YP, YM pins with a multimeter"), LEFT, 114);
    myGLCD.print(F("should be about 300 ohms"), LEFT, 126);

    while (true) {};
}

void loop()
{
    startup();

    delay(100);
    myGLCD.clrScr();
    myGLCD.setColor(80, 80, 80);
    drawCrossHair(dispx - 11, 10);
    drawCrossHair(dispx / 2, 10);
    drawCrossHair(10, 10);
    drawCrossHair(dispx - 11, dispy / 2);
    drawCrossHair(10, dispy / 2);
    drawCrossHair(dispx - 11, dispy - 11);
    drawCrossHair(dispx / 2, dispy - 11);
    drawCrossHair(10, dispy - 11);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.print(F("***********"), CENTER, text_y_center - 12);
    myGLCD.print(F("***********"), CENTER, text_y_center + 12);

    calibrate(10, 10, 0);
    calibrate(10, dispy / 2, 1);
    calibrate(10, dispy - 11, 2);
    calibrate(dispx / 2, 10, 3);
    calibrate(dispx / 2, dispy - 11, 4);
    calibrate(dispx - 11, 10, 5);
    calibrate(dispx - 11, dispy / 2, 6);
    calibrate(dispx - 11, dispy - 11, 7);

    cals = (long(dispx - 1) << 12) + (dispy - 1);
    if (TOUCH_ORIENTATION == PORTRAIT) swapxy = rx[2] - rx[0];
    else swapxy = ry[2] - ry[0];
    swapxy = (swapxy < -500 || swapxy > 500);
    if ((TOUCH_ORIENTATION == PORTRAIT) ^ (swapxy != 0)) {
        clx = (rx[0] + rx[1] + rx[2]) / 3;
        crx = (rx[5] + rx[6] + rx[7]) / 3;
        cty = (ry[0] + ry[3] + ry[5]) / 3;
        cby = (ry[2] + ry[4] + ry[7]) / 3;
    } else {
        clx = (ry[0] + ry[1] + ry[2]) / 3;
        crx = (ry[5] + ry[6] + ry[7]) / 3;
        cty = (rx[0] + rx[3] + rx[5]) / 3;
        cby = (rx[2] + rx[4] + rx[7]) / 3;
    }
    px = float(crx - clx) / (dispx - 20);
    py = float(cby - cty) / (dispy - 20);
    //  px = 0;
    clx -= px * 10;
    crx += px * 10;
    cty -= py * 10;
    cby += py * 10;

    calx = (long(clx) << 14) + long(crx);
    caly = (long(cty) << 14) + long(cby);
    if (swapxy)
        cals |= (1L << 31);

    done();
    while (true) {}
}


