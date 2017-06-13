/***************************************************
  This is an example sketch for the Adafruit 3.5" R61581 16 bit parallel display.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Original written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
  
  Adapted for R61581 display
 ****************************************************/

// Comment out next line to stop delays between tests
//#define DELAY

#include "Adafruit_GFX_AS.h"
#include "Adafruit_R61581_AS.h"

Adafruit_R61581_AS tft = Adafruit_R61581_AS();

unsigned long total = 0;
unsigned long tn = 0;
void setup() {
  Serial.begin(115200);
  while (!Serial);
 
  tft.begin(R61581B);

  tn = micros();
  tft.fillScreen(R61581_BLACK);
  //tft.drawPixel(10,10,R61581_WHITE);
  //tft.fillRect(10,10,40,40,R61581_WHITE);
  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  //total+=testFillScreen();
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Text                     "));
  Serial.println(testText());
  //total+=testText();
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Lines                    "));
  Serial.println(testLines(R61581_CYAN));
  //total+=testLines(R61581_CYAN);
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(R61581_RED, R61581_BLUE));
  //total+=testFastLines(R61581_RED, R61581_BLUE);
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(R61581_GREEN));
  //total+=testRects(R61581_GREEN);
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(R61581_YELLOW, R61581_MAGENTA));
  //total+=testFilledRects(R61581_YELLOW, R61581_MAGENTA);
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, R61581_MAGENTA));
  //total+= testFilledCircles(10, R61581_MAGENTA);
  
  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, R61581_WHITE));
  //total+=testCircles(10, R61581_WHITE);
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  //total+=testTriangles();
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  //total+= testFilledTriangles();
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  //total+=testRoundRects();
#ifdef DELAY
  delay(500);
#endif

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  //total+=testFilledRoundRects();
#ifdef DELAY
  delay(500);
#endif

  //Serial.print(F("Done! Total = ")); Serial.println(total);

  //Serial.println(millis()-tn);
}

void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(500);
  }
}


unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(R61581_BLACK);
  tft.fillScreen(R61581_RED);
  tft.fillScreen(R61581_GREEN);
  tft.fillScreen(R61581_BLUE);
  tft.fillScreen(R61581_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(R61581_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(R61581_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(R61581_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(R61581_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(R61581_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(R61581_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(R61581_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(R61581_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(R61581_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(R61581_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(R61581_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(R61581_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n-1; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(R61581_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(R61581_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(R61581_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 2,
                cy = tft.height() / 2 - 2;

  tft.fillScreen(R61581_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(R61581_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}
