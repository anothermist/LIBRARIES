/*********************
 * http://www.digole.com/forum   communicate with us and other user
 * http://www.digole.com    buyer Digole module
 * Digole Serial display module with touch screen demo
 * This demo will show you how to write data (u8g fonts, command set to onboard
 * flush memory, then use it. Also show you Checkbox and Radio buttons
 * The onboard flash memory is 2M bytes
 * NOTE: if you using UART mode, you need to disconnect the RX from Arduino
 * When you uploading your sketch
 **********************/
#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, 
#define Ver 33 //if the firmware version on the module is not V3.3 and later, remove this line
//#define FLASH_CHIP
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
//IMPORTANT: A 4.7K~20K PULL DOWN RESISTOR NEEDED BETWEEN SI(11) AND GROUND IN ORDER TO OPERATE TOUCH SCREEN
//AND FLASH MEMORY CORRECTLY
DigoleSerialDisp mydisp(8,9,10,11);  //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
//the flowing function can be put in DigoleSerialDisp class
void flashErase(unsigned long int addr,unsigned long int len);
void flashWriteL(unsigned long int addr,unsigned long int len, const unsigned char *data);
void flashWrite(unsigned long int addr,unsigned long int len, const unsigned char *data);
void flashReadStart(unsigned long int addr,unsigned long int len);
void setFlashFont(unsigned long int addr);
void runCommandSet(unsigned long int addr);
void drawAllCheckbox(void);
void readPenClick(void);
void drawAllRadios(void);
const unsigned int checkboxCord[][4]={
  {5,50,90,10  }
  ,{5,80,90,10  }
  ,{5,110,90,10  }
  ,{5,140,90,10  }
  ,{5,170,90,10  }
  ,{5,200,90,10  }
};  //x0,y0,width,height
const unsigned char checkboxText[][20]={
  "Checkbox 1","Checkbox 2","Checkbox 3","Checkbox 4","Checkbox 5","Checkbox 6"};
byte checkboxChecked[]={1,0,1,1,0,0};
char checkboxSize;
const unsigned int radioCord[][4]={
  {150,50,80,10  }
  ,{150,80,80,10  }
  ,{150,110,80,10  }
  ,{150,140,80,10  }
  ,{150,170,80,10  }
  ,{150,200,80,10  }
};  //x0,y0,width,height
const unsigned char radioText[][20]={
  "Radio 1","Radio 2","Radio 3","Radio 4","Radio 5","Radio 6"};
