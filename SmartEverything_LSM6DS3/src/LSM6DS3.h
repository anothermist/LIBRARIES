 /*
 * LSM6DS3.h
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

#ifndef LSM6DS3_SENSOR_H_
#define LSM6DS3_SENSOR_H_

#include <Arduino.h>
#include <stdbool.h>
#include <stdint-gcc.h>

#define CHECK_STATUS_RETRIES 10

/*

Output data rate values.
By default operating mode is in High Performance
For the accelerometer all the values are valid.
For the gyroscope the only valid values are in the range 12.5Hz-1.66Khz

+------------------------------+--------------------------------+
|		 A/XL_HM_MODE = 1      |	A/XL_HM_MODE = 0 (default)	|
+------------------------------+--------------------------------+
|  12.5 Hz (low power)         |   12.5 Hz (high performance)   |
|  26 Hz (low power)           |   26 Hz (high performance)     |
|  52 Hz (low power)		   |   52 Hz (high performance)     |
|  104 Hz (normal mode)		   |   104 Hz (high performance)    |
|  208 Hz (normal mode)		   |   208 Hz (high performance)    |
|  416 Hz (high performance)   |   416 Hz (high performance)    |
|  833 Hz (high performance)   |   833 Hz (high performance)    |
|  1.66 kHz (high performance) |   1.66 kHz (high performance)  |
|  3.33 kHz (high performance) |   3.33 kHz (high performance)  |
|  6.66 kHz (high performance) |   6.66 kHz (high performance)  |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
enum OutputDataRate {
	POWER_12_5_HZ=	0x10,
	POWER_26_HZ=	0x20,
	POWER_52_HZ=	0x30,
	POWER_104_HZ=	0x40,
	POWER_208_HZ=	0x50,
	POWER_416_HZ=	0x60,
	POWER_833_HZ=	0x70,
	POWER_1_66_KHZ=	0x80,
	POWER_3_33_KHZ=	0x90,
	POWER_6_66_KHZ=	0xA0
};

struct OperatingMode{
	byte bits;
};

//Sensitivity Struct
struct FullScale{
	byte bits; //Registry value
	float full_scale_value; //Sensitivity value
	int range;
};

//Bandwidth struct
struct Bandwidth{
	byte bits; //Registry value
	int value; //Bandwidth value
};

struct Axis{
	byte lowRegistry;
	byte highRegistry;
};

struct AxisRegistry{
	Axis XAxis;
	Axis YAxis;
	Axis ZAxis;
};

struct SensorSettings{
	byte			power_mode_registry;
	byte			status_sensor_value; //Value of the Status Registry for the specific sensor
	byte			sensor_control_registry; //Address of the Status Registry for the specific sensor
	FullScale		fullScale;
	AxisRegistry	axisRegistry;
	byte			output_data_rate_reset_mask;
	byte			operating_mode_reset_mask;
	byte			full_scale_reset_mask;
	byte			bandwidth_reset_mask;
};

//Base class with common operations for both accelerometer and gyroscope sensor
class LSM6DS3
{
	public:
		LSM6DS3();
		~LSM6DS3() 
		{
			if(_isActive)
			{
				powerOff();	
			}
		};

		/** Concrete methods **/
		//Check if sensor is active or not
		boolean isActive(){return _isActive;};
		//Move the sensor to power on state
		boolean powerOn(OutputDataRate power_value=POWER_104_HZ);
		//Move the sensor to power down state
		boolean powerOff();
		//Change sensor power value. This command, if sensor previously was active, power off the sensor, and then reactivate it 
		boolean changeOutputDataRate(OutputDataRate powerValue);
		//Change the sensor sensitivity
		boolean changeFullScale(FullScale sensitivity_bits);
		//Change the power mode (Normal or High Performance)
		boolean changeOperatingMode(OperatingMode powerMode);
		//Return the converted X-Axis value
		float getConvertedXAxis();
		//Return the converted Y-Axis value
		float getConvertedYAxis();
		//Return the converted Z-Axis value
		float getConvertedZAxis();
		//Return the raw X-Axis value
		int getRawXAxis();
		//Return the raw Y-Axis value
		int getRawYAxis();
		//Return the raw Z-Axis value
		int getRawZAxis();	
		//Initialize the sensor
		void begin();
		//Reset registry values to default
		void resetToDefault();
		
	protected:
		//Variables
		boolean			_isActive; //True if specific sensor is active , False if is in power down
		SensorSettings	_sensorSettings;
		//Methods
		//Virtual Methods
		//Check if power value is valid or not for the specific sensor
		virtual boolean isValidOutputDataRate(OutputDataRate power_value)=0;
		//Check if sensitivity is valid or not for the specific sensor
		virtual boolean isValidFullScale(FullScale sensitivity)=0;
		//Check if power mode is valid or not for the specific sensor
		virtual boolean isValidOperatingMode(OperatingMode powerMode)=0;
		//Convert raw value to sensor specific value
		virtual float convertAxisValue(int value)=0;
		//Concrete methods
		//Read value from registry
		byte readRegister( byte regToRead);
		//Write value to registry
		boolean writeRegister(byte regToWrite, byte dataToWrite);
		//Read 16bits value from two 8 bits registry
		boolean readCombinedRegistry(unsigned char lsb_registry,unsigned char msb_registry,int* value);
		void changeRegistryValue(byte registry,byte resetMask,byte value);
		virtual FullScale readFullScaleFromRegistry()=0;

};

//Accelerometer class with specific sensor implementation
class LSM6DS3_Accelerometer: public LSM6DS3{
	public:
		LSM6DS3_Accelerometer();
		//Change the bandwidth value
		boolean changeBandwidth(Bandwidth bandwidth);
		String getStatus();
		void resetToDefault();
	protected:
		boolean isValidOutputDataRate(OutputDataRate power_value);
		boolean isValidFullScale(FullScale sensitivity);
		boolean isValidBandwidth(Bandwidth bandwidth);
		boolean isValidOperatingMode(OperatingMode powerMode);
		float convertAxisValue(int value);
		void setManualBandwidthSelection();
		FullScale readFullScaleFromRegistry();
};

//Gyroscope class with specific sensor implementation
class LSM6DS3_Gyroscope: public LSM6DS3{
	public:
		LSM6DS3_Gyroscope();
		String getStatus();
		void resetToDefault();
	protected:
		boolean isValidOutputDataRate(OutputDataRate power_value);
		boolean isValidFullScale(FullScale sensitivity);
		boolean isValidOperatingMode(OperatingMode powerMode);
		float convertAxisValue(int value);
		FullScale readFullScaleFromRegistry();
};

extern LSM6DS3_Accelerometer accelerometer;
extern LSM6DS3_Gyroscope gyroscope;
//Gyroscope values
extern const FullScale G_FS_245_DPS;
extern const FullScale G_FS_500_DPS;
extern const FullScale G_FS_1000_DPS;
extern const FullScale G_FS_2000_DPS;
extern const OperatingMode G_HM_Normal;
extern const OperatingMode G_HM_High_Performance;
//Accelerometer values
extern const OperatingMode XL_HM_Normal;
extern const OperatingMode XL_HM_High_Performance;
extern const FullScale XL_FS_2G;
extern const FullScale XL_FS_4G;
extern const FullScale XL_FS_8G;
extern const FullScale XL_FS_16G;
extern const Bandwidth XL_Band_400Hz;
extern const Bandwidth XL_Band_200Hz;
extern const Bandwidth XL_Band_100Hz;
extern const Bandwidth XL_Band_50Hz;

#endif /* LSM6DS3_H */
