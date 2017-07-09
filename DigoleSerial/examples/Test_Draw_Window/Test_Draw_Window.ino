#define LCDWidth 240  //define screen width,height
#define LCDHeight 320
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 

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

//define draw window
#define DW_X 5
#define DW_Y 8
#define DW_W (LCDWidth - 10)
#define DW_H (LCDHeight - 10)
unsigned char i;
void setup() {
  mydisp.begin(); //initiate serial port
  mydisp.setBgColor(255); //set another back ground color
  mydisp.clearScreen(); //CLear screen
  mydisp.setColor(0);
  for (i = 0; i < 4; i++)
  {
    mydisp.setRotation(i);
    mydisp.drawStr(0, 0, "TEST"); //print string at 3,0 in draw window
  }
  delay(1000);
  mydisp.setRotation(0);  //change screen direction to 1, screen width and screen height exchanged
  mydisp.setDrawWindow(DW_X, DW_Y, DW_W, DW_H); //set a draw window left/top:5,8, Height=LCDHeight-15, width=LCDWidth-10
  mydisp.setColor(255);
unsigned char j=0;

for (i = 0; i < 16; i++)
  {
mydisp.setBgColor(j);
mydisp.setColor(~j);

//    mydisp.setBgColor(random(256)); //set another back ground color
    mydisp.setRotation(i % 4);
    mydisp.cleanDrawWindow();
    mydisp.drawStr(0, 0, "TEST"); //print string at 3,0 in draw window
    mydisp.print(i);
    delay(2000);
    j=~j;
  }
  for (i = 0; i < 4; i++)
  {
    mydisp.setRotation(i);
    mydisp.drawStr(0, 0, "TEST"); //print string at 3,0 in draw window
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
