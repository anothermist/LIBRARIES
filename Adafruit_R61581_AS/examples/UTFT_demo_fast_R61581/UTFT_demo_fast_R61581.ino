// Demo based on:
// UTFT_Demo by Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//


#include "Adafruit_GFX_AS.h"
#include "Adafruit_R61581_AS.h"

#define R61581_GREY 0x7BEF

// Comment out next line to stop delays between tests
//#define DELAY

Adafruit_R61581_AS myGLCD = Adafruit_R61581_AS();       // Invoke custom library

unsigned long runTime = 0;
void setup()
{
  randomSeed(analogRead(A0));
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  delay(10);
  digitalWrite(7, HIGH);
// Setup the LCD
  myGLCD.begin(R61581B);
  myGLCD.setRotation(3);
}

void loop()
{
  randomSeed(millis());
  //randomSeed(1234); // This ensure test is repeatable with exact same draws each loop
  int buf[398];
  int x, x2;
  int y, y2;
  int r;
  runTime = millis();

// Clear the screen and draw the frame
  myGLCD.fillScreen(R61581_BLACK);


  myGLCD.fillRect(0, 0, 399, 14,R61581_RED);

  myGLCD.fillRect(0, 226, 399, 14,R61581_GREY);

  myGLCD.setTextColor(R61581_BLACK,R61581_RED);
  myGLCD.drawCentreString("* R61581_AS Color TFT Display Library *", 160, 3, 1);
  myGLCD.setTextColor(R61581_YELLOW,R61581_GREY);
  myGLCD.drawCentreString("Adapted by Bodmer 13/2/15", 160, 229,1);

  myGLCD.drawRect(0, 14, 399, 211, R61581_BLUE);

// Draw crosshairs
  myGLCD.drawLine(199, 15, 199, 224,R61581_BLUE);
  myGLCD.drawLine(1, 119, 398, 119,R61581_BLUE);
  for (int i=9; i<310; i+=10)
    myGLCD.drawLine(i, 117, i, 121,R61581_BLUE);
  for (int i=19; i<220; i+=10)
    myGLCD.drawLine(197, i, 201, i,R61581_BLUE);

// Draw sin-, cos- and tan-lines  
  myGLCD.setTextColor(R61581_CYAN);
  myGLCD.drawString("Sin", 5, 15,2);
  for (int i=1; i<398; i++)
  {
    myGLCD.drawPixel(i,119+(sin(((i*0.9)*3.14)/180)*95),R61581_CYAN);
  }
  myGLCD.setTextColor(R61581_RED);
  myGLCD.drawString("Cos", 5, 30,2);
  for (int i=1; i<398; i++)
  {
    myGLCD.drawPixel(i,119+(cos(((i*0.9)*3.14)/180)*95),R61581_RED);
  }
  myGLCD.setTextColor(R61581_YELLOW);
  myGLCD.drawString("Tan", 5, 45,2);
  for (int i=1; i<398; i++)
  {
    myGLCD.drawPixel(i,119+(tan(((i*0.9)*3.14)/180)),R61581_YELLOW);
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

  myGLCD.drawLine(199, 15, 199, 224,R61581_BLUE);
  myGLCD.drawLine(1, 119, 398, 119,R61581_BLUE);
int col = 0;
// Draw a moving sinewave
  x=1;
  for (int i=1; i<(397*20); i++) 
  {
    x++;
    if (x==398)
      x=1;
    if (i>398)
    {
      if ((x==199)||(buf[x-1]==119))
        col = R61581_BLUE;
      else
      myGLCD.drawPixel(x,buf[x-1],R61581_BLACK);
    }
    y=119+(sin(((i)*3.14)/180)*(90-(i / 100)));
    myGLCD.drawPixel(x,y,R61581_BLUE);
    buf[x-1]=y;
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        col = R61581_MAGENTA;
        break;
      case 2:
        col = R61581_RED;
        break;
      case 3:
        col = R61581_GREEN;
        break;
      case 4:
        col = R61581_BLUE;
        break;
      case 5:
        col = R61581_YELLOW;
        break;
    }
    myGLCD.fillRect(110+(i*20), 30+(i*20), 60, 60,col);
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        col = R61581_MAGENTA;
        break;
      case 2:
        col = R61581_RED;
        break;
      case 3:
        col = R61581_GREEN;
        break;
      case 4:
        col = R61581_BLUE;
        break;
      case 5:
        col = R61581_YELLOW;
        break;
    }
    myGLCD.fillRoundRect(230-(i*20), 30+(i*20), 60,60, 3,col);
  }
  
