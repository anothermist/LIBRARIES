/*********************************************************************
**  Device: AD9850/9851                                             **
**  File:   EF_AD9850.h					                                    **
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

#ifndef __EF_AD9850_H__
#define __EF_AD9850_H__

#include "WProgram.h"
#include <avr/pgmspace.h>


class EF_AD9850{
	public:
		 EF_AD9850(int D_CLK, int D_FQUP, int D_REST, int D_BitData);
     void init(void);
     void reset(void);
     void wr_serial(unsigned char w0,double frequence);
     void wr_parrel(unsigned char w0,double frequence);

   private:
     int BitData, FQUP, REST, Mode, CLK;
	
};

#endif
