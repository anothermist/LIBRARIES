/**************************************************************************//**
 * \brief EEPROM 24C32 / 24C64 library for Arduino - Demonstration program
 * \author Copyright (C) 2012  Julien Le Sech - www.idreammicro.com
 * \version 1.0
 * \date 20120218
 *
 * This file is part of the EEPROM 24C32 / 24C64 library for Arduino.
 *
 * This library is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/
 ******************************************************************************/

/**************************************************************************//**
 * \file WriteReadByte.ino
 ******************************************************************************/
 
/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include <Wire.h>

#include <Eeprom24C32_64.h>

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \def EEPROM_ADDRESS
 * \brief Address of EEPROM memory on TWI bus.
 ******************************************************************************/
#define EEPROM_ADDRESS  0x50

/******************************************************************************
 * Private variable definitions.
 ******************************************************************************/

static Eeprom24C32_64 eeprom(EEPROM_ADDRESS);

/******************************************************************************
 * Public function definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \fn void setup()
 *
 * \brief
 ******************************************************************************/
void setup()
{
    // Initialize serial communication.
    Serial.begin(9600);
        
    // Initialize EEPROM library.
    eeprom.initialize();

    const word address = 0;
    
    // Write a byte at address 0 in EEPROM memory.
    Serial.println("Write byte to EEPROM memory...");
    eeprom.writeByte(address, 0xAA);
    
    // Write cycle time (tWR). See EEPROM memory datasheet for more details.
    delay(10);
    
    // Read a byte at address 0 in EEPROM memory.
    Serial.println("Read byte from EEPROM memory...");
    byte data = eeprom.readByte(address);
    
    // Print read byte.
    Serial.print("Read byte = 0x");
    Serial.print(data, HEX);
    Serial.println("");
}

/**************************************************************************//**
 * \fn void loop()
 *
 * \brief
 ******************************************************************************/
void loop()
{

}
