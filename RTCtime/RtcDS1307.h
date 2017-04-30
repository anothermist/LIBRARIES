#ifndef __RTCDS1307_H__
#define __RTCDS1307_H__

#include <Arduino.h>
#include <time.h>

// We define _BV here in case it hasn't been already (Oddly, it is not defined for the DUE board...)
#ifndef _BV
#define _BV(b) (1UL << (b))
#endif

//I2C Slave Address
const uint8_t DS1307_ADDRESS = 0x68;

//DS1307 Register Addresses
const uint8_t DS1307_REG_TIMEDATE   = 0x00;
const uint8_t DS1307_REG_STATUS     = 0x00;
const uint8_t DS1307_REG_CONTROL    = 0x07;
const uint8_t DS1307_REG_RAMSTART   = 0x08;
const uint8_t DS1307_REG_RAMEND     = 0x3f;
const uint8_t DS1307_REG_RAMSIZE = DS1307_REG_RAMEND - DS1307_REG_RAMSTART;

//DS1307 Register Data Size if not just 1
const uint8_t DS1307_REG_TIMEDATE_SIZE = 7;

// DS1307 Control Register Bits
const uint8_t DS1307_RS0   = 0;
const uint8_t DS1307_RS1   = 1;
const uint8_t DS1307_SQWE  = 4;
const uint8_t DS1307_OUT   = 7;

// DS1307 Status Register Bits
const uint8_t DS1307_CH    = 7;

enum DS1307SquareWaveOut
{
    DS1307SquareWaveOut_1Hz  =  0b00010000,
    DS1307SquareWaveOut_4kHz =  0b00010001,
    DS1307SquareWaveOut_8kHz =  0b00010010,
    DS1307SquareWaveOut_32kHz = 0b00010011,
    DS1307SquareWaveOut_High =  0b10000000,
    DS1307SquareWaveOut_Low =   0b00000000,
};

