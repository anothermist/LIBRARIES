/*
 * GLCD_BigDemo
 *
 * A rolling demo showing many capabilities of the openGLCD library
 * It combines a number of sketches included in the download.
 * This sketch uses around 26k of flash so will not run on 
 * an ATmega168
 *
 */

#include <openGLCD.h>

//#include <Streaming.h>  // uncomment to use Streaming library for streaming
#include <Time.h>  // download from: http://www.arduino.cc/playground/Code/Time

Image_t icon;

gText textArea;              // a text area to be defined later in the sketch
gText textAreaArray[3];      // an array of text areas  
gText countdownArea =  gText(textAreaFULL, SystemFont5x7); // text area for countdown digits

unsigned long startMillis;
unsigned int  loops = 0;
unsigned int  iter = 0;
         int  theDelay = 20; 

void setup()
{

  GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen

  if(GLCD.Height >= 64)   
    icon = ArduinoIcon64x64;  // the 64 pixel high icon
  else
    icon = ArduinoIcon64x32;  // the 32 pixel high icon
  introScreen();
  GLCD.ClearScreen();    
  GLCD.SelectFont(System5x7, BLACK); // font for the default text area
  clockBegin(); // start the clock
  clock(10); // show the clock to allow it to be set

}

void  loop()
{  
  GLCD.ClearScreen();  
  scribble(5000);  // run for 5 seconds

  GLCD.ClearScreen(); 

  GLCD.SelectFont(System5x7, BLACK);

  showCharacters("5x7 font:", System5x7);
  countdown(3);
  showCharacters("Arial14:", Arial14);
  countdown(3);

  clock(10); // show the clock given number of seconds
  
  life(20000); // 20 seconds

  GLCD.ClearScreen();   
  textAreaDemo();
 
  if((GLCD.Height >= 64) && (GLCD.Width >= 128))
  {
    rocket(30000);  // 30 seconds of rocket
  }
  
  GLCD.SelectFont(System5x7, BLACK);
  GLCD.ClearScreen(); 
  scrollingDemo();

  GLCD.ClearScreen();   

  if(GLCD.Width >= 192)
  {         
    GLCD.ClearScreen();   
    FPS(128, GLCD.Height, 10000); // 10 seconds of FPS
  }

  FPS(GLCD.Width, GLCD.Height, 10000); // 10 seconds of FPS
}



void introScreen(){  

  GLCD.SelectFont(Arial14); // you can also make your own fonts, see playground for details   
  GLCD.DrawString(GLCD_GLCDLIB_NAMESTR, gTextfmt_center, 3);
  GLCD.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, GLCD.CharHeight(0) + 2);
  GLCD.DrawRoundRect(0+10,0, GLCD.Right-20, GLCD.CharHeight(0) *2 + 1, 5);  // rounded rectangle around text area   
  countdown(5);  
  GLCD.ClearScreen(); 
  GLCD.DrawBitmap(icon, GLCD.CenterX-32,0); //draw the bitmap at the given x,y position
  countdown(3);
  GLCD.ClearScreen();
}


void showCharacters(const char * title, Font_t font) {
  // this displays the desired font
  GLCD.ClearScreen();  
  GLCD.CursorTo(0,0);
  GLCD.print(title);
  GLCD.DrawRoundRect(GLCD.CenterX + 2, 0, GLCD.CenterX -3, GLCD.Bottom, 5);  // rounded rectangle around text area 
  textArea.DefineArea(GLCD.CenterX + 5, 3, GLCD.Right-2, GLCD.Bottom-4, SCROLL_UP); 
  textArea.SelectFont(font, BLACK);
  textArea.CursorTo(0,0);
  for(char c = 32; c < 127; c++){
    textArea.print(c);  
    delay(20);
  }
}
void drawSpinner(byte pos, byte x, byte y, uint8_t color)
{
  // this draws an object that appears to spin
  switch(pos % 8) {
    case 0 : GLCD.DrawLine( x, y-8, x, y+8, color);        break;
    case 1 : GLCD.DrawLine( x+3, y-7, x-3, y+7, color);    break;
    case 2 : GLCD.DrawLine( x+6, y-6, x-6, y+6, color);    break;
    case 3 : GLCD.DrawLine( x+7, y-3, x-7, y+3, color);    break;
    case 4 : GLCD.DrawLine( x+8, y, x-8, y, color);        break;
    case 5 : GLCD.DrawLine( x+7, y+3, x-7, y-3, color);    break;
    case 6 : GLCD.DrawLine( x+6, y+6, x-6, y-6, color);    break; 
    case 7 : GLCD.DrawLine( x+3, y+7, x-3, y-7, color);    break;
  } 
}

