/*********************************************************************
**  Device: AD9850/9851                                             **
**  File:   EF_AD9850.cpp	v1.2			            **
**								    **
**  Created by ElecFreaks Robi.W /28 Oct 2011                       **
**                                                                  **
**  Description:                                                    **
**  This file is a sample code for your reference.About AD9850/9851 **
**  module use on Arduino with serial mode. And then show the       **
**  square wave and sine wave by LXARDOSCOPE which is an free       **
**  Arduino based oscilloscope Linux, using the Xforms library.     **
**  Note:This library just Serial mode for AD9850                   **
**                                                                  **
**  This demo code is free software; you can redistribute it and/or **
**  modify it under the terms of the GNU Lesser General Public	    **
**  License as published by the Free Software Foundation; either    **	
**  version 2.1 of the License, or (at your option)   		    **
**  any later version.						    **
**                                                                  **
**  Copyright (C) 2011 ElecFreaks Corp.                     	    **
**                                                                  **
**  http://www.elecfreaks.com                                       **
*********************************************************************/

#include <EF_AD9850.h>

//Define for LXARDOSCOPE
int sensorValue = 0;        // value read from the pot
byte lb;
byte hb;

//BitData - D8, CLK - D9, FQUP - D10, REST - D11
EF_AD9850 AD9850(9, 10, 11, 8);

void setup()
{
  AD9850.init();
  AD9850.reset();
  AD9850.wr_serial(0x00, 200); //200Hz
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
}

void loop(){
  // read A0:
  sensorValue = analogRead(A0);            
// shift sample by 3 bits, and select higher byte  
  hb=highByte(sensorValue<<3); 
// set 3 most significant bits and send out
  Serial.print(hb|224,BYTE); 
// select lower byte and clear 3 most significant bits
  lb=(lowByte(sensorValue))&31;
// set bits 5 and 6 and send out
  Serial.print(lb|96,BYTE);
// read A1
  sensorValue = analogRead(A1);            
// shift sample by 3 bits, and select higher byte 
  hb=highByte(sensorValue<<3); 
// set bits 5 and 6 and send out
  Serial.print(hb|96,BYTE); 
// select lower byte and clear 3 most significant bits
  lb=(lowByte(sensorValue))&31;
// set bits 5 and 6 and send out
  Serial.print(lb|96,BYTE);
}
