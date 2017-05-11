#include "SmartCardReader.h"

bool SmartCardReader::IsCardDetected()
{
	bool CardDetected = Get(CD);
	// The purpose of this is to allow for a reset of the Read Buffer
	// for IS23SC4442 Cards when a new card is inserted into the Reader
	// and we only want to do it if a) The CardDetected Value has 
	// changed since last time, and b) There is a Card Present in the
	// Reader.  If this isn't an IS23SC4442 Card, the _OnCardDetected()
	// function doesn't do anything anyhow.
	if (CardDetected != _HoldCardDetected)
	{
		_HoldCardDetected = CardDetected;
		if (CardDetected)
			_OnCardDetected();
	}
	return CardDetected;
}

bool SmartCardReader::Get(int Pin)
{
  return digitalRead(Pin);
}

void SmartCardReader::Set_Low(int Pin)
{
  digitalWrite(Pin, LOW);
  delayMicroseconds(DelayAmt);
}

void SmartCardReader::Set_High(int Pin)
{
  digitalWrite(Pin, HIGH);
  delayMicroseconds(DelayAmt);
}

void SmartCardReader::Input(int Pin)
{
  pinMode(Pin, INPUT);
}

void SmartCardReader::Output(int Pin)
{
  pinMode(Pin, OUTPUT);
}
