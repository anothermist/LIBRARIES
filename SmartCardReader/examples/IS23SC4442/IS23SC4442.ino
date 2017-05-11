// THIS SOFTWARE IS PROVIDED TO YOU "AS IS," AND WE MAKE NO EXPRESS OR IMPLIED WARRANTIES WHATSOEVER 
// WITH RESPECT TO ITS FUNCTIONALITY, OPERABILITY, OR USE, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR INFRINGEMENT. WE EXPRESSLY 
// DISCLAIM ANY LIABILITY WHATSOEVER FOR ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL OR SPECIAL 
// DAMAGES, INCLUDING, WITHOUT LIMITATION, LOST REVENUES, LOST PROFITS, LOSSES RESULTING FROM BUSINESS 
// INTERRUPTION OR LOSS OF DATA, REGARDLESS OF THE FORM OF ACTION OR LEGAL THEORY UNDER WHICH THE 
// LIABILITY MAY BE ASSERTED, EVEN IF ADVISED OF THE POSSIBILITY OR LIKELIHOOD OF SUCH DAMAGES. 

// Use Card 32321

// This Sample Application Performs the following (All Output goes to the Serial Monitor):
//
// 1 - Resets Card
// 2 - Checkes for Authenticated Password (FF FF FF).  
//     Note: Being that I locked my first card ... and then found out even the PBasic 2.5 and Propeller Demos would automatically lock after 3 card inserts
//           I have taken pains to make sure that it shouldn't happen again ... 
//           but if you need that type of security ... 
//           you may want to look at the compare function and make some changes.
//           But for testing ... locking up the card (so you can't write values) shouldn't be possible with this code (but no guarantees).
// 3 - Writes characters 0 through 255 to memory location 0 through 255 
// 4 - Displays a memory dump
// 5 - Clears all memory locations
// 6 - Displays another memory dump.
// 7 - Runs through a Test of the String Functions available.
//
// Components required:
//
// An Arduino or compatible should work ... 
//    I tested with an Arduino UNO SMD, an Arduino UNO R3 and an Arduino Mega 2560 R3 ... all 3 worked.
// A Parallax Smart Card Reader.
// A IS23SC4442 (32321) Smart Card.
//

#include "Arduino.h"
#include "IS23SC4442.h"

IS23SC4442 CardReader;

bool CardDetected = 0;   // Last Card Detect Event

void setup() 
{
	Serial.begin(9600);

	Serial.println("Note: 32321 Card Expected");

	CardDetected = CardReader.IsCardDetected();

	if (!CardDetected)
		Serial.println("Please insert a Card...");
	else
		Serial.println("Card was already inserted at startup...Please remove and re-insert card.");
}

void loop() 
{
	bool cd = CardReader.IsCardDetected();

	if (cd != CardDetected)
	{
		if (cd) 
		{ 
			Serial.println("Card Inserted!");
			CardReader.Initialize();
			if (IsAuthenticated())
			{
				Write_Memory();
				Read_Memory();
				Clear_Memory();
				Read_Memory();
				Test_String();
			}
			else
				Serial.println("Authentication Failed!");
		}
		else
			Serial.println("Card Removed!");
		CardDetected = cd;
	}

	delay(1000);
}

bool IsAuthenticated()
{
	Serial.println("Authenticating...");

	uint8_t errCounter = CardReader.Authenticate();
	switch (errCounter)
	{
		case 0x00:  // Hopefully we'll NEVER see this ... because once it's locked it can't be unlocked as far as I know
			Serial.println(" Card is locked.");
			break;
		case 0x01:
			Serial.println(" Invalid PSC, one try remaining.");
			break;
		case 0x03:
			Serial.println(" Invalid PSC, two tries remaining.");
			break;
		case 0x07:
			Serial.println(" PSC verified, you may now write data to the card.");
			break;
		default:
			Serial.println(" Unabled to Authenticate.");
			break;
	}

	Serial.println(" Done.");

	return (errCounter == 0x07);
}

void Write_Memory()
{
	Serial.println("Write Memory...");
	for (int i = 0; i <= 255; i++)
		CardReader.Write((uint8_t) i, (uint8_t) i);
	Serial.println(" Done.");
}

void Clear_Memory()
{
	Serial.println("Clear Memory...");
	for (int i = 0; i <= 255; i++)
		CardReader.Write((uint8_t) i, (uint8_t) 0);
	Serial.println(" Done.");
}

void Read_Memory()
{
	Serial.println("Read Memory...");
	
	uint8_t bstr[16];
	int bcnt = 0;

	Serial.println("                            [ D A T A ]");
	Serial.print(" * |");
	for (int i = 0; i < 16; i++)
	{
		Serial.print("  ");
		Serial.print(i, HEX);
	}
	Serial.print("    characters");
	Serial.println("");
	Serial.println(" --+-----------------------------------------------------------------");
	for (int index = 0; index <= 255; index++)
	{
		if ((index % 16) == 0)
		{
			if (index != 0)
				Serial.println("");
			Serial.print(" ");
			Serial.print(index / 16, HEX);
			Serial.print(" |");
		}
		uint8_t Temp = CardReader.Read((uint8_t) index);
		bstr[bcnt++] = Temp;
		Serial.print(" ");
		if (Temp < 0x10) // add a leading zero if < 10 hex.
			Serial.print("0");
		Serial.print(Temp, HEX);
		if  ((index % 16) == 15)
		{
			Serial.print(" ");
			for (int b = 0; b < 16; b++)
			{
				if (bstr[b] < 0x20 || bstr[b] > 0x7f)
					Serial.print('.');
				else
					Serial.print((char)bstr[b]);
			}
			bcnt = 0;
		}
	}
	Serial.println("");
	Serial.println(" --------------------------------------------------------------------");
	Serial.println(" Done.");  
}

void Test_String()
{
  Serial.println("String Function Test...");  
  Serial.println(" Writing String");
  CardReader.WriteString(0x0A, "This is a Test.");
  Serial.println(" Done.");  

  Serial.println(" Reading String");
  char ReadBuffer[16];
  CardReader.ReadString(0x0A, 15, ReadBuffer);
  Serial.print(" String Read: ");
  Serial.println(ReadBuffer);
  Serial.print(" Length of String: ");
  Serial.println(strlen(ReadBuffer));
  Serial.println(" Done.");  

  Read_Memory();

  Serial.println(" Clearing String");
  CardReader.ClearString(0x0A,15);
  Serial.println(" Done.");  

  Read_Memory();
}

