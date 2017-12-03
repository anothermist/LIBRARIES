 /*
 * AccelerometerFunctions
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
#include <Arduino.h>
#include "LSM6DS3.h"
#include "Wire.h"

void setup() {
	pinMode(PIN_LED_13,OUTPUT);
	Wire.begin();
	Serial.begin(115200);
	while(!Serial){};
	//Begin should be called after Wire.begin()
	accelerometer.begin();
}

void printMenu(){
	Serial.println("Test Menu'");
	Serial.println("Actions");
	Serial.println("1 - PowerOn Sensor");
	Serial.println("2 - PowerOff Sensor");
	Serial.println("3 - Reset to Default Values");
	Serial.println("4 - Print Sensor Status");
	Serial.println("5 - Print Sensor Registry Values");
	Serial.println("6 - Print values");
	Serial.println("Output Data Rate");
	Serial.println("7 - Pass to 12,5Hz");
	Serial.println("8 - Pass to 26Hz");
	Serial.println("9 - Pass to 52Hz");
	Serial.println("10 - Pass to 104Hz (Default)");
	Serial.println("11 - Pass to 208Hz");
	Serial.println("12 - Pass to 416Hz");
	Serial.println("13 - Pass to 833Hz");
	Serial.println("14 - Pass to 1,66KHz");
	Serial.println("15 - Pass to 3,33Khz");
	Serial.println("16 - Pass to 6,66Khz");
	Serial.println("Full Scale");
	Serial.println("17 - Pass to +/- 2G (Default)");
	Serial.println("18 - Pass to +/- 4G");
	Serial.println("19 - Pass to +/- 8G");
	Serial.println("20 - Pass to +/- 16G");
	Serial.println("Bandwidth");
	Serial.println("21 - Pass to 50Hz");
	Serial.println("22 - Pass to 100Hz");
	Serial.println("23 - Pass to 200Hz");
	Serial.println("24 - Pass to 400Hz (Default)");
	Serial.println("Operating Mode");
	Serial.println("25 - Pass Normal");
	Serial.println("26 - High Performance (Default)");
	Serial.println("Choose a command");
	while(Serial.available() == 0){}
	digitalWrite(PIN_LED_13,HIGH);
	String read=Serial.readString();
	int value=read.toInt();
	Serial.println("+++++++++++++++++++++++++++++++++++++");
	switch(value){
		case 1:
			Serial.println("Power On");
			accelerometer.powerOn();
			break;
		case 2:
			Serial.println("Power Off");
			accelerometer.powerOff();
			break;
		case 3:
			Serial.println("Reset To Default Values");
			accelerometer.resetToDefault();
			break;
		case 4:
			if(accelerometer.isActive()){
				Serial.println("Accelerometer is Active");
			}else{
				Serial.println("Accelerometer is not Active");
			}
			break;
		case 5:
			Serial.println(accelerometer.getStatus());
		break;
		case 6:
			for(int i=1;i<=10;i++){
				Serial.println("*********************************************");
				Serial.print("Raw  X = ");
				Serial.print(accelerometer.getRawXAxis());
				Serial.print("     Y = ");
				Serial.print(accelerometer.getRawYAxis());
				Serial.print("     Z = ");
				Serial.println(accelerometer.getRawZAxis());
				Serial.print(" X = ");
				Serial.print(accelerometer.getConvertedXAxis(), 2);
				Serial.print("g Y = ");
				Serial.print(accelerometer.getConvertedYAxis(), 2);
				Serial.print("g Z = ");
				Serial.print(accelerometer.getConvertedZAxis(), 2);
				Serial.println("g");
				Serial.println("*********************************************");
				delay(1000);
			}
			break;
		case 7:
			Serial.println("Change OutputDataRate to 12.5Hz");
			accelerometer.changeOutputDataRate(POWER_12_5_HZ);
			break;
		case 8:
			Serial.println("Change OutputDataRate to 26Hz");
			accelerometer.changeOutputDataRate(POWER_26_HZ);
			break;
		case 9:
			Serial.println("Change OutputDataRate to 52Hz");
			accelerometer.changeOutputDataRate(POWER_52_HZ);
			break;		
		case 10:
			Serial.println("Change OutputDataRate to 104Hz");
			accelerometer.changeOutputDataRate(POWER_104_HZ);
			break;
		case 11:
			Serial.println("Change OutputDataRate to 208Hz");
			accelerometer.changeOutputDataRate(POWER_208_HZ);
			break;
		case 12:
			Serial.println("Change OutputDataRate to 416Hz");
			accelerometer.changeOutputDataRate(POWER_416_HZ);
			break;
		case 13:
			Serial.println("Change OutputDataRate to 833Hz");
			accelerometer.changeOutputDataRate(POWER_833_HZ);
			break;
		case 14:
			Serial.println("Change OutputDataRate to 1.66KHz");
			accelerometer.changeOutputDataRate(POWER_1_66_KHZ);
			break;
		case 15:
			Serial.println("Change OutputDataRate to 3.33KHz");
			accelerometer.changeOutputDataRate(POWER_3_33_KHZ);
			break;
		case 16:
			Serial.println("Change OutputDataRate to 6.66KHz");
			accelerometer.changeOutputDataRate(POWER_6_66_KHZ);
			break;
		case 17:
			Serial.println("Change Full Scale to ±2G");
			accelerometer.changeFullScale(XL_FS_2G);
			break;
		case 18:
			Serial.println("Change Full Scale to ±4G");
			accelerometer.changeFullScale(XL_FS_4G);
			break;
		case 19:
			Serial.println("Change Full Scale to ±8G");
			accelerometer.changeFullScale(XL_FS_8G);
			break;
		case 20:
			Serial.println("Change Full Scale to ±16G");
			accelerometer.changeFullScale(XL_FS_16G);
			break;
		case 21:
			Serial.println("Change Bandwidth to 50Hz");
			accelerometer.changeBandwidth(XL_Band_50Hz);
			break;
		case 22:
			Serial.println("Change Bandwidth to 100Hz");
			accelerometer.changeBandwidth(XL_Band_100Hz);
			break;
		case 23:
			Serial.println("Change Bandwidth to 200Hz");
			accelerometer.changeBandwidth(XL_Band_200Hz);
			break;
		case 24:
			Serial.println("Change Bandwidth to 400Hz");
			accelerometer.changeBandwidth(XL_Band_400Hz);
			break;
		case 25:
			Serial.println("Change Operating Mode to Normal");
			accelerometer.changeOperatingMode(XL_HM_Normal);
			break;
		case 26:
			Serial.println("Change Operating Mode to High Performance");
			accelerometer.changeOperatingMode(XL_HM_High_Performance);
			break;
		default:
			Serial.println("Unknown Command");
	}
	Serial.println("+++++++++++++++++++++++++++++++++++++");
	digitalWrite(PIN_LED_13,LOW);
}

void loop() {
	printMenu();
}

