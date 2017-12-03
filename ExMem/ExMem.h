// ExMem.h created for ExMem library v1.0.0 on 12/3/2012.
// written by Corey Johnson

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

/*******************************************************************************
*   This Library is written for the Arduino Mega 1280/2560. See the ATmega1280/
* 2560 data sheet on how to connect your hardware to the External Memory
* Interface of the ATmega. I have successfully tested this library with a
* Mega1280/2560 and the texas instraments bq3287EA Real-Time Clock and an Atmel
* AT29HC256 EEPROM connected in parallel. I used a 74ACH573 transparent latch
* between the mega and the EEPROM/RTC as per the ATmega data sheet. I used the
* ExMem.enable(16, 0x8000, 0, 0) function to set up the External Memory Inter-
* face to bitWidth = 16, 0x8000 = upper/lower sector boundry, 0 = upper sector
* wait-states, 0 = lower sector wait-states. I was the able to read/write to
* the RTC and EEPROM as SRAM address space of the mega. The RTC being the lower
* sector and the EEPROM being the upper sector. See the data sheets for the
* ATmega1280/2560 and the hardware you are interfacing. Your hardware must use
* a parallel address/data bus (8-bit to 16-bit address width/8-bit data width).  
* See ExMem_Example in examples folder for example of use.
*******************************************************************************/

#ifndef EXMEM_H
#define EXMEM_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <inttypes.h>
#include <avr/io.h>

// Create exmem class
class exmem
{
    public:
	    // To use externally (in your code)
		static bool LOWER;
		static bool UPPER;
	
		// Constructor
        exmem();
        
        // Enable external memory interface
        void enable(uint8_t _addressWidth = 16,
                    uint16_t _upperLowerBoundry = 0x0000,
                    uint8_t _upperSectorWaitState = 0x00,
                    uint8_t _lowerSectorWaitState = 0x00);
        
        // Disable external memory interface
        void disable(void);
        
        // Writes data of any type
        template <class TYPE>
        uint16_t write(bool sector,
                       uint16_t address,
                       const TYPE &data);
        
        // Reads data of any type
        template <class TYPE>
        uint16_t read(bool sector,
                      uint16_t address,
                      TYPE &store);
}; /* exmem */

extern exmem ExMem; // Create external library interface

/* internal variables */

// Offset from start of lower sector(0x2200)
// to start of upper sector
static uint16_t upperSectorOffset = 0x0000; // default value

// Highest accessible address
static uint16_t upperLimit = 0xFFFF; // default value

// Sector size = Highest valid address + 1.
static uint16_t upperSize = 0xDE00; // default value

// Sector size = Highest valid address + 1.
static uint16_t lowerSize = 0xDE00; // default value

static bool LOWER = 0;
static bool UPPER = 1;

// Constructor definition
exmem::exmem()
{
}

// Using enable() with no parameters selects the default parameters.
// Any invalid value results in the default value being set.

// Valid _addressWidth values =
// 8, 10, 11, 12, 13, 14, 15, 16(default)

// Valid upperLowerBoundry values =
// 0x0000(default), 0x4000, 0x6000, 0x8000, 0xA000, 0xC000, 0xE000
//     *note    0x0000 = no lower sector

// Valid wait-state values for Upper and Lower sectors = 0(default), 1, 2, 3
// 0 = no wait-states
// 1 = 1 wait-state for read/write
// 2 = 2 wait-states for read/write
// 3 = 2 wait-states for read/write plus 1 wait-state after address to bus

