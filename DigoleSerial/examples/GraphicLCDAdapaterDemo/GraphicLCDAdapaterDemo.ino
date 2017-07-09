/*
 *This demo code will show you all functions for
 *Digole Graphic LCD adapter
 */
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

const unsigned char welcomeimage[] PROGMEM = {
    0, 0, 0, 0, 0, 127
    , 0, 8, 1, 2, 0, 127
    , 0, 8, 0, 148, 0, 127
    , 0, 16, 0, 89, 112, 127
    , 3, 144, 0, 16, 144, 127
    , 28, 145, 192, 16, 144, 127
    , 1, 30, 128, 80, 144, 127
    , 9, 49, 3, 208, 144, 127
    , 5, 72, 0, 80, 144, 127
    , 2, 72, 0, 150, 144, 127
    , 3, 8, 0, 152, 208, 127
    , 5, 24, 0, 64, 160, 127
    , 4, 148, 0, 64, 128, 127
    , 8, 36, 0, 128, 128, 127
    , 16, 34, 3, 240, 128, 127
    , 32, 65, 0, 14, 0, 127
    , 0, 129, 128, 1, 252, 127
    , 3, 0, 64, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
    , 0, 0, 0, 0, 0, 127
};
int ptr;
const char a[] = "disp char array";
const char b = 'Q';
int c = 3456;
String d = "I'm a string";
float pi = 3.1415926535;
double lg10;
const unsigned char fonts[] = {6, 10, 18, 51, 120, 123};
const char *fontdir[] = {"0\xb0", "90\xb0", "180\xb0", "270\xb0"};
void resetpos1(void) //for demo use, reset display position and clean the demo line
{
    mydisp.setPrintPos(0, 0, _TEXT_);
    delay(3000); //delay 2 seconds
    mydisp.println("                "); //display space, use to clear the demo line
    mydisp.setPrintPos(0, 0, _TEXT_);
}
void resetpos(void) //for demo use, reset display position and clean the demo line
{
    mydisp.setPrintPos(0, 1, _TEXT_);
    delay(3000); //delay 2 seconds
    mydisp.println("                "); //display space, use to clear the demo line
    mydisp.setPrintPos(0, 1, _TEXT_);
}

