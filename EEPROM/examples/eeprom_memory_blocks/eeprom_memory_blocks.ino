/***
    eeprom_memory_blocks example.
    This example sketch shows how to use the EEPROM library
    to read and write arbitrary blocks of data.

    To write contiguous arrays or objects, see the examples:
    eeprom_put, eeprom_get, and eeprom_arrays.

    Written by Christopher Andrews 2015
    Released under MIT licence.
***/

#include <EEPROM.h>

void setup() {

  //Initial setup (only relevant to example, not the EEPROM library in general).
  delay( 2000 );  //Prevent touching the EEPROM through resets and uploading of sketch.
  Serial.begin( 9600 );

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo boards only.
  }

  /***

    Manipulating memory blocks using the EEPROM library.

    The functions EEPROM.get() and EEPROM.put() can read and write whole arrays and objects
    easily using two parameters (eeprom_put, eeprom_get, eeprom_arrays examples).

    This section shows how to read and write parts of an array or arbitrary memory block using
    the overloads of EEPROM.get() and EEPROM.put() taking 3 parameters.

    EEPROM.get( address, pointer, count );
    EEPROM.put( address, pointer, count );

    address: Location of where to access the EEPROM.
    pointer: Location in RAM to access.
    count:   Count of elements to write, take care to note that this is NOT a count of bytes, 
             but the number of objects/elements to write. The size of an element is determined 
             by the type of pointer passed to 'pointer'. To write out a count of bytes, simply   
             cast your pointer to a 'char*' or 'byte*':  
             EEPROM.put( address, (char*) pointer, count );
  ***/

  int address = 100; //An address/offset to the EEPROM cell we want to begin reading or writing.

  /***
    Example 1: Using a c-string (char array), put only the word 'simple' in the EEPROM.

    Tip: The name of an array used in code without a subscript '[]' will
         automatically convert to a pointer which points at the first
         element in the array. Pointer arithmetic can be used to relocate
         the pointed too address.
  ***/

  char text[] = "A simple string.";

  Serial.print( "Contents of array 'text': " );
  Serial.println( text );

  EEPROM.put( address, text + 2, 6 ); //  The word 'simple' starts at index 2.
                                      //  The length of the word 'simple' is 6 chars.
                                      //  In code you could deduce these using something like strpos().
                                      //  Note: no NULL character has been written

  /***
    The data has now been placed in to the EEPROM.
    
    To show how the three parameter version of get can allow us to read part of an array, 
    we will re-use the array 'text' used above. To prove the data is actually loaded, 
    memset is used to clear the contents of the array (and subsequently ensures our 
    string read from the EEPROM is null terminated in the array).
  ***/

  memset( text, 0, sizeof(text) ); //  Clear the array contents.

  EEPROM.get( address, text, 6 );  // Read back the word 'simple' to the start of the array 'text'.
                                   // Without the third parameter, enough cells would be read to fill
                                   // the entire array, however we only stored 6 characters, so that 
                                   // is all we shall read.

  Serial.print( "Contents of array 'text': " );
  Serial.println( text );

  
  /***
    Example 2: Storage of pointers or dynamic memory.
    
      When using dynamic memory only a pointer is provided as a way to access the data.
      Therefore put() and get() need the third parameter as an arrays length cannot be
      deduced from a pointer alone.
  ***/
  
  address += 6; //Move address to the end of the previous test (prevent re-using the same cells).
  
  const int itmCount = 10;

  int *dynamicData = new int[ itmCount ];

  // Fill array with some data and print each element.
  for( int index = 0 ; index < itmCount ;  ++index ){
  
    dynamicData[ index ] = random(INT_MAX);
    
    Serial.print( "dynamicData[" );
    Serial.print( index );
    Serial.print( "] = " );
    Serial.println( dynamicData[ index ] );
  }
  
  // Write array to EEPROM.
  EEPROM.put( address, dynamicData, itmCount );
  
  // Somewhere to place the data retrieved from the EEPROM.
  int *resultData = (int*) malloc( itemCount * sizeof( int ) );
  
  EEPROM.get( address, resultData, itmCount );
  
  for( int index = 0 ; index < itmCount ;  ++index ){
    Serial.print( "resultData[" );
    Serial.print( index );
    Serial.print( "] = " );
    Serial.println( resultData[ index ] );
  }  

  // Clean up unused dynamic memory allocations.
  delete [] dynamicData;
  free( resultData );


  /***
    Example 3: Using String data.
    
    Strings provide a method named: c_str(). This function returns a pointer to
    the internal string buffer, which is essentially just a null-terminated C
    style string, or simply c-string.
    
    By using this feature and passing the returned pointer to the EEPROM.put()
    method, just like the example above, we can write a String object in a single
    call.

    Tip: Strings use dynamic memory. RAM can fill quickly when many Strings 
    are manipulated at once. Avoid global Strings as they can prevent 
    fragmented memory from being recovered. A fragment may occur on a simple
    concatenation of two Strings, which is not fixed until the allocation 
    storing the result of the concatenation is released.
  ***/

  address += itmCount * sizeof( int ); //Move address to the end of the previous test (prevent re-using the same cells).

  /** Store a String in the EEPROM **/
  String str = "A test string, stored in a String object!";
  
  EEPROM.put( address, str.c_str(), str.length() );

  Serial.print( "Contents of String 'str': " );
  Serial.println( str );

  /*** 
    Retrieve the String data from the EEPROM. 
    
    String's also provide a function named reserve(), which allows us to pre-buffer
    some space in the String.    
  ***/
  
  String result;
  int len = str.length();
  
  result.reserve( len ); //Buffer enough space for the String data.
  
  EEPROM.get( address, result.c_str(), len );  //'get' the data directly into the String buffer.
  
  result.validate(); // Allow the String to detect the changed data.

  Serial.print( "Contents of String 'result': " );
  Serial.println( result );
}

void loop() {
  //Empty loop
}