// Sets and enables external memory interface. Note that if you omit the
// arguments the default values will be used.
void exmem::enable(uint8_t _addressWidth, uint16_t _upperLowerBoundry,
                   uint8_t upperSectorWaitState, uint8_t lowerSectorWaitState)
{
    // Release unused address pins from Port C
    XMCRB &= 0xF8; // Clear External Memory High Mask register bits
    switch (_addressWidth)
    {
        case 8: // Releases all Port C pins (Mega pins 30-37)
            upperLimit = 0x00FF; // Set highest accessible memory address
            XMCRB = 0x07; // Set External Memory High Mask register bits
            break;
        case 10: // Releases Port C7-C2 (Mega pins 30-35)
            upperLimit = 0x03FF;
            XMCRB = 0x06;
            break;
        case 11: // Releases Port C7-C3 (Mega pins 30-34)
            upperLimit = 0x07FF;
            XMCRB = 0x05;
            break;
        case 12: // Releases Port C7-C4 (Mega pins 30-33)
            upperLimit = 0x0FFF;
            XMCRB = 0x04;
            break;
        case 13: // Releases Port C7-C5 (Mega pins 30-32)
            upperLimit = 0x1FFF;
            XMCRB = 0x03;
            break;
        case 14: // Releases Port C7-C6 (Mega pins 30-31)
            upperLimit = 0x3FFF;
            XMCRB = 0x02;
            break;
        case 15: // Releases Port C7 (Mega pin 30)
            upperLimit = 0x7FFF;
            XMCRB = 0x01;
            break;
        default: // Use all Port C pins(default)
            upperLimit = 0xFFFF;
            break;
    }
    
    // Adjust _upperLowerBoundry for _addressWidth
    uint16_t boundry;
    if (_addressWidth < 15)
    {
        boundry = 0x0000;
    }
    else if (_addressWidth < 16)
    {
        boundry = constrain(_upperLowerBoundry, 0x0000, 0x6000);
    }
    else
    {
        boundry = _upperLowerBoundry;
    }

/***************************************************************************
* You can have two different wait-state settings if needed. (eg. Two
* hardware devices each having different timing specs). If only one wait
* state setting is needed set _upperLowerBoundry to 0x0000(upper sector only)
* in enable() and omit the lower sector wait-state argument. Otherwise enter
* valid values for the upper and lower sector wait-state arguments and each
* will have it's own wait-state setting. If enable() is used with no
* arguments the defaults values are used. If there is only an upper sector
* reading/writing to lower or upper sector will behave the same as there is
* only one sector.
***************************************************************************/
    
    // Sets the address boundry between lower and upper sectors
    XMCRA &= 0x8F; // Clears External Memory Upper Sector Limit bits
    switch (boundry)
    {   
        case 0x4000: // Sets upper sector to 0x4000
            upperSectorOffset = 0x1E00; // Sets upper sector offset
            XMCRA |= 0x20; // Sets External Memory Upper Sector Limit bits
            break;
        case 0x6000: // Sets upper sector to 0x6000
            upperSectorOffset = 0x3E00;
            XMCRA |= 0x30;
            break;
        case 0x8000: // Sets upper sector to 0x8000
            upperSectorOffset = 0x5E00;
            XMCRA |= 0x40;
            break;
        case 0xA000: // Sets upper sector to 0xA000
            upperSectorOffset = 0x7E00;
            XMCRA |= 0x50;
            break;
        case 0xC000: // Sets upper sector to 0xC000
            upperSectorOffset = 0x9E00;
            XMCRA |= 0x60;
            break;
        case 0xE000: // Sets upper sector to 0xE000
            upperSectorOffset = 0xBE00;
            XMCRA |= 0x70;
            break;
        default: // Sets upper sector to 0x0000(default)
            upperSectorOffset = 0x0000;
            break;
    }
    
    // Set upper/lower sector sizes
    if (0 < upperSectorOffset)
    {
        lowerSize = upperSectorOffset;
        upperSize = ((1 + upperLimit) - (0x2200 + upperSectorOffset));
    }
    else
    {
        lowerSize = upperSize = ((1 + upperLimit) - 0x2200);
    }
    
    // Set the upper sector wait-states
    XMCRA &= 0xF3; // Clears Upper Sector Wait State register bits
    switch (upperSectorWaitState)
    {
        case 1: // 1 wait-state for read/write
            XMCRA |= 0x04; // Sets Upper Sector Wait State register bits
            break;
        case 2: // 2 wait-states for read/write
            XMCRA |= 0x08;
            break;
        case 3: // 2 wait-states for read/write and 1 wait-state after address
            XMCRA |= 0x0C;
            break;
        default: // No wait-states(default)
            break;
    }
    
    // Sets the lower sector wait-states
    XMCRA &= 0xFC; // Clears the Lower Sector Wait State register bits
    switch (lowerSectorWaitState)
    {
        case 1: // 1 wait-state for read/write
            XMCRA |= 0x01;
            break;
        case 2: // 2 wait-states for read/write
            XMCRA |= 0x02;
            break;
        case 3: // 2 wait-states for read/write and 1 wait-state after address
            XMCRA |= 0x03;
            break;
        default: // No wait-states(default)
            break;
    }
    
    // Enable External Memory Interface
    XMCRA |= 0x80;
}

