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


#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_DLB.h>
#include <UTFT_Buttons.h>
#include <UTFT_DLB_Buttons.h>

UTFT_DLB_Buttons::UTFT_DLB_Buttons(UTFT_DLB *ptrUTFT, UTouch *ptrUTouch) : 
                    UTFT_Buttons(ptrUTFT, ptrUTouch)
{
}

buttonExt* buttonsXtn = NULL;

void UTFT_DLB_Buttons::setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back)
{
    // This method is here to keep the compiler happy since we've reoverloaded
    // this method name with an extra individual button parameter below
    UTFT_Buttons::setButtonColors(atxt, iatxt, brd, brdhi, back);
}

// set button colors for an individual button
void UTFT_DLB_Buttons::setButtonColors(int buttonID, word atxt, word iatxt, word brd, word brdhi, word back)
{
    buttonExt* buttonPtr = buttonsXtn;
    if (buttonPtr == NULL)
    {
        buttonsXtn = new buttonExt();
        buttonPtr = buttonsXtn;
    }
    else
    {
        do
        {
            if (buttonPtr->id == buttonID)
            {
                break;
            }
            
            if (buttonPtr->next == NULL)
            {
                // We didn't find it. Append the extra button.
                buttonPtr->next = new buttonExt();
            }
            buttonPtr = buttonPtr->next;
        }
        while (buttonPtr->next != NULL);
    }
    
    buttonPtr->id = buttonID;
    buttonPtr->_color_text = atxt;
    buttonPtr->_color_text_inactive = iatxt;
    buttonPtr->_color_background = back;
    buttonPtr->_color_border = brd;
    buttonPtr->_color_hilite = brdhi;
}

// return any button specific colors for this buttonID
buttonExt* UTFT_DLB_Buttons::getButtonColors(int buttonID)
{
    buttonExt* buttonPtr = buttonsXtn;
    while (buttonPtr != NULL)
    {
        if (buttonPtr->id == buttonID)
        {
            break;
        }
        buttonPtr = buttonPtr->next;
    }
    return buttonPtr;
}

// remove any button specific colors for this buttonID
void UTFT_DLB_Buttons::clearButtonColors(int buttonID)
{
    buttonExt* buttonPtr = buttonsXtn;
    buttonExt* prevButtonPtr = buttonsXtn;
    while (buttonPtr != NULL)
    {
        if (buttonPtr->id == buttonID)
        {
            if (buttonPtr == buttonsXtn)
            {
                // first button special case
                buttonsXtn = buttonPtr->next; // remove button from the list
                delete buttonPtr;   // free the memory
            }
            else
            {
                prevButtonPtr->next = buttonPtr->next; // remove button from the list
                delete buttonPtr;   // free the memory
            }
            break;
        }
        prevButtonPtr = buttonPtr;
        buttonPtr = buttonPtr->next;
    }
}

// non-blocking check of button being pressed
bool UTFT_DLB_Buttons::testButton(int id)
{
    if (_UTouch->dataAvailable() == true)
    {
		_UTouch->read();
		int		touch_x = _UTouch->getX();
		int		touch_y = _UTouch->getY();
		word	_current_color = _UTFT->getColor();
  
		if (((buttons[id].flags & BUTTON_UNUSED) == 0) and ((buttons[id].flags & BUTTON_DISABLED) == 0))
		{
			if ((touch_x >= buttons[id].pos_x) and (touch_x <= (buttons[id].pos_x + buttons[id].width)) and (touch_y >= buttons[id].pos_y) and (touch_y <= (buttons[id].pos_y + buttons[id].height)))
         {
				return true;
         }
		}
    }
      
   return false;
}

// this version copies the string in case the original is not a string literal
int UTFT_DLB_Buttons::addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags)
{
    int btcnt = UTFT_Buttons::addButton(x, y, width, height, label, flags);
    
	 buttons[btcnt].label = new char[strlen(label)+1];
    strcpy(buttons[btcnt].label, label);  // copy the string
    return btcnt;
}