char currentRadio,radioSize;
int penx,peny;
//checkbox checked command set, 15x15pixels 256 color image
//you can convert images to this format by using our online tool: http://www.digole.com/tools/PicturetoC_Hex_converter.php
const unsigned char checked[] PROGMEM="EDIM1\x0f\x0f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xdf\x29\x29\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x4d\x00\x05\xff\xff\xbb\x93\x6e\x4a\x4a\x4a\x4a\x4a\x4a\x4a\x00\x00\x96\xff\xff\x93\x93\xbb\xdf\xdf\xdf\xdf\xdf\xdf\x05\x00\x29\xff\xff\xff\x6e\xbb\xdf\xdf\xdf\xdf\xdf\xdf\x4e\x00\x05\x4e\xff\xff\xff\x4a\xbb\xbb\xbb\xbf\xbf\xbf\x97\x00\x00\x96\x4a\xff\xff\xff\x4a\xbb\x52\x00\x4e\xbb\xbb\x05\x00\x49\xbb\x4a\xff\xff\xff\x4a\x9b\x72\x00\x00\x72\x4e\x00\x05\xbb\x9b\x4a\xff\xff\xff\x4a\x7b\x9b\x29\x00\x00\x00\x00\x72\x9b\x7b\x4a\xff\xff\xff\x4a\x9b\x9b\xbb\x29\x00\x00\x29\x9b\x9b\x9b\x4a\xff\xff\xff\x49\x9b\x9b\x9b\xbb\x25\x05\xbb\x9b\x9b\x9b\x49\xff\xff\xff\x4e\x97\xbf\xbf\xbf\xdf\xdf\xbf\xbf\xbf\x97\x4e\xff\xff\xff\x72\x72\x9b\xbf\xdf\xdf\xdf\xdf\xbf\x9b\x72\x72\xff\xff\xff\xbb\x72\x4d\x49\x49\x49\x49\x49\x49\x4d\x92\xdf\xff\xff\xff\xdf\xff\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xff\xff\xff\xff\xff";
//checkbox unchecked command set, 15x15pixels 256 color image
const unsigned char uncheck[] PROGMEM="EDIM1\x0f\x0f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xbb\x96\x92\x72\x72\x72\x72\x72\x72\x92\x96\xbb\xff\xff\xff\x96\xb6\xdf\xff\xff\xff\xff\xff\xff\xdf\xb6\x96\xff\xff\xff\x72\xdf\xff\xff\xff\xff\xff\xff\xff\xff\xdf\x72\xff\xff\xff\x72\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x72\xff\xff\xff\x72\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x72\xff\xff\xff\x72\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x72\xff\xff\xff\x72\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x72\xff\xff\xff\x72\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x72\xff\xff\xff\x72\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x72\xff\xff\xff\x72\xdb\xff\xff\xff\xff\xff\xff\xff\xff\xdb\x72\xff\xff\xff\x96\x96\xdb\xff\xff\xff\xff\xff\xff\xdb\x96\x96\xff\xff\xff\xbb\x96\x72\x72\x72\x72\x72\x72\x72\x72\x96\xbb\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xFF";
const unsigned char radioChecked[] PROGMEM="EDIM1\x0e\x0e\xff\xff\xff\xdb\xb7\x8f\x6e\x6e\x8f\xb7\xdb\xff\xff\xff\xff\xff\xb7\x93\x97\xbb\xdf\xdf\xbb\x97\x93\xb7\xff\xff\xff\xb7\x73\xbb\xdf\xdf\xdf\xdf\xdf\xdf\xbb\x73\xb7\xff\xdb\x72\xb7\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xb7\x72\xdb\xb7\x93\xbb\xbb\xbb\xbb\xbb\xbb\xbb\xbb\xbb\xbb\x93\xb7\x6e\x97\xbb\xbb\xbb\x77\x05\x05\x77\xbb\xbb\xbb\x97\x6e\x6a\x9b\x9b\xbb\xbb\x05\x00\x00\x05\xbb\xbb\x9b\x9b\x6a\x6a\x77\x7b\x7b\x7b\x05\x00\x00\x05\x7b\x7b\x7b\x77\x6a\x6e\x77\x9b\x9b\x9b\x97\x05\x05\x97\x9b\x9b\x9b\x77\x6e\x96\x72\x9b\x9b\x9b\xbb\xbf\xbf\xbb\x9b\x9b\x9b\x72\x96\xdb\x6e\x97\x9b\xbb\xbb\xbb\xbb\xbb\xbb\x9b\x97\x6e\xdb\xff\xb7\x6e\x97\xbf\xbf\xbf\xbf\xbf\xbf\x97\x6e\xb7\xff\xff\xff\xb6\x6e\x92\xbb\xbf\xbf\xbb\x92\x6e\xb6\xff\xff\xff\xff\xff\xdb\x96\x6e\x49\x49\x6e\x96\xdb\xff\xff\xff\xFF\XFF";
const unsigned char radioUnchecked[] PROGMEM="EDIM1\x0e\x0e\xff\xff\xff\xdf\xbb\xb6\x92\x92\xb6\xbb\xdf\xff\xff\xff\xff\xff\xdb\xb6\xb6\xdb\xff\xff\xdb\xb6\xb6\xdb\xff\xff\xff\xdb\xb6\xdb\xff\xff\xff\xff\xff\xff\xdb\xb6\xdb\xff\xdf\xb6\xdb\xff\xff\xff\xff\xff\xff\xff\xff\xdb\xb6\xdf\xbb\xb6\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xb6\xbb\x96\xdb\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xdb\x96\x92\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x92\x92\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x92\x96\xdb\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xdb\x96\xb6\xb6\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xb6\xb6\xdb\x96\xdb\xff\xff\xff\xff\xff\xff\xff\xff\xdb\x96\xdb\xff\xbb\x96\xdb\xff\xff\xff\xff\xff\xff\xdb\x96\xbb\xff\xff\xff\xbb\x96\xb6\xdb\xff\xff\xdb\xb6\x96\xbb\xff\xff\xff\xff\xff\xdb\xb6\x96\x92\x92\x96\xb6\xdb\xff\xff\xff\xFF\xFF";
#include <sampledata.h>
#define saddr 2060
void setup()
{
  mydisp.begin();
  for(char i=0;i<5;i++)
    mydisp.read1(); //dummy read to clear pending data on display
  mydisp.setBgColor(255);
  mydisp.clearScreen();
  mydisp.setBgColor(0B00001111);  //use current color as background
  mydisp.setColor(1);
  mydisp.setFont(18);
  mydisp.setRot90();
  mydisp.drawStr(0,0,"Checkbox & Radio buttons");
  mydisp.setBgColor(255);  //use current color as background
  mydisp.setFont(0);
  //mydisp.setColor(0);
  //If your flash memory is blank, you need use the following 4 line of code to write command sets to flash
  //*
    mydisp.flashErase(saddr,4096);  //erase length of 4096 bytes for story command sets to drawing checkbox, and radio buttons
    delay(100);
    mydisp.flashWrite(saddr,sizeof(checked), checked);  
    mydisp.flashWrite(saddr+256, sizeof(uncheck), uncheck); 
    mydisp.flashWrite(saddr+512, sizeof(radioChecked), radioChecked);
    mydisp.flashWrite(saddr+768, sizeof(radioUnchecked), radioUnchecked);
    for(char i=0;i<5;i++)
        mydisp.read1(); //dummy read to clear pending data on display
  //*/
   checkboxSize =sizeof(checkboxCord)/(sizeof(checkboxCord[0]));
  drawAllCheckbox();
  currentRadio=0;
  radioSize =sizeof(radioCord)/(sizeof(radioCord[0]));
  drawAllRadios();
}
void loop()
{
  char i;
  //read a pen click on touch screen
   readPenClick();
   mydisp.setPrintPos(230,10,1);
   mydisp.print(penx);
   mydisp.print(',');
   mydisp.print(peny);
   mydisp.print("    ");
   for(i=0;i<checkboxSize;i++)
  {
    if(penx>=checkboxCord[i][0]&&penx<=(checkboxCord[i][0]+checkboxCord[i][2])&&peny<=checkboxCord[i][1]&&peny>=(checkboxCord[i][1]-checkboxCord[i][3]))
      break;
  }
  if(i>=0&&i<checkboxSize)  //one item touched
  {
        mydisp.setPrintPos(checkboxCord[i][0]+85,checkboxCord[i][1]-12,1);
    if(checkboxChecked[i]==1)
    {
      checkboxChecked[i]=0;
      mydisp.runCommandSet(saddr+256);  //show checkbox unchecked
    }
    else
    {
      checkboxChecked[i]=1;
      mydisp.runCommandSet(saddr);  //show checkbox checked
    }
  }
  else
  {
   for(i=0;i<radioSize;i++)
   {
    if(penx>=radioCord[i][0]&&penx<=(radioCord[i][0]+radioCord[i][2])&&peny<=radioCord[i][1]&&peny>=(radioCord[i][1]-radioCord[i][3]))
      break;
   }
   if(i>=0&&i<radioSize)
   {
     if(i!=currentRadio)
     {
        mydisp.setPrintPos(radioCord[currentRadio][0]+61,radioCord[currentRadio][1]-12,1);
        mydisp.runCommandSet(saddr+768);  //show radio unchecked
        mydisp.setPrintPos(radioCord[i][0]+61,radioCord[i][1]-12,1);
        mydisp.runCommandSet(saddr+512);  //show radio checked
        currentRadio=i;
     }
   }
  }
}
void drawAllCheckbox(void)
{
   for(char i=0;i<checkboxSize;i++)
  {
    mydisp.setColor(0);  //256 color format RRRGGGBB
    mydisp.setPrintPos(checkboxCord[i][0],checkboxCord[i][1],1);
    mydisp.print((const char*)checkboxText[i]);
    mydisp.setTextPosOffset(5,-12);
    if(checkboxChecked[i]==1)
      mydisp.runCommandSet(saddr);  //show checkbox checked
    else
      mydisp.runCommandSet(saddr+256);  //show checkbox unchecked
  }
}
void drawAllRadios(void)
{
   for(char i=0;i<radioSize;i++)
  {
    mydisp.setColor(0B01101111);  //256 color format RRRGGGBB
    mydisp.setPrintPos(radioCord[i][0],radioCord[i][1],1);
    mydisp.print((const char*)radioText[i]);
    mydisp.setTextPosOffset(5,-12);  //move x,y position to a relative value
    if(i==currentRadio)
      mydisp.runCommandSet(saddr+512);  //show radio checked
    else
      mydisp.runCommandSet(saddr+768);  //show radio unchecked
  }
}
void readPenClick(void){
//  mydisp.readTouchScreen();
  mydisp.readClick();
  penx=mydisp.read1();
  penx<<=8;
  penx+=mydisp.read1();
  peny=mydisp.read1();
  peny<<=8;
  peny+=mydisp.read1();
}