#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        col = R61581_MAGENTA;
        break;
      case 2:
        col = R61581_RED;
        break;
      case 3:
        col = R61581_GREEN;
        break;
      case 4:
        col = R61581_BLUE;
        break;
      case 5:
        col = R61581_YELLOW;
        break;
    }
    myGLCD.fillCircle(110+(i*20),60+(i*20), 30,col);
  }
  
#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some lines in a pattern

  for (int i=15; i<224; i+=5)
  {
    myGLCD.drawLine(1, i, (i*1.77)-10, 223,R61581_RED);
  }

  for (int i=223; i>15; i-=5)
  {
    myGLCD.drawLine(397, i, (i*1.77)-11, 15,R61581_RED);
  }

  for (int i=223; i>15; i-=5)
  {
    myGLCD.drawLine(1, i, 411-(i*1.77), 15,R61581_CYAN);
  }

  for (int i=15; i<224; i+=5)
  {
    myGLCD.drawLine(397, i, 410-(i*1.77), 223,R61581_CYAN);
  }
  
#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    x=32+random(336);
    y=45+random(146);
    r=random(30);
    myGLCD.drawCircle(x, y, r,random(0xFFFF));
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    x=2+random(396);
    y=16+random(207);
    x2=2+random(396);
    y2=16+random(207);
    if (x2<x) {
      r=x;x=x2;x2=r;
    }
    if (y2<y) {
      r=y;y=y2;y2=r;
    }
    myGLCD.drawRect(x, y, x2-x, y2-y,random(0xFFFF));
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
    x=2+random(396);
    y=16+random(207);
    x2=2+random(396);
    y2=16+random(207);
    // We need to get the width and height and do some window checking
    if (x2<x) {
      r=x;x=x2;x2=r;
    }
    if (y2<y) {
      r=y;y=y2;y2=r;
    }
    // We need a minimum size of 6
    if((x2-x)<6) x2=x+6;
    if((y2-y)<6) y2=y+6;
    myGLCD.drawRoundRect(x, y, x2-x,y2-y, 3,random(0xFFFF));
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

 //randomSeed(1234);
 int colour = 0;
 for (int i=0; i<100; i++)
  {
    x=2+random(396);
    y=16+random(209);
    x2=2+random(396);
    y2=16+random(209);
    colour=random(0xFFFF);
    myGLCD.drawLine(x, y, x2, y2,colour);
  }

#ifdef DELAY
  delay(2000);
#endif

  myGLCD.fillRect(1,15,397,209,R61581_BLACK);

  // Draw 10,000 pixels but not using random numbers as that wastes time
  byte i = 100;
  while (i--) {
    byte j = 100;
    while (j--) myGLCD.drawPixel(i+110,j+70,i+j);
  }
  
#ifdef DELAY
  delay(2000);
#endif

  //myGLCD.fillScreen(R61581_BLUE);
  myGLCD.fillRect(0, 0, 399, 239,R61581_BLUE);
  myGLCD.fillRoundRect(80, 70, 239-80,169-70, 3,R61581_RED);
  
  myGLCD.setTextColor(R61581_WHITE,R61581_RED);
  myGLCD.drawCentreString("That's it!", 160, 93,2);
  myGLCD.drawCentreString("Restarting in a", 160, 119,2);
  myGLCD.drawCentreString("few seconds...", 160, 132,2);

  //runTime = millis()-runTime;
  myGLCD.setTextColor(R61581_GREEN,R61581_BLUE);
  myGLCD.drawCentreString("Runtime: (msecs)", 160, 180, 2);
  myGLCD.drawNumber(millis()-runTime, 140, 200, 2);
  delay (5000);
}