// make sure we free up memory when we delete buttons
void UTFT_DLB_Buttons::deleteButton(int buttonID)
{
  buttons[buttonID].pos_x=0;
  buttons[buttonID].pos_y=0;
  buttons[buttonID].width=0;
  buttons[buttonID].height=0;
  if (buttons[buttonID].flags!=BUTTON_UNUSED)
  {
    delete [] buttons[buttonID].label; // free the memory we grabbed
  }
  buttons[buttonID].flags=BUTTON_UNUSED;
  buttons[buttonID].label=NULL;
  
  // remove any memory we grabbed for button colors
  clearButtonColors(buttonID);
}

void UTFT_DLB_Buttons::deleteAllButtons()
{
	for (int i=0;i<MAX_BUTTONS;i++)
	{
       deleteButton(i);
	}
}

UTFT_DLB_Buttons::~UTFT_DLB_Buttons()
{
    deleteAllButtons();
}

// This version copies the string in case it's in a temporary variable (like a value)
void UTFT_DLB_Buttons::relabelButton(int buttonID, char *label, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
      delete [] buttons[buttonID].label;  // free old memory
       
      buttons[buttonID].label = new char[strlen(label)+1];
      strcpy(buttons[buttonID].label, label);   // create space and copy the string
		if (redraw)
      {
			drawButton(buttonID);
      }
	}
}

// This drawButton allows proportional fonts and fixed fonts    
void UTFT_DLB_Buttons::drawButton(int buttonID)
{
	_UTFT->setFont(_font_text);
    
 	 word	old_color_text = _color_text;
    word old_color_text_inactive = _color_text_inactive;
    word old_color_background = _color_background;
    word old_color_border = _color_border;
    word old_color_hilite = _color_hilite;    
    
    buttonExt* tempBtn;
    if ((tempBtn = getButtonColors(buttonID)) != NULL)
    {
        _color_text = tempBtn->_color_text;
        _color_text_inactive = tempBtn->_color_text_inactive;
        _color_background = tempBtn->_color_background;
        _color_border = tempBtn->_color_border;
        _color_hilite = tempBtn->_color_hilite;    
    }
    
   if (_UTFT->getFontXsize() != 0)
   {
       return UTFT_Buttons::drawButton(buttonID);
   }

	if ((buttons[buttonID].flags & BUTTON_BITMAP) ||
		(buttons[buttonID].flags & BUTTON_SYMBOL))
	{
       return UTFT_Buttons::drawButton(buttonID);
	}
	else
	{
       int		text_x, text_y;
       uint8_t	*_font_current = _UTFT->getFont();
       word	_current_color = _UTFT->getColor();
       word	_current_back  = _UTFT->getBackColor();
       
		_UTFT->setColor(_color_background);
		_UTFT->fillRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		_UTFT->setColor(_color_border);
		_UTFT->drawRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		if (buttons[buttonID].flags & BUTTON_DISABLED)
      {
			_UTFT->setColor(_color_text_inactive);
      }
		else
      {
			_UTFT->setColor(_color_text);
      }
     
		_UTFT->setFont(_font_text);
		text_x = ((buttons[buttonID].width/2) - ((((UTFT_DLB*)_UTFT)->getStringWidth(buttons[buttonID].label))/2)) + buttons[buttonID].pos_x;
		text_y = (buttons[buttonID].height/2) - (((UTFT_DLB*)_UTFT)->getFontHeight()/2) + buttons[buttonID].pos_y;
      
		//_UTFT->setBackColor(_color_background);
      _UTFT->setBackColor(VGA_TRANSPARENT);
      
		((UTFT_DLB*)_UTFT)->print(buttons[buttonID].label, text_x, text_y);
      
       _UTFT->setFont(_font_current);
       _UTFT->setColor(_current_color);
       _UTFT->setBackColor(_current_back);      
	}
   
 	 _color_text = old_color_text;
    _color_text_inactive = old_color_text_inactive;
    _color_background = old_color_background;
    _color_border = old_color_border;
    _color_hilite = old_color_hilite;   
}