/*
  UTFT_tinyFAT.h - Add-on library to integrate UTFT and tinyFAT
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

#ifndef UTFT_tinyFAT_h
#define UTFT_tinyFAT_h

#include <UTFT.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif
#include "tinyFAT.h"

class UTFT_tinyFAT
{
	public:
		UTFT_tinyFAT(UTFT *ptrUTFT);

		word loadBitmap(int x, int y, int sx, int sy, char *filename);
		word loadBitmap(int x, int y, int sx, int sy, String filename);

protected:
		UTFT *_UTFT;
};

#endif