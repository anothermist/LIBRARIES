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
 * \file WriteReadBytes.ino
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
#define EEPROM_ADDRESS 0x50

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
    
    // Initiliaze EEPROM library.
    eeprom.initialize();

    const word address = 0;
    const byte count = 94;

    // Declare byte arrays.
    byte inputBytes[count] = { 0 };
    byte outputBytes[count] = { 0 };

    // Fill input array with printable characters. See ASCII table for more
    // details.
    for (byte i = 0; i < count; i++)
    {    
        inputBytes[i] = i + 33;
    }

    // Write input array to EEPROM memory.
    Serial.println("Write bytes to EEPROM memory...");
    eeprom.writeBytes(address, count, inputBytes);

    // Read array with bytes read from EEPROM memory.
    Serial.println("Read bytes from EEPROM memory...");
    eeprom.readBytes(address, count, outputBytes);
    
    // Print read bytes.
    Serial.println("Read bytes:");
    for (byte i = 0; i < count; i++)
    {
        Serial.write(outputBytes[i]);
        Serial.print(" ");
    }
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
