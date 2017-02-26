/*
 * ks0108 GLCDexample
 * (example sketch from original ks0108 library)
 *
 * Basic test code for the Arduino KS0108 GLCD library.
 * This code exercises a range of graphic functions supported
 * by the library and is an example of its use.
 * It also gives an indication of performance, showing the
 *  number of frames drawn per second.  
 */

#include <openGLCD.h>

Image_t icon;

gText textArea;              // a text area to be defined later in the sketch

unsigned long startMillis;
unsigned int loops = 0;
unsigned int iter = 0;

void setup()
{
  GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
  if(GLCD.Height >= 64)   
    icon = ArduinoIcon64x64;  // the 64 pixel high icon
  else
    icon = ArduinoIcon64x32;  // the 32 pixel high icon


  introScreen();              // show some intro stuff 
  GLCD.ClearScreen();  
  GLCD.SelectFont(System5x7); // default fixed width system font 
  GLCD.ClearScreen();
  // Create a text window on the right side of the display

  if(GLCD.Height <= 32)
  {
    textArea.DefineArea(GLCD.CenterX + 4, 1, GLCD.Right-2, GLCD.Bottom-4);
  }
  else
  {
    textArea.DefineArea(GLCD.CenterX + 4, 3, GLCD.Right-2, GLCD.Bottom-4);
  }
  textArea.SelectFont(System5x7); 

}

void introScreen()
{
  GLCD.ClearScreen();
  GLCD.SelectFont(System5x7);
  GLCD.DrawString(GLCD_GLCDLIB_NAMESTR, gTextfmt_center, 3);
  GLCD.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, GLCD.CharHeight(0) + 3);
  GLCD.DrawRoundRect(0+10,0, GLCD.Width-20, GLCD.CharHeight(0) *2 + 4, 5);  // rounded rectangle around text area   
  if(GLCD.Height < 64)   
  	countdown(5);
  showCharacters();
  countdown(5);
  GLCD.ClearScreen();
  GLCD.DrawBitmap(icon, GLCD.CenterX-32,0); //draw the bitmap at the given x,y position
  countdown(3);
}

void showCharacters()
{
  byte line = 3; // start on the fourth line 
  for(byte c = 32; c <=127; c++)
  {
     if( (c-32) % 20 == 0)
         GLCD.CursorTo(1,line++);  // CursorTo is used for fixed width system font
     GLCD.PutChar(c);    
  }   
}

void drawSpinner(byte pos, byte x, byte y, uint8_t color)
{   
  // this draws an object that appears to spin
  switch(pos % 8)
  {
    case 0 : GLCD.DrawLine( x, y-8, x, y+8, color);      break;
    case 1 : GLCD.DrawLine( x+3, y-7, x-3, y+7, color);  break;
    case 2 : GLCD.DrawLine( x+6, y-6, x-6, y+6, color);  break;
    case 3 : GLCD.DrawLine( x+7, y-3, x-7, y+3, color);  break;
    case 4 : GLCD.DrawLine( x+8, y, x-8, y, color);      break;
    case 5 : GLCD.DrawLine( x+7, y+3, x-7, y-3, color);  break;
    case 6 : GLCD.DrawLine( x+6, y+6, x-6, y-6, color);  break; 
    case 7 : GLCD.DrawLine( x+3, y+7, x-3, y-7, color);  break;
  } 
}

void countdown(int count)
{
  while(count--)  // do countdown  
  {
     GLCD.CursorTo(0,1);   // first column, second row (offset is from 0)
     GLCD.PutChar(count + '0');
     delay(1000);  
  }  
}

void  loop()
{  
  iter=0; 
  startMillis = millis();
  while(iter < 10) // do 10 iterations
  {
    GLCD.DrawRect(0, 0, GLCD.CenterX+1, GLCD.Height); // rectangle in left side of screen
    GLCD.DrawRoundRect(GLCD.CenterX + 2, 0, GLCD.CenterX - 3, GLCD.Height, 5);  // rounded rectangle around text area   
    for(int i=0; i < GLCD.Bottom; i += 4)
      GLCD.DrawLine(1,1, GLCD.CenterX-1, i);  // draw lines from upper left down right side of rectangle  
    GLCD.DrawCircle(GLCD.CenterX/2, GLCD.CenterY-1, min(GLCD.CenterX/2, GLCD.CenterY)-2);   // draw circle centered in the left side of screen  
    drawSpinner(loops++, GLCD.CenterX + GLCD.CenterX/2, GLCD.CenterY + GLCD.CenterY/2, PIXEL_OFF);  // clear previous spinner position
    drawSpinner(loops, GLCD.CenterX + GLCD.CenterX/2, GLCD.CenterY + GLCD.CenterY/2, PIXEL_ON);  // draw new spinner position
    GLCD.CursorToXY(GLCD.CenterX/2, GLCD.Bottom -15);          
    GLCD.print(iter);            // print current iteration at the current cursor position 
    iter++;
  } 
  // display iterations per second
  unsigned long duration = millis() - startMillis;
  int fps = 10000 / duration;
  int fps_fract = (10000 % duration) * 10 / (duration/10);
  if(GLCD.Height <= 32)
  {
    textArea.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, gTextfmt_row(0));
    textArea.CursorTo(0,1);
  }
  else  
  {
    textArea.DrawString(GLCD_GLCDLIB_NAMESTR, gTextfmt_center, gTextfmt_row(0));
    textArea.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, gTextfmt_row(1));
    textArea.CursorTo(0,3);
  }

  textArea.print("FPS=");               // print a text string
  textArea.print(fps);                  // print an integer value
  textArea.print(".");
  if(fps_fract < 10)
    textArea.print((int)0);             // have to manually print the leading 0 when necessary
  textArea.print(fps_fract);
}
