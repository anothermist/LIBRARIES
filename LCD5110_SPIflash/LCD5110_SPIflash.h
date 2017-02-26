/*
  LCD5110_SPIflash.h - Add-on library to integrate LCD5110_Graph and SPIflash
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library is an add-on to LCD5110_Graph and will not work on its own. This 
  add-on library also requires the SPIflash library.
  Note that this library does not work in combination with the LCD5110_Basic library.

  This library adds a simple way to load images from SPI flash chip. The images 
  must be contained within the SPIflash file system. Images can be added to the 
  flash chips using the FlashUploader tool supplied with the SPIflash library.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef LCD5110_SPIflash_h
#define LCD5110_SPIflash_h

#include "LCD5110_Graph.h"
#include "SPIflash.h"

class LCD5110_SPIflash
{
	public:
		LCD5110_SPIflash(LCD5110 *ptrLCD5110, SPIflash *ptrSPIflash);

		word loadBitmap(uint16_t fileid, int x, int y);
		word loadBitmap(uint16_t fileid, int x, int y, int ox, int oy, int sx, int sy);

protected:
		LCD5110		*_LCD5110;
		SPIflash	*_SPIflash;

		char			imgbuf[84];  // Optimal buffer size: 84 bytes
};

#endif