/*----------NOTE----------
new version of lib will save you compiled code size for the sample
The size for 2 version(@Arduino 1.0.1):
      UART  I2C    SPI
OLD   8998  8988   9132
NEW   6966  7566   6354
------------------------*/
#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8,9,10);  //SPI:Pin 8: data, 9:clock, 10: SS, you can assign 255 to SS, and hard ground SS pin on module
#endif
#define LCDCol 16
#define LCDRow 2

int ptr;
const char a[] = "disp char array";
const char b = 'Q';
int c = 3456;
String d = "I'm a string";
float pi = 3.1415926535;
double lg10;

void resetpos(void) //for demo use, reset display position and clean the demo line
{
    mydisp.setPrintPos(0, 1);
    delay(2000); //delay 2 seconds
    mydisp.println("                "); //display space, use to clear the demo line
    mydisp.setPrintPos(0, 1);
}

void setup() {
    mydisp.begin();
    /*----------for text LCD adapter and graphic LCD adapter ------------*/
    mydisp.clearScreen(); //CLear screen
    //mydisp.displayConfig(1);    //set config display ON, 0=off
    //mydisp.setI2CAddress(0x29);  //this function only working when you connect using I2C, from 1 to 127
    //delay(1000);
    //mydisp.setLCDColRow(16,2);  //set LCD Col and Row, only time set up is OK
    mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
    mydisp.drawStr(4, 0, "Demo now"); //display string at: x=4, y=0

    //Test print function
    mydisp.setPrintPos(0, 1);
    mydisp.print(a); // display a char array
    resetpos();
    mydisp.print("display a char:");
    mydisp.print(b); //display a char
    resetpos();
    mydisp.print("int as DEC:");
    mydisp.print(c); //display 3456 in Dec
    resetpos();
    mydisp.print("as HEX:");
    mydisp.print(c, HEX); //display 3456 in Hex
    resetpos();
    mydisp.print("as OCT:");
    mydisp.print(c, OCT); //display 3456 in Oct
    resetpos();
    mydisp.print("BIN:");
    mydisp.print(c, BIN); //display 3456 in Bin
    resetpos();
    mydisp.print(d); //display String object
    resetpos();
    mydisp.print("float pi=");
    mydisp.print(pi); //display float of PI
    resetpos();
    mydisp.print("Pi6=");
    mydisp.print(pi, 6); //display PI in 6 Accuracy
    resetpos();
    mydisp.print("Pi*3=");
    mydisp.print(pi * 3, 6); //display PI time 3 in 6 Accuracy
    resetpos();
    mydisp.print("lg5=");
    mydisp.print(log(5), 8); //display log(5) in 8 Accuracy
    resetpos();
    for (uint8_t j = 0; j < 4; j++) //making "Hello" string moving
    {
        for (uint8_t i = 0; i < 10; i++) //move string to right
        {
            mydisp.setPrintPos(i, 1);
            mydisp.print(" Hello ");
            delay(100); //delay 100ms
        }
        for (uint8_t i = 0; i < 10; i++) //move string to left
        {
            mydisp.setPrintPos(9 - i, 1);
            mydisp.print(" Hello ");
            delay(100);
        }
    }
    delay(2000);
    mydisp.clearScreen();
    mydisp.print("Turn Off backlight after 2 seconds");
    delay(2000);
    mydisp.backLightOff();
    delay(2000);
    mydisp.clearScreen();
    mydisp.print("Turn On backlight");
    mydisp.backLightOn();
    mydisp.setPrintPos(0, 1);
    mydisp.print("Enjoy it! www.digole.com");
    mydisp.enableCursor(); //enable cursor
}
void loop() {
}