// Disable External Memory Interface
void exmem::disable(void)
{
    XMCRA &= 0x7F;
}

/***************************************************************************
* To select the lower/upper sector enter LOWER or UPPER respectively for the
* sector argument (0 or 1 may also be used). Any other value will produce an
* error. Valid Addresses for upper and lower sectors are from 0x0000 up to
* the size of the sector - 1. If an invalid address is used the read/write
* function will exit without the read/write being performed. Any valid data
* type can be passed to the read/write functions. The functions will return
* the size of data type read/written. Pass a variable of valid data type
* to the read function to hold the data read by the function. Note: if you
* pass a pointer to the functions, i.e. data type char*, the functions will
* return the size of the pointer (2 bytes), not the number of bytes held by
* the variable being pointed to. 
***************************************************************************/

// Writes any valid data type to external memory
template <class TYPE>
uint16_t exmem::write(bool sector, uint16_t address, const TYPE &data)
{
    // Pointer to first External Memory address.
    uint8_t *pExtMemory = reinterpret_cast <uint8_t *> (0x2200);
    
    // Pointer to the data being written
    const uint8_t *pData = (const uint8_t *)(const void *)&data;
    
    uint16_t i; // index for number of bytes
    i = 0;
    
    // Exit if address is out of range
    if (sector) // Upper sector
    {
        if (address >= upperSize)
        {
            return i;
        }
    }
    else // Lower sector
    {
        if (address >= lowerSize)
        {
            return i;
        }
    }
    
    // Write byte(s) to external memory
    for (; i < sizeof(data); i++)
    {
        if (sector) // Upper sector
        {
            // Create address pointer
            uint8_t *pMemory;
            
            // Set pointer to upper sector address
            pMemory = (uint8_t *)(pExtMemory + upperSectorOffset + address++);
            
            // Copy data to address
            *pMemory = (uint8_t)(*pData++);
        }
        else // Lower sector
        {
            // Create address pointer
            uint8_t *pMemory;
            
            // Set pointer to lower sector address
            pMemory = (uint8_t *)(pExtMemory + address++);
            
            // Copy data to address
            *pMemory = (uint8_t)(*pData++);
        }
    }
    
    // Return the number of bytes written
    return sizeof(data);
}

// Reads any valid data type from external memory
// into a variable passed to the function
template <class TYPE>
uint16_t exmem::read(bool sector, uint16_t address, TYPE &store)
{
    // Pointer to first External Memory address
    uint8_t *pExtMemory = reinterpret_cast <uint8_t *> (0x2200);
    
    // Pointer to the passed variable that holds the data
    uint8_t *pStore = (uint8_t *)(void *)&store;
    
     uint16_t i; // index for number of bytes
    i = 0;
    
    // Exit if address is out of range
    if (sector) // Upper sector
    {
        if (address >= upperSize)
        {
            return i;
        }
    }
    else // Lower sector
    {
        if (address >= lowerSize)
        {
            return i;
        }
    }
    
    // Read byte(s) from external memory
    for (; i < sizeof(store); i++)
    {
        if (sector) // Upper sector
        {
            // Create a pointer
            uint8_t *pMemory;
            
            // Set pointer to upper sector address
            pMemory = (pExtMemory + upperSectorOffset + address++);
            
            // Copy data from address to variable
            *pStore++ = (uint8_t)(*pMemory);
        }
        else // Lower sector
        {
            // Create a pointer
            uint8_t *pMemory;
            
            // Set pointer to lower sector address
            pMemory = (pExtMemory + address++);
            
            // Copy data from address to variable
            *pStore++ = (uint8_t)(*pMemory);
        }
    }
    
    // Return the number of bytes read
    return sizeof(store);
}

exmem ExMem = exmem(); // Create an instance named 'ExMem'

#endif /* EXMEM_H */