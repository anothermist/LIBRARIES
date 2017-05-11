#if !defined(_SMARTCARDREADER_H)
#define _SMARTCARDREADER_H

#include "Arduino.h"

class IS24C02A;
class IS24C16A;
class IS23SC4442;

class SmartCardReader
{
public:
	SmartCardReader(int IOPin, int CLKPin, int CDPin, int RSTPin, int Delay)
	{
		IO = IOPin;
		CLK = CLKPin;
		CD = CDPin;
		RST = RSTPin;
		DelayAmt = Delay;

		Input(CD);
		
		_HoldCardDetected = false;
	}
	
	bool IsCardDetected();
	
protected:
	bool Get(int Pin);
	void Set_Low(int Pin);
	void Set_High(int Pin);
	void Input(int Pin);
	void Output(int Pin);
	
	int IO;
	int CLK;
	int CD;
	int RST;
	int DelayAmt;
	
private:
	virtual void _OnCardDetected() 
	{ 
		// We don't really need to do anything for IS24SC02A and IS24SC16A Cards 
		// but there is an override of this function for the IS23SC4442 Card
	}
	bool _HoldCardDetected;
};

#endif // _SMARTCARDREADER_H