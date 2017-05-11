#include <IS23SC4442.h>

IS23SC4442::IS23SC4442(int IOPin, int CLKPin, int CDPin, int RSTPin, int Delay)
	: SmartCardReader(IOPin, CLKPin, CDPin, RSTPin, Delay)
{
	_ReadRefresh = true;
}

void IS23SC4442::Initialize()
{
  Output(CLK);
  Input(IO);
  Output(RST);
  Set_Low(RST);
  Set_Low(CLK);
  Set_High(RST);
  delayMicroseconds(DelayAmt); // A little extra wait time for this
  Set_High(CLK);
  Set_Low(CLK);
  delayMicroseconds(DelayAmt); // A little extra wait time for this
  Set_Low(RST);
  for (int index = 0; index < 4; index++)
  {
    _Read();
  }
}

uint8_t IS23SC4442::Read(uint8_t Location)
{
  if (_ReadRefresh)
  {
	  _ReadRefresh = false;
      _SendCommand(ReadMain, 0x00, 0x00);
	  for (int i = 0; i < 256; i++)
		  _ReadBuffer[i] = _Read();
  }
  
  return _ReadBuffer[Location];
}

void IS23SC4442::Write(uint8_t Location, uint8_t Val)
{
  _ReadRefresh = true;
  _SendCommand(UpdateMain, Location, Val);
}

void IS23SC4442::ReadString(uint8_t Location, int Len, char *String)
{
	for (int i = 0; i < Len; i++)
	{
		if (Location < 256)
		{
			*String++ = (char) Read(Location++);
			*String = (char) NULL;
		}
	}
}

void IS23SC4442::WriteString(uint8_t Location, const char *String)
{
	for (int i = 0; i < strlen(String); i++)
	{
		if (Location < 256)
		{
			// For some reason, I have to write the first character
			// of the string twice for IS23SC4442 cards ... no idea why.
			if (i == 0)
				Write(Location, (uint8_t) String[i]);
			
			Write(Location++, (uint8_t) String[i]);
			if (Location < 256)
				Write(Location, (uint8_t) NULL);
		}
	}
}

void IS23SC4442::ClearString(uint8_t Location, int Len)
{
	for (int i = 0; i < Len; i++)
	{
		if (Location < 256)
		{
			// For some reason, I have to write the first character
			// of the string twice for IS23SC4442 cards ... no idea why.
			if (i == 0)
				Write(Location, (uint8_t) NULL);
			
			Write(Location++, (uint8_t) NULL);
		}
	}
}

uint8_t IS23SC4442::Authenticate()
{
  uint8_t Data = 0x03;  // Just make sure we never lock the Card
  
  uint8_t Password1 = 0xFF;  // 1st byte of PSC
  uint8_t Password2 = 0xFF;  // 2nd byte of PSC 
  uint8_t Password3 = 0xFF;  // 3rd byte of PSC
  
  uint8_t errCounter = _ReadSecurity();
  
  errCounter = errCounter & 0x07;

  switch (errCounter)
  {
    case 0x07:
      Data = 0x03;
      break;
    case 0x03:
      Data = 0x03;  // was 0x01 ... only one less and we'll lock the card
      break;
    case 0x01:
      Data = 0x03;  // was 0x00 ... which will lock the Card
      break;
    case 0x00:
      Data = 0x03;
      break;
  }
  
  _SendCommand(UpdateSecurity, 0x00, Data);
  _SendCommand(CompareData, 0x01, Password1);
  _SendCommand(CompareData, 0x02, Password2);
  _SendCommand(CompareData, 0x03, Password3);
  _SendCommand(UpdateSecurity, 0x00, 0x07);   // Reset Counter to it's highest value (so we don't ever lock the Card)
 
  return errCounter;
}

void IS23SC4442::_SendCommand(uint8_t Command, uint8_t Address, uint8_t Data) 
{
  Output(CLK);
  Set_High(CLK);
  Output(IO);
  Set_Low(IO);
  _Write(Command);
  _Write(Address);
  _Write(Data);
  Set_Low(CLK);
  Set_Low(IO);
  Set_High(CLK);
  Set_High(IO);
  
  if (Command == UpdateMain || Command == UpdateProtection || Command == CompareData || Command == UpdateSecurity)
    _Process();
}

void IS23SC4442::_Process()
{
  Input(IO);
  Set_Low(CLK);
  
  while (Get(IO) == 0)
  {
    Set_High(CLK);
    Set_Low(CLK);
  }
}


// I tried to get shiftIn to work, but it didn't.
uint8_t IS23SC4442::_Read()
{
  uint8_t data = 0;
  bool bit = 0;
  
  Output(CLK);
  Input(IO);
  Set_High(CLK);
  
  for (int i = 0; i < 8; i++)
  {
      data = data >> 1;
      Set_Low(CLK);   
      Set_High(CLK);
      bit = Get(IO);
      if (bit) 
      {
        data += 0x80;
      }
  }
  
  return data;
}

// I tried to get shiftOut to work, but it didn't.
void IS23SC4442::_Write(uint8_t data)
{
    uint8_t command = data;
    bool temp = 0;
    
    for (int i = 0; i < 8; i++)
    {
      temp = command & 0x01;
      command = command >> 1;
      Set_Low(CLK);
      if (temp != 0)
        Set_High(IO);
      else
        Set_Low(IO);
      Set_High(CLK);
    }
}

uint8_t IS23SC4442::_ReadSecurity()
{
  _SendCommand(ReadSecurity, 0x00, 0x00);
  uint8_t errCounter = _Read();
 
  _SendCommand(ReadSecurity, 0x01, 0x00);
  uint8_t PSW1 = _Read();
  
  _SendCommand(ReadSecurity, 0x02, 0x00);
  uint8_t PSW2 = _Read();
  
  _SendCommand(ReadSecurity, 0x03, 0x00);
  uint8_t PSW3 = _Read();
  
  return errCounter;
}

void IS23SC4442::_OnCardDetected()
{
	_ReadRefresh = true;
}

