// THIS SOFTWARE IS PROVIDED TO YOU "AS IS," AND WE MAKE NO EXPRESS OR IMPLIED WARRANTIES WHATSOEVER 
// WITH RESPECT TO ITS FUNCTIONALITY, OPERABILITY, OR USE, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR INFRINGEMENT. WE EXPRESSLY 
// DISCLAIM ANY LIABILITY WHATSOEVER FOR ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL OR SPECIAL 
// DAMAGES, INCLUDING, WITHOUT LIMITATION, LOST REVENUES, LOST PROFITS, LOSSES RESULTING FROM BUSINESS 
// INTERRUPTION OR LOSS OF DATA, REGARDLESS OF THE FORM OF ACTION OR LEGAL THEORY UNDER WHICH THE 
// LIABILITY MAY BE ASSERTED, EVEN IF ADVISED OF THE POSSIBILITY OR LIKELIHOOD OF SUCH DAMAGES.

// Use Card 32322

// This Sample Application Performs the following (All output goes to the Serial Monitor):
//
// 1 - Writes same character to each memory location 0 through 255 dependent upon page #
//     i.e. Writes '1' for Page 1, '2' for Page 2, '3' for Page 3 ... all the way to page 8.
// 2 - Displays a memory dump of all 8 pages
// 3 - Clears all memory locations of all pages (sets all locations to 0)
// 4 - Displays another memory dump of all 8 pages
// 5 - Runs through a test of the String Functions Available (test uses Page 5 only)
//
// Components required:
//
// An Arduino or compatible should work ... 
//    I tested with an Arduino UNO SMD, an Arduino UNO R3 and an Arduino Mega 2560 R3 ... all 3 worked.
// A Parallax Smart Card Reader.
// A IS24C16A (32322) Smart Card.
//

#include "Arduino.h"
#include "IS24C16A.h"

IS24C16A CardReader;

bool CardDetected = 0;   // Last Card Detect Event

void setup() 
{
  Serial.begin(9600);
  
  Serial.println("Note: 32322 Card Expected");
  
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
      
      for (uint8_t page = 0; page < 8; page++)
      {
        if (CardReader.IsDevicePresent(page))
          Write_Memory(page);
        else
          Serial.println("Device not present.");  
      }
      
      for (uint8_t page = 0; page < 8; page++)
      {
        if (CardReader.IsDevicePresent(page)) 
          Read_Memory(page);
        else
          Serial.println("Device not present.");  
      }

      for (uint8_t page = 0; page < 8; page++)
      {
        if (CardReader.IsDevicePresent(page)) 
          Clear_Memory(page);
        else
          Serial.println("Device not present.");  
      }
      
      for (uint8_t page = 0; page < 8; page++)
      {
        if (CardReader.IsDevicePresent(page)) 
          Read_Memory(page);
        else
          Serial.println("Device not present.");  
      }
	  
	  Test_String();
    }
    else
      Serial.println("Card Removed!");
    CardDetected = cd;
  }
  
  delay(1000);
}

void Clear_Memory(uint8_t page)
{
  Serial.print("Clearing Memory...Page ");
  Serial.println(page + 1);
  
  for (int i = 0; i <= 255; i++)
  {
    CardReader.Write(page, (uint8_t) i, 0x00);
  }
  
  Serial.println(" Done.");
}

void Write_Memory(uint8_t page)
{
  Serial.print("Writing Memory...Page ");
  Serial.println(page + 1);
  
  for (int i = 0; i <= 255; i++)
  {
    CardReader.Write(page, (uint8_t) i, (uint8_t) (page + '1'));
  }
  
  Serial.println(" Done.");
}

void Read_Memory(uint8_t page)
{
  Serial.print("Reading Memory...Page ");
  Serial.println(page + 1);
  
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
    uint8_t Temp = CardReader.Read(page, (uint8_t) index);
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
  Serial.println("String Function Test...Page 6");  
  Serial.println(" Writing String");
  CardReader.WriteString(5, 0x0A, "This is a Test.");
  Serial.println(" Done.");  

  Serial.println(" Reading String");
  char ReadBuffer[16];
  CardReader.ReadString(5, 0x0A, 15, ReadBuffer);
  Serial.print(" String Read: ");
  Serial.println(ReadBuffer);
  Serial.print(" Length of String: ");
  Serial.println(strlen(ReadBuffer));
  Serial.println(" Done.");  

  Read_Memory(5);

  Serial.println(" Clearing String");
  CardReader.ClearString(5, 0x0A,15);
  Serial.println(" Done.");  

  Read_Memory(5);
}