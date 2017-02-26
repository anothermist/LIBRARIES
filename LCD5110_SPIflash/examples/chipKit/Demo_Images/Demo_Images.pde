// Demo_Images 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the loadBitmap()-function.
//
// This program requires LCD5110_SPIflash, LCD5110_Graph, 
// as well as SPIflash.
// Please note that this example does not work with 
// LCD5110_Basic.
//
// This demo expects the "TestImages_Mono" (MONO.SFD)
// dataset to be loaded into the flash chip.
// (Demo datasets are supplied with the SPIflash library)
//

#include <SPIflash.h>
#include <LCD5110_Graph.h>
#include <LCD5110_SPIflash.h>

// Remember to set the appropriate pins for your setup.
// See the LCD5110_Graph manual for further information.
// myGLCD(<SCK>, <MOSI>, <DC>, <RST>, <CS>);
LCD5110 myGLCD(34,9,6,5,3);

// myFlash(<SS>); (if using the hardware SPI pins. See the SPIflash manual for other configurations.)
SPIflash      myFlash(8);

LCD5110_SPIflash myFiles(&myGLCD, &myFlash);

// Declare which fonts we will be using
extern uint8_t TinyFont[];

uint8_t  fileid, xPos, yPos;
uint16_t filetype;

void setup()
{
  randomSeed(analogRead(0));
  
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(TinyFont);
  myFlash.begin();

  if (myFlash.ID_device==0)
  {
    myGLCD.print("Unknown flash device!", 0, 0);
    myGLCD.update();
    while (true) {};
  }    
}

void loop()
{
  fileid = 0;
  filetype = 0;

  while (filetype != ERR_FILE_DOES_NOT_EXIST)
  {
    filetype = myFlash.getFileType(fileid);
    if (filetype == 6)
    {
      myGLCD.clrScr();
      xPos = random(84-myFlash.getImageXSize(fileid));
      yPos = random(48-myFlash.getImageYSize(fileid));
      myFiles.loadBitmap(fileid, xPos, yPos);
      myGLCD.update();
      delay(500);
      fileid++;
    }
  }
}


