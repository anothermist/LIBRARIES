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


#ifndef UTFT_DLB_h
#define UTFT_DLB_h

/*#include "WProgram.h"*/
#include "arduino.h"

#include <UTFT.h>

class propFont
{
    public:
        byte charCode;
        int adjYOffset;
        int width;
        int height;
        int xOffset;
        int xDelta;
        byte* dataPtr;
};
    

class UTFT_DLB : public UTFT
{
	public:
		UTFT_DLB();
      UTFT_DLB(byte model, int RS, int WR,int CS, int RST, int SER=0);

		void print(char *st, int x, int y, int deg=0);
   
      int getStringWidth(char* str);
      int getFontHeight();
      byte getOrientation();

	protected:
      int printProportionalChar(byte c, int x, int y);
		int rotatePropChar(byte c, int x, int y, int offset, int deg);

   private:
      bool getCharPtr(byte c, propFont& font);

   
};

#endif