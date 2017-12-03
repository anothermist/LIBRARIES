#include "SSD1306_minimal.h" // SCL PB2 || SDA PB0
#include <avr/pgmspace.h>

SSD1306_Mini oled;

void heartBeat(){

    digitalWrite(4, HIGH);   // set the LED on  
    delay(200);  
    digitalWrite(4, LOW);    // set the LED off
    delay(100);

    digitalWrite(4, HIGH);   // set the LED on  
    delay(500);  
    digitalWrite(4, LOW);    // set the LED off
    delay(100);
  
}

/*
    display 128x64
    snake size 4x4
   
    128 / 8 => 16cols
     64 / 8 => 8rows
    
*/

#define ColCount 32
#define RowCount 16 

// there are different wall types
unsigned char wall[5][4]= { 
  0x0, 0x0, 0x0, 0x0,
  0xf, 0xf, 0xf, 0xf,
  0xf, 0x9, 0x9, 0xf,
  0x9, 0x9, 0x9, 0x9,
  0x9, 0x6, 0x6, 0x9,
};
// the ball shape
unsigned char ball[4]= { 0x6, 0x9, 0x9, 0x6 };

static unsigned char snakeRow= 10;
static unsigned char snakeCol= 7;  
static char snakeRowDir= +1;
static char snakeColDir= -1;
static char wallIdx=1;

// this is the room shape
const static uint8_t room[] PROGMEM ={
1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,2,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,2,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,2,0,0,0,0,2 , 2,2,2,0,0,0,0,1,
1,0,0,0,2,0,0,0 , 0,0,2,2,2,0,0,0 , 0,0,2,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,2,0,0,0 , 0,0,0,0,2,0,0,0 , 0,0,2,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,2,0,0,0 , 0,0,0,0,2,2,2,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,2,2,2,2,0,0,0 , 0,2,0,0,0,0,0,2 , 2,2,2,2,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,2,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1 , 1,1,1,1,1,1,1,1,
};


unsigned char getRoom( unsigned char row, unsigned char col  ){

  unsigned char data= pgm_read_byte( &(room[row*ColCount + col]) );
  
  return data;
}

unsigned char getSnake( unsigned char row, unsigned char col ){
  
  
  unsigned char data=0;
  
  if ((row == snakeRow) && (col == snakeCol)){
    data=2;    
  }
  
  return data;  
}

  
unsigned char hitRoom( unsigned char row, unsigned char col ){

  uint8_t data;
  data= getRoom( row, col );
 
  return data;
}
  
void displayRoom(){

  oled.startScreen();

  uint8_t upperRow;
  uint8_t lowerRow;

  uint8_t upperSnake;
  uint8_t lowerSnake;
  
  uint8_t data[4];
  

  int i=0;
  for (char r=0;r<RowCount; r=r+2 ){
    for (char c=0;c<ColCount; c++){
        
          // send a bunch of data in one xmission
          Wire.beginTransmission(SlaveAddress);
          Wire.send(GOFi2cOLED_Data_Mode);            // data mode

            upperRow= getRoom(r, c);
            lowerRow= getRoom(r+1, c);
            
            upperSnake= getSnake(r, c);
            lowerSnake= getSnake(r+1, c);

            //data= 0x0;
               data[0]= 0x0;
               data[1]= 0x0;
               data[2]= 0x0;
               data[3]= 0x0;
            
            // room
            if (upperRow){
               data[0]|= wall[upperRow][0] << 0;
               data[1]|= wall[upperRow][1] << 0;
               data[2]|= wall[upperRow][2] << 0;
               data[3]|= wall[upperRow][3] << 0;
            }
            
            if (lowerRow){
               data[0]|= wall[lowerRow][0] << 4;
               data[1]|= wall[lowerRow][1] << 4;
               data[2]|= wall[lowerRow][2] << 4;
               data[3]|= wall[lowerRow][3] << 4;
            }
            
             //data|= (upperRow?0x0F:0x00) | (lowerRow?0xF0:0);            
            // snake
            if (upperSnake){
               data[0]|= ball[0] << 0;
               data[1]|= ball[1] << 0;
               data[2]|= ball[2] << 0;
               data[3]|= ball[3] << 0;
            }
            if (lowerSnake){
               data[0]|= ball[0] << 4;
               data[1]|= ball[1] << 4;
               data[2]|= ball[2] << 4;
               data[3]|= ball[3] << 4;
            }
            //data|= (upperSnake?0x0A:0x00) | (lowerSnake?0xA0:0);            
            
            
            Wire.send( data[0] );
            Wire.send( data[1] );
            Wire.send( data[2] );
            Wire.send( data[3] );
          Wire.endTransmission();
          
    }
  }  
}

void changeWallIdx(){
      wallIdx++;
      if (wallIdx>1){
        wallIdx=0;
      }
}

void die( unsigned char r, unsigned char c ){

  Wire.beginTransmission(SlaveAddress);
    Wire.send(GOFi2cOLED_Data_Mode);            // data mode

    Wire.send( 0xFF );
    Wire.send( 0x81 );
    Wire.send( 0x81 );
    Wire.send( 0xFF );
  Wire.endTransmission();
}

void snake(){
  
  oled.cursorTo(3,63);

  die( 0, 0);
  
}

void move(){
  
  char bHitRoom;
  char hitType;
  
  do {  
    bHitRoom= 0;
    // test row
    hitType= hitRoom( snakeRow + snakeRowDir, snakeCol);
    if (hitType != 0){
      snakeRowDir= -1* snakeRowDir;
      bHitRoom= hitType;
    }
    // test col
    hitType= hitRoom( snakeRow, snakeCol + snakeColDir);
    if (hitType != 0){
      snakeColDir= -1* snakeColDir;
      bHitRoom= hitType;
    }
    //
    if (bHitRoom==1){
      //nur an der wand ändern
      //changeWallIdx();
    }
    
  } while (bHitRoom);
  
  snakeRow= snakeRow + snakeRowDir;
  snakeCol= snakeCol + snakeColDir;
  
  
}


void setup()
{
  pinMode(4, OUTPUT);   
  
    heartBeat();

    oled.init(0x3c);
    oled.clear();
    
    for (int k=0;k<100;k++){
    for (int i=0;i<1000;i++){
      displayRoom();
      
      move();
     
      delay(5); 
    }
      
    }
    
   
    delay(4000);
    
/*
    for (int i=0;i<10000;i++){
    for (int k=0;k<10000;k++){
      oled.displayX(k);
    }
    }
    
    */
}
 
void loop()
{
 
} 