void setup() {
    mydisp.begin();
    /*----------for text LCD adapter and graphic LCD adapter ------------*/
  mydisp.setColor(1);
    mydisp.clearScreen(); //CLear screen
    //mydisp.displayConfig(1);    //set config display ON, 0=off
    //mydisp.setI2CAddress(0x29);  //this function only working when you connect using I2C, from 1 to 127
    //delay(1000);
    //mydisp.setLCDColRow(16,2);  //set LCD Col and Row, only time set up is OK
    mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
    mydisp.drawStr(2, 0, "Demo TEXT now"); //display string at: x=4, y=0
    //Test print function
    mydisp.setPrintPos(0, 1, _TEXT_);
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
    mydisp.print(d); //display String object
    resetpos();
    for (uint8_t j = 0; j < 4; j++) //making "Hello" string moving
    {
        for (uint8_t i = 0; i < 10; i++) //move string to right
        {
            mydisp.setPrintPos(i, 1, _TEXT_);
            mydisp.print(" Hello ");
            delay(100); //delay 100ms
        }
        for (uint8_t i = 0; i < 10; i++) //move string to left
        {
            mydisp.setPrintPos(9 - i, 1, _TEXT_);
            mydisp.print(" Hello ");
            delay(100);
        }
    }
    mydisp.print("Enjoy it!");
    mydisp.enableCursor(); //enable cursor
    /*---------for Graphic LCD adapter only-------*/
    mydisp.disableCursor(); //enable cursor
    resetpos();
    mydisp.drawStr(0, 1, "Negative/flash");
    mydisp.setMode('~');
    for (uint8_t i = 0; i < 10; i++) {
        mydisp.drawBox(0, 13, 110, 13);
        delay(500);
    }
    resetpos();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "Draw image in 4 dir");
    //display image in 4 different directions, use setRot90();setRot180();setRot270();undoRotation();setRotation(dir);
    for (uint8_t i = 0; i < 4; i++) {
        mydisp.setRotation(i);
        mydisp.drawBitmap(12, 12, 41, 21, welcomeimage);
    }
    mydisp.undoRotation();
    //test setting contrast
    resetpos1();
    mydisp.drawStr(0, 0, "set contrast (This display may not support this)");
    for (uint8_t i = 0; i < 63; i++) {
        mydisp.setContrast(i);
        delay(100);
    }
    mydisp.setContrast(20);
    //test drawing a box(filled rectangle)
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "draw a box");
    mydisp.drawBox(20, 15, 30, 40);
    //test drawing a circle
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "draw a circle and a pie");
    mydisp.drawCircle(31, 31, 30);
    mydisp.drawCircle(31, 31, 14, 1);
    //test drawing a Frame
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "draw a Frame");
    mydisp.drawFrame(20, 15, 30, 40);
  //Clear an area use set color =0
  resetpos1();
  mydisp.clearScreen();
  mydisp.drawStr(0, 0, "Clear an area by using set color=0");
  mydisp.drawCircle(35, 70, 35, 1);
  mydisp.drawBox(80, 40, 80, 60);
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
  mydisp.setColor(0);
  mydisp.drawCircle(110, 70, 25, 1);
  mydisp.drawBox(20, 50, 40, 30);
  mydisp.setColor(1);
    //test drawing Pixels
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "draw Pixels");
    for (uint8_t i = 0; i < 20; i++) {
        mydisp.drawPixel(20 + i * 2, 12 + random(50));
    }
    //test drawing Lines
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "draw Lines");
    for (uint8_t i = 0; i < 10; i++) {
        mydisp.drawLine(random(50), 12 + random(50), random(50), 12 + random(50));
    }
    //test drawing Lines to
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "draw Lines to");
    mydisp.drawLine(random(50), 12 + random(50), random(50), 12 + random(50));
    for (uint8_t i = 0; i < 10; i++) {
        mydisp.drawLineTo(random(50), 12 + random(50));
    }
    //test differnt font
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "default font");
    mydisp.setFont(fonts[0]);
    mydisp.nextTextLine();
    mydisp.print("Font 6");
    mydisp.setFont(fonts[1]);
    mydisp.nextTextLine();
    mydisp.print("Font 10");
    mydisp.setFont(fonts[2]);
    mydisp.nextTextLine();
    mydisp.print("Font 18");
    mydisp.setFont(fonts[3]);
    mydisp.nextTextLine();
    mydisp.print("Font 51");
    resetpos1();
    mydisp.clearScreen();
    mydisp.setFont(fonts[4]);
    mydisp.setPrintPos(0, 0, _TEXT_);
    mydisp.print("Font 120");
    mydisp.setFont(fonts[5]); //font 123 only have chars of space and 1 to 9
    mydisp.nextTextLine();
    mydisp.print("123");
    resetpos1();
    mydisp.clearScreen();
    //User font using number: 200,201,202,203,only available after upload to adapter
    mydisp.setFont(10);
    mydisp.drawStr(0, 0, "User font using:200,201,202,203,available after upload to adapter,16Kb space.");
    //Control backlight on/off
    resetpos1();
    mydisp.clearScreen();
    mydisp.drawStr(0, 0, "Turn backlight off after 2 seconds the on again");
    delay(2000);
    mydisp.backLightOff();
    delay(2000);
    mydisp.backLightOn();
    //move area on screen
    resetpos1();
    mydisp.clearScreen();
    mydisp.print("move area on screen");
    mydisp.drawCircle(31, 31, 14, 1);
    delay(2000);
    mydisp.moveArea(31, 31, 15, 15, 5, 8);
    //set display mode, affect all graphic functions, the available mode: "|"-or,"&"-and,"^"-xor,"!"-not,"C"-copy
    resetpos1();
    mydisp.clearScreen();
    mydisp.print("use xor or not mode to show a Circle Animation");
    mydisp.setMode('^');
    for (uint8_t i = 1; i < 31; i++) {
        mydisp.drawCircle(31, 31, i); //draw a circle
        delay(100);
        mydisp.drawCircle(31, 31, i); //disappear the previos draw
    }
    //this section show how to use enhanced TEXT position functions:
    //setTextPosBack(),setTextPosOffset() and setTextPosAbs() to make some thing fun
    resetpos1();
    mydisp.clearScreen();
    mydisp.setMode('|');
    mydisp.print("Bold D");
    mydisp.setFont(120);
    mydisp.nextTextLine();
    mydisp.print('D');
    mydisp.setTextPosBack(); //set text position back
    delay(2000);
    mydisp.setTextPosOffset(2, 0); //move text position to x+2
    mydisp.print('D'); //display D again, under or mode, it will bold 'D'
    //use setTextPosAbs() to make a Animation letter
    resetpos1();
    mydisp.clearScreen();
    mydisp.print("make > animation");
    mydisp.setFont(51);
    mydisp.setMode('C');
    for (uint8_t i = 0; i < 110; i++) {
        mydisp.setTextPosAbs(i, 45);
        mydisp.print('>');
        delay(30);
    }
    //show font in different direction
    resetpos1();
    mydisp.clearScreen();
    mydisp.setFont(fonts[1]);
    mydisp.print("show font in different direction");
    resetpos1();
    mydisp.clearScreen();
    for (uint8_t i = 0; i < 4; i++) {
        mydisp.setRotation(i);
        mydisp.setFont(fonts[2]);
        mydisp.setPrintPos(0, 0);
        for (uint8_t j = 0; j < 3; j++) {
            mydisp.setFont(fonts[2 - j]);
            if (j != 0)
                mydisp.nextTextLine();
            mydisp.print(fontdir[i]);
        }
    }
    resetpos1();
    mydisp.clearScreen();
    mydisp.setMode('~'); //set graphic Drawing Mode to COPY
    mydisp.drawHLine(0, 31, 127); //draw horizontal LiNe
    mydisp.setPrintPos(0, 31, 1); //Set Graphic position
    for (uint8_t i = 1; i <= 127; i = i + 6) //this loop will draw sin graph
    {
        mydisp.drawLineTo(i, (uint8_t) (32 - (float) (sin(i * 3.14 / 63)*28)));
    }
}
void loop() {
}
