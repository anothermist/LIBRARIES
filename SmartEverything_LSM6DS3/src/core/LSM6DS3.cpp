 /*
 * LSM6DS3.cpp
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

#include "Wire.h"
#include "LSM6DS3.h"
#include "LSM6DS3Reg.h"

//Public methods

LSM6DS3::LSM6DS3() 
{
}

boolean LSM6DS3::powerOn(OutputDataRate odr){
	if(!_isActive){
		uint8_t data=readRegister(WHO_AM_I_REG_ADDRESS);
		if(data==WHO_AM_I_VALUE){
			if(changeOutputDataRate(odr)){
				_isActive=true;
				return true;
			}
		}
	}
	return false;
}

boolean LSM6DS3::powerOff(){
	if(_isActive){
		uint8_t data=readRegister(_sensorSettings.sensor_control_registry);
		data&=_sensorSettings.output_data_rate_reset_mask;
		writeRegister(_sensorSettings.sensor_control_registry,data);
		_isActive=false;
		return true;
	}
	return false;
}

boolean LSM6DS3::changeOutputDataRate(OutputDataRate odr)
{
	if(isValidOutputDataRate(odr)){
		changeRegistryValue(_sensorSettings.sensor_control_registry,_sensorSettings.output_data_rate_reset_mask,odr);
		return true;
	}
	return false;
}

boolean LSM6DS3::changeFullScale(FullScale sensitivity){
	if(isValidFullScale(sensitivity))
	{
		_sensorSettings.fullScale=sensitivity;
		changeRegistryValue(_sensorSettings.sensor_control_registry,_sensorSettings.full_scale_reset_mask,sensitivity.bits);
		return true;
	}
	return false;
}

boolean LSM6DS3::changeOperatingMode(OperatingMode operatingMode){
	if(isValidOperatingMode(operatingMode))
	{
		changeRegistryValue(_sensorSettings.power_mode_registry,_sensorSettings.operating_mode_reset_mask,operatingMode.bits);
		return true;
	}
	return false;
}

float LSM6DS3::getConvertedXAxis(){
	int value=getRawXAxis();
	return convertAxisValue(value);
}

float LSM6DS3::getConvertedYAxis(){
	int value=getRawYAxis();
	return convertAxisValue(value);
}

float LSM6DS3::getConvertedZAxis(){
	int value=getRawZAxis();
	return convertAxisValue(value);
}

int LSM6DS3::getRawXAxis()
{
	int data=0;
	if(_isActive){
		readCombinedRegistry(_sensorSettings.axisRegistry.XAxis.lowRegistry,_sensorSettings.axisRegistry.XAxis.highRegistry,&data);
	}
	return data;
}
int LSM6DS3::getRawYAxis()
{
	int data=0;
	if(_isActive){
		readCombinedRegistry(_sensorSettings.axisRegistry.YAxis.lowRegistry,_sensorSettings.axisRegistry.YAxis.highRegistry,&data);
	}
	return data;
}
int LSM6DS3::getRawZAxis()
{
	int data=0;
	if(_isActive){
		readCombinedRegistry(_sensorSettings.axisRegistry.ZAxis.lowRegistry,_sensorSettings.axisRegistry.ZAxis.highRegistry,&data);
	}
	return data;
}


void LSM6DS3::begin()
{
	//Read current value from the register
	_sensorSettings.fullScale=readFullScaleFromRegistry();
	byte odr=readRegister(_sensorSettings.sensor_control_registry)&~_sensorSettings.output_data_rate_reset_mask;
	_isActive=odr!=0x00;
}

void LSM6DS3::resetToDefault()
{
	changeOutputDataRate(POWER_104_HZ);
}

//Protected methods

void LSM6DS3::changeRegistryValue(byte registry,byte resetMask,byte value){
		uint8_t data=readRegister(registry);
		//Reset OperatingMode bits
		data&=resetMask;
		//Set the new value
		data|=value;
		writeRegister(registry,data);
}

boolean LSM6DS3::readCombinedRegistry(unsigned char lsb_registry,unsigned char msb_registry,int* value){
	uint16_t data = 0;
	unsigned char  read = 0;
	int16_t signed_data = 0;

	for (int i=0;i<CHECK_STATUS_RETRIES;i++) {
		read = readRegister(STATUS_REG_ADDRESS);
		if (read & _sensorSettings.status_sensor_value) {
			read = readRegister(lsb_registry);
			data = read;      // LSB

			read = readRegister(msb_registry);
			data |= read << 8; // MSB
			signed_data = data;
			*value=  signed_data;
			return true;
		} else {
			delay (1);
		}
	}
	return false;
}

byte LSM6DS3::readRegister(byte regToRead)
{
	Wire.beginTransmission(LSM6DS3_ADDRESS);
	Wire.write(regToRead);
	Wire.endTransmission(false); //endTransmission but keep the connection active

	Wire.requestFrom(LSM6DS3_ADDRESS, 1); //Ask for 1 byte, once done, bus is released by default

	while(!Wire.available()) ; //Wait for the data to come back
	return Wire.read(); //Return this one byte
}

boolean LSM6DS3::writeRegister(byte regToWrite, byte dataToWrite)
{
	uint8_t errorNo;
	Wire.beginTransmission(LSM6DS3_ADDRESS);

	if (!Wire.write(regToWrite)) {
		return false;
	}
	if (!Wire.write(dataToWrite)) {
		return false;
	}

	errorNo = Wire.endTransmission(); //Stop transmitting
	return (errorNo == 0);
}
