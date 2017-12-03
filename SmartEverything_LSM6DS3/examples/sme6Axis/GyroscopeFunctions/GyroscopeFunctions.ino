 /*
 * GyroscopeFunctions.ino
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
	gyroscope.begin();
}

void printMenu(){
	Serial.println("Test Menu'");
	Serial.println("Actions");
	Serial.println("1 - PowerOn Sensor");
	Serial.println("2 - PowerOff Sensor");
	Serial.println("3 - Reset to Default Values");
	Serial.println("4 - Print Sensor Status");
	Serial.println("5 - Print Sensor Registry Values");
	Serial.println("6 - Print Axis Values");
	Serial.println("Output Data Rate");
	Serial.println("7 - Pass to 12,5Hz");
	Serial.println("8 - Pass to 26Hz");
	Serial.println("9 - Pass to 52Hz");
	Serial.println("10 - Pass to 104Hz (Default)");
	Serial.println("11 - Pass to 208Hz");
	Serial.println("12 - Pass to 416Hz");
	Serial.println("13 - Pass to 833Hz");
	Serial.println("14 - Pass to 1,66KHz");
	Serial.println("Full Scale");
	Serial.println("15 - Pass to 245dps (Default)");
	Serial.println("16 - Pass to 500dps");
	Serial.println("17 - Pass to 1000dps");
	Serial.println("18 - Pass to 2000dps");
	Serial.println("Operating Mode");
	Serial.println("19 - Pass Normal");
	Serial.println("20 - High Performance (Default)");
	Serial.println("Choose a command");
	while(Serial.available() == 0){}
	digitalWrite(PIN_LED_13,HIGH);
	String read=Serial.readString();
	int value=read.toInt();
	Serial.println("+++++++++++++++++++++++++++++++++++++");
	switch(value){
		case 1:
			Serial.println("Power On");
			gyroscope.powerOn();
			break;
		case 2:
			Serial.println("Power Off");
			gyroscope.powerOff();
			break;
		case 3:
			Serial.println("Reset To Default Values");
			gyroscope.resetToDefault();
			break;
		case 4:
			if(gyroscope.isActive()){
				Serial.println("Gyroscope is Active");
			}else{
				Serial.println("Gyroscope is not Active");
			}
			break;
		case 5:
			Serial.println(gyroscope.getStatus());
			break;
		case 6:
			for(int i=1;i<=10;i++){
				Serial.println("*********************************************");
				Serial.print("Raw  X = ");
				Serial.print(gyroscope.getRawXAxis());
				Serial.print(" Y = ");
				Serial.print(gyroscope.getRawYAxis());
				Serial.print(" Z = ");
				Serial.println(gyroscope.getRawZAxis());
				Serial.print("X = ");
				Serial.print(gyroscope.getConvertedXAxis(), 2);
				Serial.print("dps Y = ");
				Serial.print(gyroscope.getConvertedYAxis(), 2);
				Serial.print("dps Z = ");
				Serial.print(gyroscope.getConvertedZAxis(), 2);
				Serial.println("dps");
				Serial.println("*********************************************");
				delay(1000);
			}
			break;
		case 7:
			Serial.println("Change OutputDataRate to 12.5Hz");
			gyroscope.changeOutputDataRate(POWER_12_5_HZ);
			break;
		case 8:
			Serial.println("Change OutputDataRate to 26Hz");
			gyroscope.changeOutputDataRate(POWER_26_HZ);
			break;
		case 9:
			Serial.println("Change OutputDataRate to 52Hz");
			gyroscope.changeOutputDataRate(POWER_52_HZ);
			break;		
		case 10:
			Serial.println("Change OutputDataRate to 104Hz");			
			gyroscope.changeOutputDataRate(POWER_104_HZ);
			break;
		case 11:
			Serial.println("Change OutputDataRate to 208Hz");
			gyroscope.changeOutputDataRate(POWER_208_HZ);
			break;
		case 12:
			Serial.println("Change OutputDataRate to 416Hz");
			gyroscope.changeOutputDataRate(POWER_416_HZ);
			break;
		case 13:
			Serial.println("Change OutputDataRate to 833Hz");
			gyroscope.changeOutputDataRate(POWER_833_HZ);
			break;
		case 14:
			Serial.println("Change OutputDataRate to 1.66KHz");
			gyroscope.changeOutputDataRate(POWER_1_66_KHZ);
			break;
		case 15:
			Serial.println("Change Full Scale to 245dps");
			gyroscope.changeFullScale(G_FS_245_DPS);
			break;
		case 16:
			Serial.println("Change Full Scale to 500dps");
			gyroscope.changeFullScale(G_FS_500_DPS);
			break;
		case 17:
			Serial.println("Change Full Scale to 1000dps");
			gyroscope.changeFullScale(G_FS_1000_DPS);
			break;
		case 18:
			Serial.println("Change Full Scale to 2000dps");
			gyroscope.changeFullScale(G_FS_2000_DPS);
			break;
		case 19:
			Serial.println("Change Operating Mode to Normal");
			gyroscope.changeOperatingMode(G_HM_Normal);
			break;
		case 20:
			Serial.println("Change Operating Mode to High Performance");
			gyroscope.changeOperatingMode(G_HM_High_Performance);
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
