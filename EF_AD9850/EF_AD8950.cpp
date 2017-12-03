/*********************************************************************
**  Device: AD9850/9851                                             **
**  File:   EF_AD9850.cpp				                                    **
**								    																							**
**  Created by ElecFreaks Robi.W /28 Oct 2011                       **
**                                                                  **
**  Description:                                                    **
**  This file is a sample code for your reference.About AD9850/9851 **
**  module use on Arduino with serial mode. And then show the       **
**  square wave and sine wave by LXARDOSCOPE which is an free       **
**  Arduino based oscilloscope Linux, using the Xforms library.     **
**                                                                  **
**  This demo code is free software; you can redistribute it and/or **
**  modify it under the terms of the GNU Lesser General Public	    **
**  License as published by the Free Software Foundation; either    **	
**  version 2.1 of the License, or (at your option)   		    			**
**  any later version.						    															**
**                                                                  **
**  Copyright (C) 2011 ElecFreaks Corp.                     	    	**
**                                                                  **
**  http://www.elecfreaks.com                                       **
*********************************************************************/

#include "EF_AD9850.h"


EF_AD9850::EF_AD9850(int D_CLK, int D_FQUP, int D_REST, int D_BitData)
	{
		pinMode(D_REST, OUTPUT);
   	pinMode(D_FQUP, OUTPUT);
   	pinMode(D_CLK , OUTPUT);
   	pinMode(D_BitData, OUTPUT);
   	
   	
   	CLK = D_CLK;
   	FQUP= D_FQUP;
   	REST= D_REST;
   	BitData = D_BitData;
	}


void EF_AD9850::init(void)
	{
		digitalWrite(REST, 0);
   	digitalWrite(FQUP, 0);
   	digitalWrite(CLK, 0);
   	digitalWrite(BitData, 0);
	}

void EF_AD9850::reset(void)
	{
		digitalWrite(CLK, 0);
	  digitalWrite(FQUP, 0);
	  //Reset signal
	  digitalWrite(REST, 0);
	  digitalWrite(REST, 1);
	  digitalWrite(REST, 0);
	  //Clk  signal
	  digitalWrite(CLK, 0);
	  digitalWrite(CLK, 1);
	  digitalWrite(CLK, 0);
	  //Fq-up signal
	  digitalWrite(FQUP, 0);
	  digitalWrite(FQUP, 1);
	  digitalWrite(FQUP, 0);
		
	}
	
void EF_AD9850::wr_serial(unsigned char w0,double frequence)
	{
		unsigned char i,w;
	  long int y;
	  double x;
	  
	  //Calculate the frequency of the HEX value
	  x=4294967295/125;//Suitable for 125M Crystal 
	  frequence=frequence/1000000;
	  frequence=frequence*x;
	  y=frequence;
	  
	  //write w4
	  w=(y>>=0);
	  for(i=0; i<8; i++)
	  {
	    digitalWrite(BitData, (w>>i)&0x01);
	    digitalWrite(CLK, 1);
	    digitalWrite(CLK, 0);
	  }
	  //write w3
	  w=(y>>8);
	  for(i=0; i<8; i++)
	  {
	    digitalWrite(BitData, (w>>i)&0x01);
	    digitalWrite(CLK, 1);
	    digitalWrite(CLK, 0);
	  }
	  //write w2
	  w=(y>>16);
	  for(i=0; i<8; i++)
	  {
	    digitalWrite(BitData, (w>>i)&0x01);
	    digitalWrite(CLK, 1);
	    digitalWrite(CLK, 0);
	  }
	  //write w1
	  w=(y>>24);
	  for(i=0; i<8; i++)
	  {
	    digitalWrite(BitData, (w>>i)&0x01);
	    digitalWrite(CLK, 1);
	    digitalWrite(CLK, 0);
	  }
	  //write w0
	  w=w0;
	  for(i=0; i<8; i++)
	  {
	    digitalWrite(BitData, (w>>i)&0x01);
	    digitalWrite(CLK, 1);
	    digitalWrite(CLK, 0);
	  }
	  digitalWrite(FQUP, 1);
	  digitalWrite(FQUP, 0);
	}	

void EF_AD9850::wr_parrel(unsigned char w0,double frequence)
	{
		
	}
	
	













