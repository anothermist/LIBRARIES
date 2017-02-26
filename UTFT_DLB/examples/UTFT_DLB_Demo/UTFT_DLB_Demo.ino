//***************************************************************
// UTFT_DLB Demo
//***************************************************************

#include <UTFT.h>
#include <UTFT_DLB.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t DejaVuSans18[];
extern uint8_t DejaVuSans24[];

// Uncomment the next line for Arduino 2009/Uno
//UTFT_DLB myGLCD(ITDB32S,19,18,17,16);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for Arduino Mega
UTFT_DLB myGLCD(ITDB32S,38,39,40,41);   // Remember to change the model parameter to suit your display module!

//*************************************************************************************************
// Setup
//*************************************************************************************************

void setup()
{
  randomSeed(analogRead(0));

  myGLCD.InitLCD(LANDSCAPE);
}

//******************************************************************************************************************
// Main loop
//******************************************************************************************************************
void loop()
{
  while (1)
  {
    // Clear the screen and draw the frame
    myGLCD.clrScr();

    //myGLCD.setFont(BigFont);
    myGLCD.setFont(DejaVuSans18);

    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(0, 0, 0);
    
    myGLCD.setFont(BigFont);
    myGLCD.print("Fixed-Width Font", 20, 20, 0);

    myGLCD.setColor(255, 120, 0);
    myGLCD.setBackColor(50, 50, 50);
    
    myGLCD.setFont(DejaVuSans18);
    myGLCD.print("DejaVuSans 18", 20, 90, 0);
    myGLCD.setColor(255, 120, 120);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.print("DejaVuSans 18", 130, 150, -10);  // rotated text

    myGLCD.setFont(DejaVuSans24);
    myGLCD.setColor(255, 120, 255);
    myGLCD.setBackColor(50, 250, 50);
    myGLCD.print("DejaVuSans 24", 20, 180, 0);
    myGLCD.setColor(0, 120, 255);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.print("DejaVuSans 24", 150, 50, 20);    // rotated text

    delay(5000);
  }
}


