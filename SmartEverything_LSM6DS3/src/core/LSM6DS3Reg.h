 /*
 * LSM6DS3Reg.h
 *
 *  Created on: 29/09/2016 23:02:00
 * by Zero-2 (Zero2sat@gmail.com)
 * 
 * License Information
 * -------------------
 *
 * Copyright (c) Axel Elettronica Srl. All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */


#ifndef LSM6DS3_REG_H_
#define LSM6DS3_REG_H_

/***************** COMMON VALUES ******************/

/*
* Sensor address. Value is common for both accelerometer and gyroscope
*/
#define LSM6DS3_ADDRESS				0x6A 

/*******************************
* Register      : WHO_AM_I
* Address       : 0X1F
* Description	: Who_AM_I register. Its value is fixed at 69h.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    0    |    1    |    1    |    0    |    1    |    0    |    0    |    1    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*
*******************************/
#define WHO_AM_I_REG_ADDRESS		0x0F
#define WHO_AM_I_VALUE				0x69 //Fixed registry value

/*******************************
* Register      : STATUS_REG
* Address       : 0X1E
* Description	: Status Register
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    -    |    -    |    -    |    -    | EV_BOOT |   TDA   |   GDA   |  XLDA   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*
*******************************/
#define STATUS_REG_ADDRESS			0x1E //GDA & XLDA

/******************************************************
****************** ACCELEROMETER **********************
******************************************************/

#define XL_OM_RESET_MASK 0xEF;
#define XL_ODR_RESET_MASK 0x0F;
#define XL_FS_RESET_MASK 0xF3;
#define XL_BW_RESET_MASK 0xFC;

/*******************************
* Register      : CTRL1_XL
* Address       : 0X10
* Description	: Linear acceleration sensor control register 1
* Permission    : RW
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*| ODR_XL3 | ODR_XL2 | ODR_XL1 | ODR_XL0 | FS_XL1  | FS_XL0  | BW_XL1  |  BW_XL0 |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*
*******************************/
#define CTRL1_XL_REG_ADDRESS		0x10

/*******************************
* Register      : CTRL6_C
* Address       : 0X15
* Description	: Angular rate sensor control register 6
* Permission    : RW
* Bits
*+---------+---------+---------+------------+---------+---------+---------+---------+
*|    7    |    6    |    5    |      4     |    3    |    2    |    1    |    0    |
*+---------+---------+---------+------------+---------+---------+---------+---------+
*| TRIG_EN |  LVLen  | LVL2_EN | XL_HM_MODE |    0    |    0    |    0    |    0    |
*+---------+---------+---------+------------+---------+---------+---------+---------+
*
*******************************/
#define CTRL6_C_REG_ADDRESS			0x15

/*******************************
* Register      : CTRL4_C
* Address       : 0X13
* Description	: Control register 4
* Permission    : RW
* Bits
*+---------+---------+---------+---------+---------+-------------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |      2		 |    1    |    0    |
*+---------+---------+---------+---------+---------+-------------+---------+---------+
*| XL_BW_  | SLEEP_G | INT2_on_|  FIFO_  |  DRDY_  | I2C_disable |    0    | STOP_ON |
*|SCAL_ODR |		 |	INT1   | TEMP_EN |	MASK   |			 |		   |   FTH   |
*+---------+---------+---------+---------+---------+-------------+---------+---------+
*
*******************************/
#define CTRL4_C_REG_ADDRESS         0x13 

//X-Axis value
/*******************************
* Register      : OUTX_L_XL
* Address       : 0X28
* Description	: LSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    D7   |    D6   |    D5   |    D4   |    D3   |    D2   |    D1   |    D0   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTX_L_XL					0x28

/*******************************
* Register      : OUTX_H_XL
* Address       : 0X29
* Description	: MSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|   D15   |   D14   |   D13   |   D12   |   D11   |   D10   |    D9   |    D8   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTX_H_XL					0x29
//Y-Axis Value
/*******************************
* Register      : OUTY_L_XL
* Address       : 0X2A
* Description	: LSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    D7   |    D6   |    D5   |    D4   |    D3   |    D2   |    D1   |    D0   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTY_L_XL					0x2A

/*******************************
* Register      : OUTY_H_XL
* Address       : 0X2B
* Description	: MSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|   D15   |   D14   |   D13   |   D12   |   D11   |   D10   |    D9   |    D8   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTY_H_XL					0x2B

//Z-Axis Value
/*******************************
* Register      : OUTZ_L_XL
* Address       : 0X2C
* Description	: LSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    D7   |    D6   |    D5   |    D4   |    D3   |    D2   |    D1   |    D0   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTZ_L_XL					0x2C

/*******************************
* Register      : OUTZ_H_XL
* Address       : 0X2D
* Description	: MSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|   D15   |   D14   |   D13   |   D12   |   D11   |   D10   |    D9   |    D8   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTZ_H_XL					0x2D

/*******************************
* Register      : CTRL6_C
* Bit Group Name: XL_HM_MODE
*******************************/
typedef enum{
	XL_HM_HIGH_PERFORMANCE		=0x00,  //Default value
	XL_HM_NORMAL_MODE			=0x10
}XL_HM_MODE;  

