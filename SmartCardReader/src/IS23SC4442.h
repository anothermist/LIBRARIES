#if !defined(_IS23SC4442_H)
#define _IS23SC4442_H

#include "Arduino.h"
#include "SmartCardReader.h"

#define DEVICE_ADDRESS 0xA0		// Device Address
#define ACK  0        			// ACK
#define NAK  1        			// NAK

#define ReadMain           0x30  // Read main memory
#define ReadProtection     0x34  // Read protection bit
#define ReadSecurity       0x31  // Read PSC
#define UpdateMain         0x38  // Update main memory
#define UpdateSecurity     0x39  // Update PSC
#define UpdateProtection   0x3C  // Update Protection bits
#define CompareData        0x33  // Compare Verification Data

class IS23SC4442 : public SmartCardReader
{
public:
	IS23SC4442(int IOPin = A4, int CLKPin = A5, int CDPin = 3, int RSTPin = 2, int Delay = 50);
	
	void Initialize();
	uint8_t Read(uint8_t Location);
	void Write(uint8_t Location, uint8_t Val);
	void WriteString(uint8_t Location, const char *String);
	void ReadString(uint8_t Location, int Len, char *String);
	void ClearString(uint8_t Location, int Len);
	uint8_t Authenticate();
	
private:
	void _Process();
	uint8_t _Read();
	void _Write(uint8_t data);
	uint8_t _ReadSecurity();
	void _SendCommand(uint8_t Command, uint8_t Address, uint8_t Data);
	void _OnCardDetected();

	bool _ReadRefresh;
	uint8_t _ReadBuffer[256];
};

#endif // _IS23SC4442_H