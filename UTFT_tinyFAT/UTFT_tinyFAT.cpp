/*
  UTFT_tinyFAT.cpp - Add-on library to integrate UTFT and tinyFAT
  Copyright (C)2010-2013 Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio. You can find the latest version of the library at
  http://www.henningkarlsen.com/electronics

  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.henningkarlsen.com/electronics/contact.php

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/

#include "UTFT_tinyFAT.h"

UTFT_tinyFAT::UTFT_tinyFAT(UTFT *ptrUTFT)
{
	_UTFT = ptrUTFT;
}

word UTFT_tinyFAT::loadBitmap(int x, int y, int sx, int sy, char *filename)
{
	int res;
	int cx, cy, cp;
	word temp, result;
	byte r,g,b;

	res=file.openFile(filename, FILEMODE_BINARY);
	if (res==NO_ERROR)
	{
		cbi(_UTFT->P_CS, _UTFT->B_CS);
		cx=0;
		cy=0;
		result=512;
		if (_UTFT->orient==PORTRAIT)
		{
			_UTFT->setXY(x, y, x+sx-1, y+sy-1);
		}
		while (result==512)
		{
			result=file.readBinary();
			switch(result)
			{
				case ERROR_WRONG_FILEMODE:
					return ERROR_WRONG_FILEMODE;
					break;
				case ERROR_NO_FILE_OPEN:
					return ERROR_NO_FILE_OPEN;
					break;
				default:
					if (_UTFT->orient==PORTRAIT)
					{
						for (int i=0; i<result; i+=2)
							_UTFT->LCD_Write_DATA(file.buffer[i],file.buffer[i+1]);
					}
					else
					{
						cp=0;
						while (cp<result)
						{
							if (((result-cp)/2)<(sx-cx))
							{
								_UTFT->setXY(x+cx, y+cy, x+cx+((result-cp)/2)-1, y+cy);
								for (int i=(result-cp)-2; i>=0; i-=2)
									_UTFT->LCD_Write_DATA(file.buffer[cp+i],file.buffer[cp+i+1]);
								cx+=((result-cp)/2);
								cp=result;
							}
							else
							{
								_UTFT->setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									_UTFT->LCD_Write_DATA(file.buffer[cp+(i*2)],file.buffer[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
					}
					break;
			}              
		}
		file.closeFile();
		_UTFT->setXY(0,0,_UTFT->getDisplayXSize()-1,_UTFT->getDisplayYSize()-1);
		sbi(_UTFT->P_CS, _UTFT->B_CS);
		return 0;
	}
	else
	{
		return res;
	}
}

word UTFT_tinyFAT::loadBitmap(int x, int y, int sx, int sy, String filename)
{
	char buf[filename.length()+1];

	filename.toCharArray(buf, filename.length()+1);
	loadBitmap(x, y, sx, sy, buf);
}
