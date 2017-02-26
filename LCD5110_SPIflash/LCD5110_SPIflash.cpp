/*
  LCD5110_SPIflash.cpp - Add-on library to integrate LCD5110_Graph and SPIflash
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

#include "LCD5110_SPIflash.h"

LCD5110_SPIflash::LCD5110_SPIflash(LCD5110 *ptrLCD5110, SPIflash *ptrSPIflash)
{
	_LCD5110	= ptrLCD5110;
	_SPIflash	= ptrSPIflash;
}

word LCD5110_SPIflash::loadBitmap(uint16_t fileid, int x, int y)
{
	uint32_t	res, imgX, imgY;
	uint16_t	bread;
	uint8_t		fhandle;
	int			cx, cy;
	int			bit;

	res = _SPIflash->getFileType(fileid);
	if (res != 6)
		return ERR_FILETYPE_INCORRECT;

	fhandle = _SPIflash->fileOpen(fileid);
	if (fhandle!=ERR_FILE_DOES_NOT_EXIST)
	{
		imgX = _SPIflash->getImageXSize(fileid);
		imgY = _SPIflash->getImageYSize(fileid);
		for (int cy=0; cy<imgY; cy++)
		{
			bit= cy % 8;
			if (bit == 0)
				bread = _SPIflash->fileRead(fhandle, imgbuf, imgX);
			for(int cx=0; cx<imgX; cx++)
			{
				if ((imgbuf[cx] & (1<<bit))>0)
					_LCD5110->setPixel(x+cx, y+cy);
				else
					_LCD5110->clrPixel(x+cx, y+cy);
			}
		}      
		_SPIflash->fileClose(fhandle);
		return ERR_NO_ERROR;
	}
}

word LCD5110_SPIflash::loadBitmap(uint16_t fileid, int x, int y, int ox, int oy, int sx, int sy)
{
	uint32_t	res, imgX, imgY, cpos;
	uint16_t	bread, bit;
	uint8_t		fhandle;
	uint16_t	cx, cy;

	res = _SPIflash->getFileType(fileid);
	if (res != 6)
		return ERR_FILETYPE_INCORRECT;

	fhandle = _SPIflash->fileOpen(fileid);
	if (fhandle!=ERR_FILE_DOES_NOT_EXIST)
	{
		imgX = _SPIflash->getImageXSize(fileid);
		imgY = _SPIflash->getImageYSize(fileid);

		_SPIflash->fileSeek(fhandle, 0);
		cpos = (((oy / 8)*imgX)+ox);
		_SPIflash->fileSeek(fhandle, cpos);
		bread = _SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));

		for (int cy=oy; cy<(oy+sy); cy++)
		{
			bit= cy % 8;
			if ((bit == 0) and (oy!=cy))
			{
				_SPIflash->fileSeek(fhandle, imgX-sx);
				bread = _SPIflash->fileRead(fhandle, imgbuf, sx);
			}
			for(int cx=0; cx<sx; cx++)
			{
				if ((imgbuf[cx] & (1<<bit))>0)
					_LCD5110->setPixel(x+cx, y+(cy-oy));
				else
					_LCD5110->clrPixel(x+cx, y+(cy-oy));
			}
		}      
		_SPIflash->fileClose(fhandle);
		return ERR_NO_ERROR;
	}
}