void  textAreaDemo()
{ 
  showArea( textAreaFULL,       "Full"); 
  showArea( textAreaTOP,        "Top");
  showArea( textAreaBOTTOM,     "Bottom"); 
  showArea( textAreaRIGHT,      "Right");
  showArea( textAreaLEFT,       "Left"); 
  showArea( textAreaTOPLEFT,    "Top Left");
  showArea( textAreaTOPRIGHT,   "Top Right");
  showArea( textAreaBOTTOMLEFT, "Bot Left"); 
  showArea( textAreaBOTTOMRIGHT,"Bot Right");  
}

void  textAreaDemox()
{ 
  showArea( textAreaFULL,       "F"); 
  showArea( textAreaTOP,        "T");
  showArea( textAreaBOTTOM,     "B"); 
  showArea( textAreaRIGHT,      "R");
  showArea( textAreaLEFT,       "L"); 
  showArea( textAreaTOPLEFT,    "TL");
  showArea( textAreaTOPRIGHT,   "TR");
  showArea( textAreaBOTTOMLEFT, "BL"); 
  showArea( textAreaBOTTOMRIGHT,"BR");  
}

void showArea(predefinedArea area, const char *description)
{
  GLCD.ClearScreen(); 
  GLCD.DrawBitmap(icon, 0,  0); 
  GLCD.DrawBitmap(icon, 64, 0); 
  textArea.DefineArea(area);
  textArea.SelectFont(System5x7);
  textArea.SetFontColor(WHITE); 
  textArea.ClearArea(); 
  textArea.println(description);
  textArea.print("text area");
  delay(1000);
  textArea.SetFontColor(WHITE); 
  textArea.ClearArea();
  showLines(10);
  delay(1000);   
  textArea.ClearArea();
  textArea.SetFontColor(BLACK);   
  showLines(10);
  delay(1000); 
}

void showAscii()
{
  for( char ch = 64; ch < 127; ch++)
  {
    GLCD.print(ch);
    delay(theDelay);
  }   
}

void showLines(int lines)
{
  for(int i = 1; i <= lines; i++)
  { 
#ifdef ARDUINO_STREAMING
    textArea << " Line  " << i << endl;   // if using streaming support
#else
    textArea.print(" Line  "); // Print class
    textArea.println(i);
#endif
    delay(theDelay);  // brief pause between lines
  }
}


void countdown(int count)
{
char strbuf[2];
  strbuf[1] = 0;
  while(count--)
  {  // do countdown  

	
    strbuf[0] = count + '0';
    countdownArea.DrawString(strbuf, gTextfmt_left, gTextfmt_bottom);

    delay(1000);  
  }  
}

void scrollingDemo()
{
  GLCD.ClearScreen();  
  textAreaArray[0].DefineArea( textAreaTOPLEFT);  
  textAreaArray[0].SelectFont(System5x7, WHITE);
  textAreaArray[0].CursorTo(0,0);
  textAreaArray[1].DefineArea( textAreaTOPRIGHT, SCROLL_DOWN);  // reverse scroll
  textAreaArray[1].SelectFont(System5x7, BLACK);
  textAreaArray[1].CursorTo(0,0);
  textAreaArray[2].DefineArea(textAreaBOTTOM); 

  textAreaArray[2].SelectFont(Arial14, BLACK);
  textAreaArray[2].CursorTo(0,0);

  for(byte area = 0; area < 3; area++)
  {
    for( char c = 64; c < 127; c++)
      textAreaArray[area].print(c);
    delay(theDelay);    
  }
  for(char c = 32; c < 127; c++)
  {
    for(byte area = 0; area < 3; area++)
      textAreaArray[area].print(c);
    delay(theDelay);
  }  

  for(byte area = 0; area< 3; area++)
  {
    textAreaArray[area].ClearArea();
  }
  for(int x = 0; x < 15; x++)
  {
    for(byte area = 0; area < 3; area++)
    {
      textAreaArray[area].print("line ");
      textAreaArray[area].println(x);
      delay(80);
    }
  } 
  delay(1000);
}