/*******************************
* Register      : STATUS_REG
* Bit Group Name: XLDA
*******************************/
typedef enum{
	XL_DISABLED		=0x00,
	XL_ENABLED		=0x01
}XL_STATUS_BIT;

/*******************************
* Register      : CTRL6_C
* Bit Group Name: XL_BW_SCAL_ODR
*******************************/
typedef enum{
	XL_BW_SCAL_ODR_AUTOMATIC	=0x00,
	XL_BW_SCAL_ODR_MANUAL		=0x80 //Bandwidth determined by setting BW_XL[1:0] in CTRL1_XL (10h) register.)
}XL_BW_SCAL_ODR;


/******************************************************
******************** GYROSCOPE ************************
******************************************************/

#define G_OM_RESET_MASK 0x7F;
#define G_ODR_RESET_MASK 0x0F;
#define G_FS_RESET_MASK 0xF3;

/*******************************
* Register      : CTRL2_G
* Address       : 0X11
* Description	: Angular rate sensor control register 2
* Permission    : RW
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*| ODR_G3  |  ODR_G2 | ODR_G1  | ODR_G0  |  FS_G1  |  FS_G0  |  FS_125 |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*
*******************************/
#define CTRL2_G_REG_ADDRESS			0x11

/*******************************
* Register      : CTRL7_G
* Address       : 0X16
* Description	: Angular rate sensor control register 7
* Permission    : RW
* Bits
*+---------+---------+---------+---------+---------+-----------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |     2	   |    1    |    0    |
*+---------+---------+---------+---------+---------+-----------+---------+---------+
*|  G_HM_  | HP_G_EN | HPCF_G1 | HPCF_G0 |  HP_G_R | ROUNDING_ |    0    |	  0    |
*|  MODE   |		 |		   |		 |	  ST   |  STATUS   |	     |         |
*+---------+---------+---------+---------+---------+-----------+---------+---------+
*
*******************************/
#define CTRL7_G_REG_ADDRESS			0x16

//X value
/*******************************
* Register      : OUTX_L_G
* Address       : 0X22
* Description	: LSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    D7   |    D6   |    D5   |    D4   |    D3   |    D2   |    D1   |    D0   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTX_L_G					0x22

/*******************************
* Register      : OUTX_H_G
* Address       : 0X22
* Description	: MSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|   D15   |   D14   |   D13   |   D12   |   D11   |   D10   |    D9   |    D8   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTX_H_G					0x23

//Y Value
/*******************************
* Register      : OUTY_L_G
* Address       : 0X24
* Description	: LSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    D7   |    D6   |    D5   |    D4   |    D3   |    D2   |    D1   |    D0   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTY_L_G					0x24

/*******************************
* Register      : OUTY_H_G
* Address       : 0X25
* Description	: MSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|   D15   |   D14   |   D13   |   D12   |   D11   |   D10   |    D9   |    D8   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTY_H_G					0x25

//Z Value
/*******************************
* Register      : OUTZ_L_G
* Address       : 0X26
* Description	: LSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    D7   |    D6   |    D5   |    D4   |    D3   |    D2   |    D1   |    D0   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTZ_L_G					0x26

/*******************************
* Register      : OUTZ_H_G
* Address       : 0X27
* Description	: MSB. Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
* Permission    : R
* Bits
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*|   D15   |   D14   |   D13   |   D12   |   D11   |   D10   |    D9   |    D8   |
*+---------+---------+---------+---------+---------+---------+---------+---------+
*/
#define OUTZ_H_G					0x27

/*******************************
* Register      : CTRL7_G
* Bit Group Name: G_HM_MODE
*******************************/
typedef enum{
	G_HM_HIGH_PERFORMANCE		=0x00,  //Default value
	G_HM_NORMAL_MODE			=0x80
}G_HM_MODE;

/*******************************
* Register      : STATUS_REG
* Bit Group Name: GDA
*******************************/
typedef enum{
	G_DISABLED		=0x00,
	G_ENABLED		=0x02
}G_STATUS_BIT;


#endif /* INCFILE1_H_ */
