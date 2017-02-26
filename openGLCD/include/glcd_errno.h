/*
  glcd_errno.h
  Copyright (c) 2011-2016 Bill Perry
  
  vi:ts=4

 ------------------------------------------------------------------------

  This file is part of the openGLCD library project

  openGLCD is free software: you can redistribute it and/or modify
  it under the terms of version 3 of the GNU General Public License
  as published by the Free Software Foundation.
 
  openGLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with openGLCD.  If not, see <http://www.gnu.org/licenses/>.

 -----------------------------------------------------------------------

  This file defines the error code return values
 
 */

#ifndef	GLCD_ERRNO_H
#define GLCD_ERRNO_H


/*
 * Define the error code return values for the openGLCD library
 */

#define GLCD_ENOERR			0	// no error This must be 0. Do not change!

#define GLCD_EBUSY			1	// glcd BUSY wait timeout
#define GLCD_ERESET			2	// glcd RESET wait timeout
#define GLCD_EINVAL			3	// invalid argument
#define GLCD_ENOTSUPPORTED	4	// not supported



#endif // GLCD_ERRNO_H
