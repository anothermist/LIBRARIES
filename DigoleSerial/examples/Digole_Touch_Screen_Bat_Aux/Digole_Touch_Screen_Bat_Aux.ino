/*********************
Digole Serial display module with touch screen demo
NOTE: if you using UART mode, you need to disconnect the RX from Arduino
When you uploading your sketch
**********************/
#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 38400); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
//IMPORTANT: A 4.7K~20K PULL DOWN RESISTOR NEEDED BETWEEN SI(11) AND GROUND IN ORDER TO OPERATE TOUCH SCREEN
//AND FLASH MEMORY CORRECTLY
DigoleSerialDisp mydisp(8,9,10,11);  //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
int readBattery(void);
int readAux(void);
int readTemperature(void);
void setup()
{
    mydisp.begin();
    mydisp.clearScreen();
    mydisp.drawStr(2,5,"The battery Voltage:");
    mydisp.drawStr(2,7,"The Aux:");
    mydisp.drawStr(2,9,"Temperature:");
//Serial.begin(9600);
}
void loop()
{
    int batVolt,Aux,tmp;
    batVolt=mydisp.readBattery();
    Aux=mydisp.readAux();
    tmp=mydisp.readTemperature();
    tmp=(tmp>>1)+(tmp>>4)+(tmp>>5)+(tmp>>6)+(tmp>>10);  //tmp=tmp*2500/4096 (mv)
    tmp=653-tmp;  //0 degree is about 653mv
    tmp=(tmp>>1)-(tmp>>5)+(tmp>>7); //2.1mv/degree
    mydisp.drawStr(23,5,"     ");
    mydisp.setPrintPos(23,5);
    mydisp.print(batVolt);
    mydisp.drawStr(23,7,"     ");
    mydisp.setPrintPos(23,7);
    mydisp.print(Aux);
    mydisp.drawStr(23,9,"     ");
    mydisp.setPrintPos(23,9);
    mydisp.print(tmp);
    delay(200);
}


