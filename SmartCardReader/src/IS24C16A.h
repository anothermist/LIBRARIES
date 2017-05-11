#if !defined(_IS24C16A_H)
#define _IS24C16A_H

#include "Arduino.h"
#include "SmartCardReader.h"

#define DEVICE_ADDRESS 0xA0		// Device Address
#define ACK  0        			// ACK
#define NAK  1        			// NAK

class IS24C16A : public SmartCardReader
{
public:
	IS24C16A(int IOPin = A4, int CLKPin = A5, int CDPin = 3, int RSTPin = 2, int Delay = 50);
	
	bool IsDevicePresent(uint8_t Page);
	void Initialize();
	uint8_t Read(uint8_t Page, uint8_t Location);
	void Write(uint8_t Page, uint8_t Location, uint8_t Val);
	void WriteString(uint8_t Page, uint8_t Location, const char *String);
	void ReadString(uint8_t Page, uint8_t Location, int Len, char *String);
	void ClearString(uint8_t Page, uint8_t Location, int Len);
private:
	void _Start();
	void _Stop();
	void _Process();
	uint8_t _Read(bool ackbit);
	bool _Write(uint8_t data);

};

#endif // _IS24C16A_H