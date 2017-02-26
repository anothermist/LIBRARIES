//***************************************************************
// UTFT_DLB_Buttons_Demo
//***************************************************************

#include <UTFT.h>

#include <UTFT_DLB.h>

#include <UTouch.h>

#include <UTFT_Buttons.h>
#include <UTFT_DLB_Buttons.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t DejaVuSans18[];
extern uint8_t DejaVuSans24[];

//*********************************************************************************
// Touchscreen control
//*********************************************************************************

//For Mega
#define LCD_RS   38         
#define LCD_WR   39     
#define LCD_CS   40       
#define LCD_REST 41

// Uncomment the next line for Arduino 2009/Uno
//UTFT myGLCD(ITDB32S,19,18,17,16);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for Arduino Mega
UTFT_DLB myGLCD(ITDB32S,38,39,40,41);   // Remember to change the model parameter to suit your display module!


//For Mega
#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3
#define IRQ      2 

// For Mega
UTouch myTouch(6,5,4,3,2);

// Button storage
UTFT_DLB_Buttons  myButtons(&myGLCD, &myTouch);

//*************************************************************************************************
// Setup
//*************************************************************************************************

#define ORIENTATION LANDSCAPE
//#define ORIENTATION PORTRAIT

void setup()
{
  randomSeed(analogRead(0));

  myGLCD.InitLCD(ORIENTATION);

  // Touchscreen Setup
  myTouch.InitTouch(ORIENTATION);
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
    
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(120, 120, 0);
    myGLCD.print("Testing Button Colors", 20, 120, 0);

    // default button colors
    myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLUE, VGA_WHITE, VGA_BLUE);    
    myButtons.setTextFont(DejaVuSans18);

    int startButton = myButtons.addButton(40, 160, 99, 39, "Start");
    myButtons.setButtonColors(startButton, VGA_BLACK, VGA_GRAY, 0x07E4, VGA_WHITE, 0x07E4); // individual button colors
    myButtons.drawButton(startButton);  

    int stopButton = myButtons.addButton(180, 160, 99, 39, "Stop");
    myButtons.setButtonColors(stopButton, VGA_BLACK, VGA_GRAY, 0xF800, VGA_WHITE, 0xF800);  // individual button colors
    myButtons.drawButton(stopButton);
    
    int pressed = myButtons.checkButtons();

    delay(3000);

    myButtons.setButtonColors(stopButton, VGA_WHITE, VGA_GRAY, 0x77, VGA_GRAY, 0x77);  // individual button colors
    myButtons.drawButton(stopButton);

    pressed = myButtons.checkButtons();
    
    if (pressed == stopButton)
    {
      // do something
    }
    
    myButtons.deleteAllButtons();

    delay(3000);
  }
}