template<class T_WIRE_METHOD> class RtcDS1307
{
public:
    RtcDS1307(T_WIRE_METHOD& wire) :
        _wire(wire)
    {
    }


    void Begin()
    {
        _wire.begin();
    }


    bool IsDateTimeValid()
    {
        return GetIsRunning();
    }


    bool GetIsRunning()
    {
        uint8_t sreg = getReg(DS1307_REG_STATUS);
        return !(sreg & _BV(DS1307_CH));
    }


    void SetIsRunning(bool isRunning)
    {
        uint8_t sreg = getReg(DS1307_REG_STATUS);
        if (isRunning)
        {
            sreg &= ~_BV(DS1307_CH);
        }
        else
        {
            sreg |= _BV(DS1307_CH);
        }
        setReg(DS1307_REG_STATUS, sreg);
    }


    void SetTime(struct tm *now)
    {
        // retain running state
        uint8_t sreg = getReg(DS1307_REG_STATUS) & _BV(DS1307_CH);

        // set the date time
        _wire.beginTransmission(DS1307_ADDRESS);
        _wire.write(DS1307_REG_TIMEDATE);

        _wire.write(Uint8ToBcd(now->tm_sec) | sreg);
        _wire.write(Uint8ToBcd(now->tm_min));
        _wire.write(Uint8ToBcd(now->tm_hour)); // 24 hour mode only

        _wire.write(Uint8ToBcd(now->tm_wday + 1));
        _wire.write(Uint8ToBcd(now->tm_mday));
        _wire.write(Uint8ToBcd(now->tm_mon + 1));
        _wire.write(Uint8ToBcd(now->tm_year - 100));

        _wire.endTransmission();
    }
    void SetTime(time_t *now)
    {
      struct tm now_tm;
      gmtime_r(now, &now_tm);
      SetTime(&now_tm);
    }


    void SetTimeUX(time_t *UXnow)
    {
      time_t now = *UXnow - UNIX_OFFSET;
      SetTime(&now);
    }
    void SetTimeUX(time_t UXnow)
    {
      SetTimeUX(&UXnow);
    }


    time_t GetTime()
    {
        struct tm now;

        _wire.beginTransmission(DS1307_ADDRESS);
        _wire.write(DS1307_REG_TIMEDATE);
        _wire.endTransmission();
        _wire.requestFrom(DS1307_ADDRESS, DS1307_REG_TIMEDATE_SIZE);

        now.tm_sec = BcdToUint8(_wire.read() & 0x7F);
        now.tm_min = BcdToUint8(_wire.read());
        now.tm_hour = BcdHourToUint8(_wire.read());
        now.tm_wday = _wire.read() - 1;
        now.tm_mday = BcdToUint8(_wire.read());
        now.tm_mon = BcdToUint8(_wire.read()) - 1;
        now.tm_year = BcdToUint8(_wire.read()) + 100;

        return mk_gmtime(&now);
    }
    void GetTime(struct tm *utc_tm)
    {
      time_t now = GetTime();
      gmtime_r(&now, utc_tm);
    }


    time_t GetTimeUX()
    {
      return (GetTime() + UNIX_OFFSET);
    }


    void GetLocalTime(struct tm *local_tm)
    {
      time_t now = GetTime();
      localtime_r(&now, local_tm);
    }


    void SetSquareWavePin(DS1307SquareWaveOut pinMode)
    {
        setReg(DS1307_REG_CONTROL, pinMode);
    }
    void SetMemory(uint8_t memoryAddress, uint8_t value)
    {
        uint8_t address = memoryAddress + DS1307_REG_RAMSTART;
        if (address <= DS1307_REG_RAMEND)
        {
            setReg(address, value);
        }
    }


    uint8_t SetMemory(uint8_t memoryAddress, const uint8_t* pValue, uint8_t countBytes)
    {
        uint8_t address = memoryAddress + DS1307_REG_RAMSTART;
        uint8_t countWritten = 0;
        if (address <= DS1307_REG_RAMEND)
        {
            _wire.beginTransmission(DS1307_ADDRESS);
            _wire.write(address);

            while (countBytes > 0 && address <= DS1307_REG_RAMEND)
            {
                _wire.write(*pValue++);
                address++;
                countBytes--;
                countWritten++;
            }

            _wire.endTransmission();
        }
        return countWritten;
    }


    uint8_t GetMemory(uint8_t memoryAddress)
    {
        uint8_t value = 0;
        uint8_t address = memoryAddress + DS1307_REG_RAMSTART;
        if (address <= DS1307_REG_RAMEND)
        {
            value = getReg(address);
        }
        return value;
    }


    uint8_t GetMemory(uint8_t memoryAddress, uint8_t* pValue, uint8_t countBytes)
    {
        uint8_t address = memoryAddress + DS1307_REG_RAMSTART;
        uint8_t countRead = 0;
        if (address <= DS1307_REG_RAMEND)
        {
            if (countBytes > DS1307_REG_RAMSIZE)
            {
                countBytes = DS1307_REG_RAMSIZE;
            }

            _wire.beginTransmission(DS1307_ADDRESS);
            _wire.write(address);
            _wire.endTransmission();

            _wire.requestFrom(DS1307_ADDRESS, countBytes);

            while (countBytes-- > 0)
            {
                *pValue++ = _wire.read();
                countRead++;
            }
        }

        return countRead;
    }



private:
    T_WIRE_METHOD& _wire;

    uint8_t getReg(uint8_t regAddress)
    {
        _wire.beginTransmission(DS1307_ADDRESS);
        _wire.write(regAddress);
        _wire.endTransmission();

        // control register
        _wire.requestFrom(DS1307_ADDRESS, (uint8_t)1);

        uint8_t regValue = _wire.read();
        return regValue;
    }


    void setReg(uint8_t regAddress, uint8_t regValue)
    {
        _wire.beginTransmission(DS1307_ADDRESS);
        _wire.write(regAddress);
        _wire.write(regValue);
        _wire.endTransmission();
    }


    uint8_t BcdToUint8(uint8_t val)
    {
        return val - 6 * (val >> 4);
    }


    uint8_t Uint8ToBcd(uint8_t val)
    {
        return val + 6 * (val / 10);
    }


    uint8_t BcdHourToUint8(uint8_t bcdHour)
    {
        uint8_t hour;
        if (bcdHour & 0x40)
        {
            // 12 hour mode, convert to 24
            bool isPm = ((bcdHour & 0x20) != 0);

            hour = BcdToUint8(bcdHour & 0x1f);
            if (isPm)
            {
              hour += 12;
            }
        }
        else
        {
            hour = BcdToUint8(bcdHour);
        }
        return hour;
    }


};

#endif // __RTCDS1307_H__
