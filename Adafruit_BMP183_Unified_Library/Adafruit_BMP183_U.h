/*!
 * @file Adafruit_BMP183_U.h
 *
 * Designed specifically to work with the Adafruit BMP183 Breakout 
 * ----> http://www.adafruit.com/products/1900
 * 
 * These sensors use SPI to communicate, 4 pins are required
 * to interface.
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 *
 * Written by Kevin Townsend "KTOWN" for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */
#ifndef __BMP183_H__
#define __BMP183_H__

#include "Arduino.h"

#include <Adafruit_Sensor.h>
#include <SPI.h>


/** Registers **/
    enum
    {
      BMP183_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CHIPID             = 0xD0,
      BMP183_REGISTER_VERSION            = 0xD1,
      BMP183_REGISTER_SOFTRESET          = 0xE0,
      BMP183_REGISTER_CONTROL            = 0xF4,
      BMP183_REGISTER_TEMPDATA           = 0xF6,
      BMP183_REGISTER_PRESSUREDATA       = 0xF6,
      BMP183_REGISTER_READTEMPCMD        = 0x2E,
      BMP183_REGISTER_READPRESSURECMD    = 0x34
    };

/** Mode Settings **/
    typedef enum
    {
      BMP183_MODE_ULTRALOWPOWER          = 0,
      BMP183_MODE_STANDARD               = 1,
      BMP183_MODE_HIGHRES                = 2,
      BMP183_MODE_ULTRAHIGHRES           = 3
    } bmp183_mode_t;

/** Calibration Data **/
    typedef struct
    {
      int16_t  ac1; /**< Calibration coefficient (ac1) **/
      int16_t  ac2; /**< Calibration coefficient (ac2) **/
      int16_t  ac3; /**< Calibration coefficient (ac3) **/
      uint16_t ac4; /**< Calibration coefficient (ac4) **/
      uint16_t ac5; /**< Calibration coefficient (ac5) **/
      uint16_t ac6; /**< Calibration coefficient (ac6) **/
      int16_t  b1;  /**< Calibration coefficient (b1) **/
      int16_t  b2;  /**< Calibration coefficient (b2) **/
      int16_t  mb;  /**< Calibration coefficient (mb) **/
      int16_t  mc;  /**< Calibration coefficient (mc) **/
      int16_t  md;  /**< Calibration coefficient (md) **/
    } bmp183_calib_data;

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          BMP183
 */
class Adafruit_BMP183_Unified : public Adafruit_Sensor
{
  public:
    Adafruit_BMP183_Unified(int8_t SPICLK, int8_t SPIMISO, int8_t SPIMOSI, int8_t SPICS, int32_t sensorID = -1);
    Adafruit_BMP183_Unified(int8_t SPICS, int32_t sensorID = -1, SPIClass *theSPI = &SPI);
  
    bool  begin(bmp183_mode_t mode = BMP183_MODE_ULTRAHIGHRES);
    float  getTemperature();
    float  getPressure();
    float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    float seaLevelForAltitude(float altitude, float atmospheric, float temp);
    bool  getEvent(sensors_event_t*);
    void  getSensor(sensor_t*);

  private:
    void readCoefficients(void);
    int16_t readRawTemperature();
    int32_t readRawPressure();

    uint8_t read8(byte reg);
    int16_t readS16(byte reg);
    uint16_t read16(byte reg);
    void writeCommand(byte reg, byte value);

    uint8_t SPIxfer(uint8_t x);

    int32_t           _sensorID;
    bmp183_calib_data _bmp183_coeffs;   
    uint8_t           _bmp183Mode;

    int8_t _cs, _clk, _miso, _mosi;
};

#endif
