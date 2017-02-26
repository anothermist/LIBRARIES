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

#ifndef UTFT_DLB_Buttons_h
#define UTFT_DLB_Buttons_h

#include "arduino.h"

#include <UTFT_DLB.h>

#include <UTFT_Buttons.h>

class buttonExt
{
    public:
        buttonExt* next;
        
        int id;
        word _color_text, _color_text_inactive, _color_background, _color_border, _color_hilite;
};

class UTFT_DLB_Buttons : public UTFT_Buttons
{
	public:
		UTFT_DLB_Buttons(UTFT_DLB *ptrUTFT, UTouch *ptrUTouch);
      virtual ~UTFT_DLB_Buttons();

      // over-ridden
		int addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags=0);
		void deleteButton(int buttonID);
      void deleteAllButtons();
		void drawButton(int buttonID);
 		void relabelButton(int buttonID, char *label, boolean redraw=false);

 		void setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back);

      // new   
		void setButtonColors(int buttonID, word atxt, word iatxt, word brd, word brdhi, word back);
      void clearButtonColors(int buttonID);   
      buttonExt* getButtonColors(int buttonID);
      bool testButton(int id);
};

#endif
