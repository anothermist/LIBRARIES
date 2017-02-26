// Demo_Portrait 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the loadBitmap()-function.
//
// This program requires UTFT_tinyFAT, UTFT v2.41 or higher, 
// as well as tinyFAT v3.0 or higher.
//
// The image files must be present in the root folder 
// of a FAT16 formatted SDcard in the module cardslot.
//
// Please note that this demo only supports the following
// display sizes:
//      176x220
//      240x320
//      240x400

#include <tinyFAT.h>
#include <UTFT.h>
#include <UTFT_tinyFAT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

UTFT         myGLCD(ITDB32S, 38, 39, 40, 41);   // Remember to change the model parameter to suit your display module!
UTFT_tinyFAT myFiles(&myGLCD);

// List of filenames for pictures to display. 
char* files320[]={"PIC101.RAW", "PIC102.RAW", "PIC103.RAW", "PIC104.RAW", "PIC105.RAW", "PIC106.RAW", "PIC107.RAW", "PIC108.RAW", "PIC109.RAW", "PIC110.RAW"}; // 240x320
char* files400[]={"PIC201.RAW", "PIC202.RAW", "PIC203.RAW", "PIC204.RAW", "PIC205.RAW", "PIC206.RAW", "PIC207.RAW", "PIC208.RAW", "PIC209.RAW", "PIC210.RAW"}; // 240x400
char* files220[]={"PIC501.RAW", "PIC502.RAW", "PIC503.RAW", "PIC504.RAW", "PIC505.RAW", "PIC506.RAW", "PIC507.RAW", "PIC508.RAW", "PIC509.RAW", "PIC510.RAW"}; // 176x220
char* files[10];

int picsize_x, picsize_y;
boolean display_rendertime=false;  // Set this to true if you want the rendertime to be displayed after a picture is loaded
boolean display_filename=true;  // Set this to false to disable showing of filename

word res;
long sm, em;

void setup()
{
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();
  file.initFAT();
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  picsize_x=myGLCD.getDisplayXSize();
  picsize_y=myGLCD.getDisplayYSize();
  switch (picsize_y)
  {
    case 220:
      for (int z=0; z<sizeof(files220)/sizeof(*files220);z++)
        files[z] = files220[z];
      break;
    case 320:
      for (int z=0; z<sizeof(files320)/sizeof(*files320);z++)
        files[z] = files320[z];
      break;
    case 400:
      for (int z=0; z<sizeof(files400)/sizeof(*files400);z++)
        files[z] = files400[z];
      break;
  }
}

void loop()
{
  
  for (int i=0; i<(sizeof(files)/sizeof(*files)); i++)
  {
    if (files[i]!="")
    {
      sm=millis();
      res=myFiles.loadBitmap(0, 0, picsize_x, picsize_y, files[i]);
      em=millis();
      if (res!=0)
      {
        if (res==0x10)
        {
          myGLCD.print("File not found...", 0, 0);
          myGLCD.print(files[i], 0, 14);
        }
        else
        {
          myGLCD.print("ERROR: ", 0, 0);
          myGLCD.printNumI(res, 56, 0);
        }
        delay(3000);
        myGLCD.clrScr();
      }
      else
      {
        if (display_rendertime==true)
        {
          myGLCD.print("Rendertime (secs):", 0, 0);
          myGLCD.printNumF(float((em-sm)/1000.0), 2, 160,0);
        }
        if (display_filename==true)
        {
           myGLCD.print(files[i], CENTER, myGLCD.getDisplayYSize()-12);
       }
        delay(3000);
      }
    }
  }
}

