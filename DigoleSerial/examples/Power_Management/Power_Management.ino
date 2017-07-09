/*********************
   http://www.digole.com/forum   communicate with us and other user
   http://www.digole.com    buyer Digole module
 **********************/


//You have to change the following definition according to the Digole display
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
//#define MONO  //if the screen panel is monochrome
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 38400); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
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
char i;

void delay1(void) //for demo use, reset display position and clean the demo line
{
  delay(3000); //delay 1.5 seconds
  mydisp.setColor(255);
}

void setup() {
  mydisp.begin(); //initiate serial port
  /*------Energy management------*/
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Turn off backlight");
  delay1();
  mydisp.backLightOff();
  delay1();
  mydisp.backLightOn();
  mydisp.drawStr(0, 0, "Backlight is on now");
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Turn screen off");
  delay1();
  mydisp.screenOnOff(0);  //turn screen off
  delay1();
  mydisp.screenOnOff(1);  //turn screen on
  mydisp.drawStr(0, 0, "Turn screen on ");
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Turn CPU off, you can measure the module current");
  mydisp.cpuOff();  //turn cpu off
  delay1();
  delay1();
  for (i = 0; i < 10; i++)
    mydisp.write(0);  //write some dummy data to wake up cpu
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "CPU is on now");
  delay1();
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Shut down whole module");
  delay1();
  mydisp.moduleOff();   //put whole module in sleep: Back light off, screen in sleep, MCU in sleep
  delay1();
  for (i = 0; i < 10; i++)
    mydisp.write(0);  //write some dummy data to wake up cpu
  mydisp.cleanDrawWindow();
  mydisp.drawStr(0, 0, "Module resumed");
}

void loop() {
  // put your main code here, to run repeatedly:

}
