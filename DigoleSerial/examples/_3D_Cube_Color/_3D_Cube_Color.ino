#define SMALL_SCREEN  //if the screen's height is <128

#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, 

//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 115200); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8,9,10,11);  //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
float tx, nx, p;
float ty, ny, py;
float rot, rotx, roty, rotz, rotxx, rotyy, rotzz, rotxxx, rotyyy, rotzzz;
int i; //0 to 360
int fl, scale; //focal length
int wireframe[12][2];
int wireframeNew[12][2];
#ifdef SMALL_SCREEN
int originx = 64;
int originy = 32; //32

int front_depth = 20;
int back_depth = -20;
#define DIM 20
#else
int originx = 128;
int originy = 128; //32

int front_depth = 40;
int back_depth = -40;
#define DIM 40
#endif
//Store cube vertic
int cube_vertex[8][3] = {
  {-DIM, -DIM, front_depth  },{DIM, -DIM, front_depth  },{DIM, DIM, front_depth  },{-DIM, DIM, front_depth  },{-DIM, -DIM, back_depth  },{DIM, -DIM, back_depth  },{DIM, DIM, back_depth  }
,{-DIM, DIM, back_depth  }};

int fd = 0; //0=orthographic
#define endp 255
void setup(void)
{
  mydisp.begin();
    mydisp.clearScreen();
/*    mydisp.print("3D Cube Animation use XOR Mode");
    delay(2000);
//  while(1)
  {
    mydisp.write(random(256));
  }
*/  mydisp.setColor(255);
//  mydisp.setBgColor();
    mydisp.clearScreen();
//  mydisp.setColor(0);
    mydisp.print("3D Cube Animation use XOR Mode");
  mydisp.write('F');  //disable auto refresh screen
  mydisp.write('S');
  mydisp.write('1');
/*  for(unsigned int x=0;x<=endp;x+=5)
  {  mydisp.drawLine(0,0,x,endp);
  mydisp.drawLine(0,0,endp,x);
  delay(200);
  }
  //while(1);
  mydisp.drawLineTo(200,200);
  mydisp.drawLineTo(0,0);
  delay(2000);
    mydisp.clearScreen();
  mydisp.drawLineTo(80,80);
  //while(1);
  */
  mydisp.setColor(195);
}

void draw_wireframe(void)
{
  mydisp.drawLine(wireframe[0][0], wireframe[0][1], wireframe[1][0], wireframe[1][1]);
  mydisp.drawLine(wireframe[1][0], wireframe[1][1], wireframe[2][0], wireframe[2][1]);
  mydisp.drawLine(wireframe[2][0], wireframe[2][1], wireframe[3][0], wireframe[3][1]);
  mydisp.drawLine(wireframe[3][0], wireframe[3][1], wireframe[0][0], wireframe[0][1]);

  //cross face above
  mydisp.drawLine(wireframe[1][0], wireframe[1][1], wireframe[3][0], wireframe[3][1]);
  mydisp.drawLine(wireframe[0][0], wireframe[0][1], wireframe[2][0], wireframe[2][1]);

  mydisp.drawLine(wireframe[4][0], wireframe[4][1], wireframe[5][0], wireframe[5][1]);
  mydisp.drawLine(wireframe[5][0], wireframe[5][1], wireframe[6][0], wireframe[6][1]);
  mydisp.drawLine(wireframe[6][0], wireframe[6][1], wireframe[7][0], wireframe[7][1]);
  mydisp.drawLine(wireframe[7][0], wireframe[7][1], wireframe[4][0], wireframe[4][1]);

  mydisp.drawLine(wireframe[0][0], wireframe[0][1], wireframe[4][0], wireframe[4][1]);
  mydisp.drawLine(wireframe[1][0], wireframe[1][1], wireframe[5][0], wireframe[5][1]);
  mydisp.drawLine(wireframe[2][0], wireframe[2][1], wireframe[6][0], wireframe[6][1]);
  mydisp.drawLine(wireframe[3][0], wireframe[3][1], wireframe[7][0], wireframe[7][1]);
}

void loop(void)
{
  //picture loop
  for (int angle = 0; angle <= 360; angle = angle + 5) {
//    mydisp.clearScreen();
//    mydisp.print("Digole Demo:V2.9");
      for (int i = 0; i < 8; i++) {

        rot = angle * 0.0174532; //0.0174532 = one degree
        //rotateY
        rotz = cube_vertex[i][2] * cos(rot) - cube_vertex[i][0] * sin(rot);
        rotx = cube_vertex[i][2] * sin(rot) + cube_vertex[i][0] * cos(rot);
        roty = cube_vertex[i][1];
        //rotateX
        rotyy = roty * cos(rot) - rotz * sin(rot);
        rotzz = roty * sin(rot) + rotz * cos(rot);
        rotxx = rotx;
        //rotateZ
        rotxxx = rotxx * cos(rot) - rotyy * sin(rot);
        rotyyy = rotxx * sin(rot) + rotyy * cos(rot);
        rotzzz = rotzz;

        //orthographic projection
        rotxxx = rotxxx + originx;
        rotyyy = rotyyy + originy;

        //store new vertices values for wireframe drawing
        wireframeNew[i][0] = rotxxx;
        wireframeNew[i][1] = rotyyy;
        wireframeNew[i][2] = rotzzz;
      }
mydisp.setMode('^');
      draw_wireframe();
//mydisp.setMode('C');
  for(unsigned char i=0;i<12;i++)
  {
          wireframe[i][0]=wireframeNew[i][0];
          wireframe[i][1]=wireframeNew[i][1];
  }
//      mydisp.setColor(random(254)+1);
      draw_wireframe();
  mydisp.write('F');  //force to refresh screen
  mydisp.write('S');
  mydisp.write('0');
  delay(50);
  }
}


