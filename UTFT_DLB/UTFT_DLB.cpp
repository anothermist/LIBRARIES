/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

//#include "WProgram.h"
#include "arduino.h"
#include "UTFT_DLB.h"
#include <avr/pgmspace.h>
#include <pins_arduino.h>


// Extend UTFT to allow proportional font handling (or other ttf converted fixed-fonts)
UTFT_DLB::UTFT_DLB()
{
}

UTFT_DLB::UTFT_DLB(byte model, int RS, int WR,int CS, int RST, int SER) :
                    UTFT(model, RS, WR, CS, RST, SER)
{     
}

//*****************************************************************************************************************
// DLB
//*****************************************************************************************************************

// draw a proportional (or other ttf converted) font character on an angle
int UTFT_DLB::rotatePropChar(byte c, int x, int y, int offset, int deg)
{
   propFont fontChar;
    
   if (!getCharPtr(c, fontChar))
   {
       return 0;
   }

   byte i,j,ch;
   word temp; 
   byte *tempPtr = fontChar.dataPtr;
	double radian = deg * 0.0175;  
   
   // fill background
   // VGA_TRANSPARENT?
   word fcolor = getColor();

	cbi(P_CS, B_CS);
    
   if (fontChar.width != 0)
   {
      byte mask = 0x80;
      float cos_radian = cos(radian);
      float sin_radian = sin(radian);
      for (int j=0; j < fontChar.height; j++)
      {
         //ch=pgm_read_byte(tempPtr++);
         for (int i=0; i < fontChar.width; i++)
         {
            if (((i + (j*fontChar.width)) % 8) == 0)
            {
                mask = 0x80;
                ch = pgm_read_byte(tempPtr++);
            }
            
            int newX = x + ((offset + i) * cos_radian - (j+fontChar.adjYOffset)*sin_radian);
            int newY = y + ((j+fontChar.adjYOffset) * cos_radian + (offset + i) * sin_radian);
            if ((ch & mask) !=0)
            {
                setXY(newX,newY,newX,newY);
                setPixel(fcolor);
            } 
            else
            {
                if (!_transparent)
                {
                    setXY(newX,newY,newX,newY);
                    setPixel(getBackColor());
                }                
            }
            mask >>= 1;
         }
      }
   }

	sbi(P_CS, B_CS);
	clrXY();
      
   return fontChar.xDelta;
}

// override UTFT::print to handle proportional and fixed-width fonts
void UTFT_DLB::print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
       if (x==RIGHT)
          x=240-(stl*cfont.x_size);
       if (x==CENTER)
          x=(240-(stl*cfont.x_size))/2;
	}
	else
	{
       if (x==RIGHT)
          x=(disp_y_size+1)-(stl*cfont.x_size);
       if (x==CENTER)
          x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

   int offset = 0;
	for (i=0; i < stl; i++)
   {
		if (deg==0)
      {
        // DLB Added this stuff...
        if (cfont.x_size == 0)
        {
            x += printProportionalChar(*st++, x, y)+1;
        }
        else
        {          
            printChar(*st++, x, y);
            x += cfont.x_size;
        }
      }
		else
      {
        // DLB Added this stuff...
        if (cfont.x_size == 0)
        {
            offset += rotatePropChar(*st++, x, y, offset, deg);
        }
        else
        {
            rotateChar(*st++, x, y, i, deg);
        }
      }
  }
}

// private method to return the Glyph data for an individual character in the ttf font
bool UTFT_DLB::getCharPtr(byte c, propFont& fontChar)
{
    byte* tempPtr = cfont.font + 4; // point at data
    
    do
    {
        fontChar.charCode = pgm_read_byte(tempPtr++);
        fontChar.adjYOffset = pgm_read_byte(tempPtr++);
        fontChar.width = pgm_read_byte(tempPtr++);
        fontChar.height = pgm_read_byte(tempPtr++);
        fontChar.xOffset = pgm_read_byte(tempPtr++);
        fontChar.xOffset = fontChar.xOffset < 0x80 ? fontChar.xOffset : (0x100 - fontChar.xOffset);
        fontChar.xDelta = pgm_read_byte(tempPtr++);
        if (c != fontChar.charCode && fontChar.charCode != 0xFF)
        {
            if (fontChar.width != 0)
            {
                // packed bits
                tempPtr += (((fontChar.width * fontChar.height)-1) / 8) + 1;
            }
        }
    } while (c != fontChar.charCode && fontChar.charCode != 0xFF);
    
    fontChar.dataPtr = tempPtr;

    return (fontChar.charCode != 0xFF);
}


// print a ttf based character
int UTFT_DLB::printProportionalChar(byte c, int x, int y)
{
	 byte i,j,ch;
	 word temp; 
    byte *tempPtr;

    propFont fontChar;    
    if (!getCharPtr(c, fontChar))
    {
        return 0;
    }
        
    // fill background
    // VGA_TRANSPARENT?
    word fcolor = getColor();
    if (!_transparent)
    {
        int fontHeight = getFontHeight();
        setColor(getBackColor());
        fillRect(x, y, x + fontChar.xDelta+1, y + fontHeight);
        setColor(fcolor);
    }
    
    tempPtr = fontChar.dataPtr;
    
    // draw Glyph
   	*P_CS &= ~B_CS;
      
      if (fontChar.width != 0)
      {
          byte mask = 0x80;
          for (j=0; j < fontChar.height; j++)
          {
             //ch=pgm_read_byte(tempPtr++);
             for (i=0; i < fontChar.width; i++)
             {
                if (((i + (j*fontChar.width)) % 8) == 0)
                {
                    mask = 0x80;
                    ch = pgm_read_byte(tempPtr++);
                }
                
                if ((ch & mask) !=0)
                {
                    setXY(x+fontChar.xOffset+i, y+j+fontChar.adjYOffset,
                                x+fontChar.xOffset+i, y+j+fontChar.adjYOffset);
                    setPixel(fcolor);
                } 
                else
                {
                    //setPixel(bcolorr, bcolorg, bcolorb);
                }
                mask >>= 1;
             }
          }
      }
      
	 *P_CS |= B_CS;
      
    return fontChar.xDelta;
}

// returns the string width in pixels. Useful for positions strings on the screen.
int UTFT_DLB::getStringWidth(char* str)
{
    char* tempStrptr = str;
    
    // is it a fixed width font?
    if (cfont.x_size != 0)
    {
        return (strlen(str) * cfont.x_size);
    }
    else
    {
        // calculate the string width
        int strWidth = 0;
        while (*str != 0)
        {
            propFont fontChar;    
            bool found = getCharPtr(*str, fontChar);
            
            if (found && *str == fontChar.charCode)
            {
                strWidth += fontChar.xDelta + 1;
            }
            
            str++;            
        }

        return strWidth;
    }
}

int UTFT_DLB::getFontHeight()
{
    return (cfont.y_size);
}

byte UTFT_DLB::getOrientation()
{
    return orient;
}