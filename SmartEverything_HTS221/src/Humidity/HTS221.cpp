/*
 * HTS221.cpp
 *
 * Created: 02/01/2015 20:50:20
 *  Author: speirano
 */
#include "Wire.h"
#include "HTS221Reg.h"
#include "HTS221.h"



static inline bool humidityReady(uint8_t data) {
    return (data & 0x02);
}
static inline bool temperatureReady(uint8_t data) {
    return (data & 0x01);
}


HTS221::HTS221(void) : _address(HTS221_ADDRESS)
{
    _temperature = 0.0;
    _humidity    = 0.0;
}


bool HTS221::begin(void)
{
    uint8_t data;

    data = readRegister(_address, WHO_AM_I);
    if (data == WHO_AM_I_RETURN){
        if (activate()){
            storeCalibration();
            return true;
        }
    }

    return false;
}

bool
HTS221::storeCalibration(void)
{
    uint8_t data;
    uint16_t tmp;

    for (int reg=CALIB_START; reg<=CALIB_END; reg++) {
        if ((reg!=CALIB_START+8) && (reg!=CALIB_START+9) && (reg!=CALIB_START+4)) {

            data = readRegister(HTS221_ADDRESS, reg);

            switch (reg) {
            case CALIB_START:
                _h0_rH = data;
                break;
            case CALIB_START+1:
            _h1_rH = data;
            break;
            case CALIB_START+2:
            _T0_degC = data;
            break;
            case CALIB_START+3:
            _T1_degC = data;
            break;

            case CALIB_START+5:
            tmp = _T0_degC;
            _T0_degC = (data&0x3)<<8;
            _T0_degC |= tmp;

            tmp = _T1_degC;
            _T1_degC = ((data&0xC)>>2)<<8;
            _T1_degC |= tmp;
            break;
            case CALIB_START+6:
            _H0_T0 = data;
            break;
            case CALIB_START+7:
            _H0_T0 |= data<<8;
            break;
            case CALIB_START+0xA:
            _H1_T0 = data;
            break;
            case CALIB_START+0xB:
            _H1_T0 |= data <<8;
            break;
            case CALIB_START+0xC:
            _T0_OUT = data;
            break;
            case CALIB_START+0xD:
            _T0_OUT |= data << 8;
            break;
            case CALIB_START+0xE:
            _T1_OUT = data;
            break;
            case CALIB_START+0xF:
            _T1_OUT |= data << 8;
            break;


            case CALIB_START+8:
            case CALIB_START+9:
            case CALIB_START+4:
            //DO NOTHING
            break;

            // to cover any possible error
            default:
                return false;
            } /* switch */
        } /* if */
    }  /* for */
    return true;
}


bool
HTS221::activate(void)
{
    uint8_t data;

    data = readRegister(_address, CTRL_REG1);
    data |= POWER_UP;
    data |= ODR0_SET;
    writeRegister(_address, CTRL_REG1, data);

    return true;
}


bool HTS221::deactivate(void)
{
    uint8_t data;

    data = readRegister(_address, CTRL_REG1);
    data &= ~POWER_UP;
    writeRegister(_address, CTRL_REG1, data);
    return true;
}


bool
HTS221::bduActivate(void)
{
    uint8_t data;

    data = readRegister(_address, CTRL_REG1);
    data |= BDU_SET;
    writeRegister(_address, CTRL_REG1, data);

    return true;
}


bool
HTS221::bduDeactivate(void)
{
    uint8_t data;

    data = readRegister(_address, CTRL_REG1);
    data &= ~BDU_SET;
    writeRegister(_address, CTRL_REG1, data);
    return true;
}


const double
HTS221::readHumidity(void)
{
    uint8_t data   = 0;
    uint16_t h_out = 0;
    double h_temp  = 0.0;
    double hum     = 0.0;

    data = readRegister(_address, STATUS_REG);

    if (data & HUMIDITY_READY) {
        data = readRegister(_address, HUMIDITY_H_REG);
        h_out = data << 8;  // MSB
        data = readRegister(_address, HUMIDITY_L_REG);
        h_out |= data;      // LSB

        // Decode Humidity
        hum = ((int16_t)(_h1_rH) - (int16_t)(_h0_rH))/2.0;  // remove x2 multiple

        // Calculate humidity in decimal of grade centigrades i.e. 15.0 = 150.
        h_temp = (double)(((int16_t)h_out - (int16_t)_H0_T0) * hum) / 
	         (double)((int16_t)_H1_T0 - (int16_t)_H0_T0);
        hum    = (double)((int16_t)_h0_rH) / 2.0; // remove x2 multiple
        _humidity = (hum + h_temp); // provide signed % measurement unit
    }
    return _humidity;
}



const double
HTS221::readTemperature(void)
{
    uint8_t data   = 0;
    uint16_t t_out = 0;
    double t_temp  = 0.0;
    double deg     = 0.0;

    data = readRegister(_address, STATUS_REG);

    if (data & TEMPERATURE_READY) {

        data= readRegister(_address, TEMP_H_REG);
        t_out = data  << 8; // MSB
        data = readRegister(_address, TEMP_L_REG);
        t_out |= data;      // LSB

        // Decode Temperature
        deg    = (double)((int16_t)(_T1_degC) - (int16_t)(_T0_degC))/8.0; // remove x8 multiple

        // Calculate Temperature in decimal of grade centigrades i.e. 15.0 = 150.
        t_temp = (double)(((int16_t)t_out - (int16_t)_T0_OUT) * deg) / 
	         (double)((int16_t)_T1_OUT - (int16_t)_T0_OUT);
        deg    = (double)((int16_t)_T0_degC) / 8.0;     // remove x8 multiple
        _temperature = deg + t_temp;   // provide signed celsius measurement unit
    }

    return _temperature;
}



// Read a single byte from addressToRead and return it as a byte
byte HTS221::readRegister(byte slaveAddress, byte regToRead)
{
    Wire.beginTransmission(slaveAddress);
    Wire.write(regToRead);
    Wire.endTransmission(false); //endTransmission but keep the connection active

    Wire.requestFrom(slaveAddress, 1); //Ask for 1 byte, once done, bus is released by default

    while(!Wire.available()) ; //Wait for the data to come back
    return Wire.read(); //Return this one byte
}

// Writes a single byte (dataToWrite) into regToWrite
bool HTS221::writeRegister(byte slaveAddress, byte regToWrite, byte dataToWrite)
{
    Wire.beginTransmission(slaveAddress);

    if (!Wire.write(regToWrite)) {
        return false;
    }
    if (!Wire.write(dataToWrite)) {
        return false;
    }

    uint8_t errorNo = Wire.endTransmission(); //Stop transmitting
    return (errorNo == 0);
}


HTS221 smeHumidity;
