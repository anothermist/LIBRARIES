/*********************
   http://www.digole.com/forum   communicate with us and other user
   http://www.digole.com    buyer Digole module
 **********************/


//You have to change the following definition according to the Digole display
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_

#define _Universal_Graphic_Adapter_
//  #define _Universal_Char_Adapter_
//  #define COL 20    //change for your char lcd
//  #define ROW 4     //change for your char lcd

#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire, '\x27'); //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8, 9, 10, 11); //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
void setup() {
  mydisp.begin(); //initiate serial port
  /*------Set Uart Baud------*/
  //in order to set Uart Baud same with Arduino, we suggest you set the Baud at the beginning code, but you also
  //can change the Baud by send "SB" follow the rate to moudle
  //  Serial->print("SB");
  //  Serial->println(newBaud);
  //  delay(100);
  //  Serial->begin(newBaud);

  /*------Change I2C address------*/
  //mydisp.setI2CAddress(newI2Caddress);

  /*------Change splash screen and display serial config setting ----*/
  //mydisp.displayConfig(0);  //0=disable, 1=enable display the serial port configuration show when power up
  //mydisp.displayStartScreen(0); //0=disable, 1=enable display flush screen when power up

  /*------Universal graphic adapter setting------*/
  //this adapter can work with ST7920, KS0108 and ST7565 LCD controller
#ifdef _Universal_Graphic_Adapter_
  //set adapter for ST7920 controller
  mydisp.clearScreen();
  mydisp.setLCDChip(0);
  delay(4000);
  mydisp.drawStr(0, 0, "command:SLCD0");
  delay(2000);
  //set for KS0108 controller, the E,CS1,CS2 on LCD can be set low or high level active
  //you should see: "command:SLCD4xxx" on screen when config is correct, remember xxx
  //remember:you have to short the 2x KS0108 jumpers on the adapter board
  for (unsigned char i = 0; i < 2; i++)
    for (unsigned char j = 0; j < 2; j++)
    {
      mydisp.setLCDChip(4);
      mydisp.write(i);  //E logic level
      mydisp.write(j);  //CS1 logic level
      mydisp.write(j);  //CS2 logic level
      delay(4000);
      mydisp.drawStr(0, 0, "command:SLCD4");
      mydisp.print(i);
      mydisp.print(j);
      mydisp.print(j);
      delay(2000);
    }
  //set work for ST7565 controller
  mydisp.setLCDChip(2);
  delay(2000);
  mydisp.drawStr(0, 0, "command:SLCD2");
#endif

  /*------Universal Character adapter setting----*/
#ifdef _Universal_Char_Adapter_
  mydisp.setLCDColRow(COL, ROW);
#endif
}

void loop() {

}
