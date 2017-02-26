/*
  LCD5110_Basic.cpp - Arduino/chipKit library support for Nokia 5110 compatible LCDs
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio. You can find the latest version of the library at
  http://www.RinkyDinkElectronics.com/

  This library has been made to make it easy to use the basic functions of
  the Nokia 5110 LCD module on an Arduino or a chipKit.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#include "LCD5110_Basic.h"
#if defined(__AVR__)
	#include <avr/pgmspace.h>
	#include "hardware/avr/HW_AVR.h"
#elif defined(__PIC32MX__)
	#pragma message("Compiling for PIC32 Architecture...")
	#include "hardware/pic32/HW_PIC32.h"
#elif defined(__arm__)
	#pragma message("Compiling for ARM Architecture...")
	#include "hardware/arm/HW_ARM.h"
#endif

LCD5110::LCD5110(int SCK, int MOSI, int DC, int RST, int CS)
{ 
	P_SCK	= portOutputRegister(digitalPinToPort(SCK));
	B_SCK	= digitalPinToBitMask(SCK);
	P_MOSI	= portOutputRegister(digitalPinToPort(MOSI));
	B_MOSI	= digitalPinToBitMask(MOSI);
	P_DC	= portOutputRegister(digitalPinToPort(DC));
	B_DC	= digitalPinToBitMask(DC);
	P_RST	= portOutputRegister(digitalPinToPort(RST));
	B_RST	= digitalPinToBitMask(RST);
	P_CS	= portOutputRegister(digitalPinToPort(CS));
	B_CS	= digitalPinToBitMask(CS);
	pinMode(SCK,OUTPUT);
	pinMode(MOSI,OUTPUT);
	pinMode(DC,OUTPUT);
	pinMode(RST,OUTPUT);
	pinMode(CS,OUTPUT);
	SCK_Pin=SCK;
	RST_Pin=RST;
}

void LCD5110::_LCD_Write(unsigned char data, unsigned char mode)
{   
    cbi(P_CS, B_CS);

	if (mode==LCD_COMMAND)
		cbi(P_DC, B_DC);
	else
		sbi(P_DC, B_DC);

	for (unsigned char c=0; c<8; c++)
	{
		if (data & 0x80)
			sbi(P_MOSI, B_MOSI);
		else
			cbi(P_MOSI, B_MOSI);
		data = data<<1;
		pulseClock;
	}

	sbi(P_CS, B_CS);
}

void LCD5110::InitLCD(int contrast)
{
	if (contrast>0x7F)
		contrast=0x7F;
	if (contrast<0)
		contrast=0;

	resetLCD;

	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	_LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
	_LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
	_LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
	_LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
	_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
	_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
	for (int c=0; c<504; c++)
		_LCD_Write(0x00, LCD_DATA);
	_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);

	cfont.font=0;
	_sleep=false;
	_contrast=contrast;
}

void LCD5110::setContrast(int contrast)
{
	if (contrast>0x7F)
		contrast=0x7F;
	if (contrast<0)
		contrast=0;
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	_LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
	_LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
	_contrast=contrast;
}

void LCD5110::enableSleep()
{
	_sleep = true;
	_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
	_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
	for (int b=0; b<504; b++)
		_LCD_Write(0, LCD_DATA);
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN, LCD_COMMAND);
}

void LCD5110::disableSleep()
{
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	_LCD_Write(PCD8544_SETVOP | _contrast, LCD_COMMAND);
	_LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
	_LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
	_LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
	_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
	_sleep = false;
}

void LCD5110::clrScr()
{
	if (!_sleep)
	{
		_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
		_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
		for (int c=0; c<504; c++)
			_LCD_Write(0x00, LCD_DATA);
	}
}

void LCD5110::clrRow(int row, int start_x, int end_x)
{
	if (!_sleep)
	{
		_LCD_Write(PCD8544_SETYADDR | row, LCD_COMMAND);
		_LCD_Write(PCD8544_SETXADDR | start_x, LCD_COMMAND);
		for (int c=start_x; c<=end_x; c++)
			_LCD_Write(0x00, LCD_DATA);
		_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
		_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
	}
}

void LCD5110::invert(bool mode)
{
	if (!_sleep)
	{
		if (mode==true)
			_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED, LCD_COMMAND);
		else
			_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
	}
}

void LCD5110::invertText(bool mode)
{
	if (mode==true)
		cfont.inverted=1;
	else
		cfont.inverted=0;
}

void LCD5110::print(char *st, int x, int y)
{
	unsigned char ch;
	int stl, row, xp;

	if (!_sleep)
	{
		stl = strlen(st);
		if (x == RIGHT)
			x = 84-(stl*cfont.x_size);
		if (x == CENTER)
			x = (84-(stl*cfont.x_size))/2;
		if (x < 0)
			x = 0;

		row = y / 8;
		xp = x;

		for (int cnt=0; cnt<stl; cnt++)
			_print_char(*st++, x + (cnt*(cfont.x_size)), row);
	}
}

void LCD5110::print(String st, int x, int y)
{
	char buf[st.length()+1];

	if (!_sleep)
	{
		st.toCharArray(buf, st.length()+1);
		print(buf, x, y);
	}
}

void LCD5110::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (!_sleep)
	{
		if (num==0)
		{
			if (length!=0)
			{
				for (c=0; c<(length-1); c++)
					st[c]=filler;
				st[c]=48;
				st[c+1]=0;
			}
			else
			{
				st[0]=48;
				st[1]=0;
			}
		}
		else
		{
			if (num<0)
			{
				neg=true;
				num=-num;
			}
	  
			while (num>0)
			{
				buf[c]=48+(num % 10);
				c++;
				num=(num-(num % 10))/10;
			}
			buf[c]=0;
	  
			if (neg)
			{
				st[0]=45;
			}
	  
			if (length>(c+neg))
			{
				for (int i=0; i<(length-c-neg); i++)
				{
					st[i+neg]=filler;
					f++;
				}
			}

			for (int i=0; i<c; i++)
			{
				st[i+neg+f]=buf[c-i-1];
			}
			st[c+neg+f]=0;

		}

		print(st,x,y);
	}
}

void LCD5110::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (!_sleep)
	{
		if (num<0)
			neg = true;

		_convert_float(st, num, length, dec);

		if (divider != '.')
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]=='.')
					st[i]=divider;
		}

		if (filler != ' ')
		{
			if (neg)
			{
				st[0]='-';
				for (int i=1; i<sizeof(st); i++)
					if ((st[i]==' ') || (st[i]=='-'))
						st[i]=filler;
			}
			else
			{
				for (int i=0; i<sizeof(st); i++)
					if (st[i]==' ')
						st[i]=filler;
			}
		}

		print(st,x,y);
	}
}

void LCD5110::_print_char(unsigned char c, int x, int row)
{
	if (!_sleep)
	{
		if (((x+cfont.x_size)<=84) and (row+(cfont.y_size/8)<=6))
		{
			for (int rowcnt=0; rowcnt<(cfont.y_size/8); rowcnt++)
			{
				_LCD_Write(PCD8544_SETYADDR | (row + rowcnt), LCD_COMMAND);
				_LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);

				int font_idx = ((c - cfont.offset)*(cfont.x_size*(cfont.y_size/8)))+4;

				for(int cnt=0; cnt<cfont.x_size; cnt++)
				{
					if (cfont.inverted==0)
						_LCD_Write(fontbyte(font_idx+cnt+(rowcnt*cfont.x_size)), LCD_DATA);
					else
						_LCD_Write(~(fontbyte(font_idx+cnt+(rowcnt*cfont.x_size))), LCD_DATA);
				}
			}
			_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
			_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
		}
	}
}

void LCD5110::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
	cfont.inverted=0;
}

void LCD5110::drawBitmap(int x, int y, bitmapdatatype bitmap, int sx, int sy)
{
	int starty, rows;
    
	if (!_sleep)
	{
		starty = y / 8;

		if (sy%8==0)
			rows=sy/8;  
		else
			rows=(sy/8)+1;

		for (int cy=0; cy<rows; cy++)
		{
			_LCD_Write(PCD8544_SETYADDR | (starty+cy), LCD_COMMAND);
			_LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);
			for(int cx=0; cx<sx; cx++)
				_LCD_Write(bitmapbyte(cx+(cy*sx)), LCD_DATA);
		}      
		_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
		_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
	}
}