/*
 * scribble drawing routine adapted from TellyMate scribble Video sketch
 * http://www.batsocks.co.uk/downloads/tms_scribble_001.pde
 */
void scribble( const unsigned int duration )
{
  const  float tick = 1/128.0;
  float g_head_pos = 0.0;
  
  for(unsigned long start = millis();  millis() - start < duration; )
  {
    g_head_pos += tick ;

    float head = g_head_pos ;
    float tail = head - (256 * tick) ;

    // set the pixels at the 'head' of the line...
    byte x = fn_x( head ) ;
    byte y = fn_y( head ) ;
    GLCD.SetDot( x , y , BLACK) ;

    // clear the pixel at the 'tail' of the line...
    x = fn_x( tail ) ;
    y = fn_y( tail ) ;  
     GLCD.SetDot( x , y , WHITE) ;
  }
}

byte fn_x( float tick )
{
  return (byte)(GLCD.Width/2 + (GLCD.Width/2-1) * sin( tick * 1.8 ) * cos( tick * 3.2 )) ;
}

byte fn_y( float tick )
{
  return (byte)(GLCD.Height/2 + (GLCD.Height/2 -1) * cos( tick * 1.2 ) * sin( tick * 3.1 )) ;
}

void FPS(const byte width, const byte height, const unsigned long msecs)

{

unsigned long stime = millis();

  // Create a text window on the righ side of the display
  if(height <= 32)
  {
    textArea.DefineArea(GLCD.CenterX + 4, 1, GLCD.Right-2, GLCD.Bottom-4);
  }
  else
  {
    textArea.DefineArea(width/2 + 4, 3, width-3, height-5);
  }
  textArea.SelectFont(System5x7); 

  GLCD.ClearScreen();   
  while(millis()  - stime < msecs)
    FPS(width, height);

}


void FPS( const byte width, const byte height)
{
  
  const byte CenterX = width/2;
  const byte CenterY = height/2;
//  const byte Right = width-1; // not used for now
  const byte Bottom = height-1;
 
  iter=0; 

  startMillis = millis();
  while(iter < 10)   // do 10 iterations
  {
    GLCD.DrawRect(0, 0, CenterX+1, height); // rectangle in left side of screen
    GLCD.DrawRoundRect(CenterX + 2, 0, CenterX - 3, height, 5);  // rounded rectangle around text area   
    for(int i=0; i < Bottom; i += 4)
      GLCD.DrawLine(1,1, CenterX-1, i);  // draw lines from upper left down right side of rectangle  
    GLCD.DrawCircle(CenterX/2, CenterY-1, min(CenterX/2, CenterY)-2);   // draw circle centered in the left side of screen
    drawSpinner(loops++, CenterX + CenterX/2, CenterY + CenterY/2, PIXEL_OFF);  // clear previous spinner position
    drawSpinner(loops, CenterX + CenterX/2, CenterY + CenterY/2, PIXEL_ON);  // draw new spinner position

    GLCD.CursorToXY(CenterX/2, Bottom -15);          
    GLCD.print(iter);            // print current iteration at the current cursor position 
    iter++;
  } 
  // display iterations per second
  unsigned long duration = millis() - startMillis;
  int fps = 10000 / duration;
  int fps_fract = (10000 % duration) * 10 / (duration/10);
  if(height <= 32)
  {
    textArea.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, gTextfmt_row(0));
#ifdef OLDWAY
    textArea.CursorTo(0,1);
#endif
  }
  else  
  {
    textArea.DrawString(GLCD_GLCDLIB_NAMESTR, gTextfmt_center, gTextfmt_row(0));
    textArea.DrawString(GLCD_GLCDLIB_VERSIONSTR, gTextfmt_center, gTextfmt_row(1));
#ifdef OLDWAY
    textArea.CursorTo(0,3);
#endif
  }

#ifdef OLDWAY
	
  textArea.print("FPS=");               // print a text string
  textArea.print(fps);                  // print an integer value
  textArea.print(".");
  if(fps_fract < 10)
    textArea.print((int)0);             // have to manually print the leading 0 when necessary
  textArea.print(fps_fract);
#else
  char buf[12];
  sprintf(buf, "FPS=%02d.%02d", fps, fps_fract);
  textArea.DrawString(buf, gTextfmt_center, gTextfmt_row(3));
#endif
}
