/***
    eeprom_arrays example.
    This example sketch shows how to use the EEPROM library
    to read and write arrays of one or more dimensions.
	
	For more fine control over what is written, see the example:
	eeprom_memory_blocks
    
    Written by Christopher Andrews 2015
    Released under MIT licence.
***/

#include <EEPROM.h>

void setup() {

  Serial.begin( 9600 );
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo boards only.
  }
  
  /***
  
    Read and write a simple 1D array.
  
  ***/
  
  int address = 0; //Address to first EEPROM cell.
  
  char text[] = "Testing of a c-string (char array) using put() and get().";
  
  EEPROM.put( address, text );  //Write array data.
  
  memset( text, 0, sizeof(text) ); //Clear testing array.
  
  EEPROM.get( address, text );  //Read back array data.
  
  Serial.print( "char array retrieved from EEPROM: " );
  Serial.println( text );
  
  /***
  
    Read and write a multi-dimensional array.
  
  ***/
  
  long numbers[3][4] = {
    { 0xA00, 0xA01, 0xA02, 0xA03 },
    { 0xB00, 0xB01, 0xB02, 0xB03 },
    { 0xC00, 0xC01, 0xC02, 0xC03 }
  };
  
  address += sizeof( text ); //Move to end of array 'text'.
  
  EEPROM.put( address, numbers );  //Write array data.
  
  memset( numbers, 0, sizeof(numbers) ); //Clear testing array.
  
  EEPROM.get( address, numbers );  //Read back array data.
  
  Serial.println( "\n\nArray of long values retrieved from EEPROM: " );
  Serial.println( '{' );
  for( int i = 0 ; i < 3 ; ++i ){
    
    Serial.print( "    {" );
    
    for( int j = 0 ; j < 4 ; ++j ){
      Serial.print( numbers[ i ][ j ], HEX );
      Serial.print( ',' ); 
    }
    Serial.println( '}' );   
  }
  Serial.println( '}' );
  
  /***
  
    Read a single dimension out of a multi-dimensional array.
  
  ***/  
  
  long set[4] = {}; //Enough space for a single dimension.
  
  int offset = address + sizeof( numbers[0] ); // Offset the address by the size of one dimension in 'numbers'. 
  
  EEPROM.get( offset, set );  //Read second dimension ( index: 1 )
  
  Serial.print( "\n\nSingle dimension retrieved from EEPROM:\n    {" );
  for( int i = 0 ; i < 4 ; ++i ){
    Serial.print( set[ i ], HEX );
    Serial.print( ',' ); 
  }  
  Serial.print( '}' );
}

void loop(){
  //Empty loop
}