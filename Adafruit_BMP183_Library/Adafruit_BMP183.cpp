/*************************************************** 
  This is a library for the Adafruit BMP183 Barometric Pressure + Temp sensor

  Designed specifically to work with the Adafruit BMP183 Breakout 
  ----> http://www.adafruit.com/products/1900

  These sensors use SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_BMP183.h"
#ifdef __AVR__
    #include <util/delay.h>
#endif
#define _delay_ms(t) delay(t)
#include <SPI.h>


/*!
 *  @brief  Instantiates a new Adafruit_BMP183 class using hardware SPI
 *  @param  SPICS
 *          cs pin
 *  @param  *theSPI
 *          optional SPI object, defaults to SPI
 */
Adafruit_BMP183::Adafruit_BMP183(int8_t SPICS, SPIClass *theSPI) {
  _cs = SPICS;
  _clk = _miso = _mosi = -1; 
  _spi = theSPI;
}

/*!
 *  @brief  Instantiates a new Adafruit_BMP183 class using software SPI
 *  @param  SPICLK
 *          SPI chip clock
 *  @param  SPIMISO
 *          SPI MISO (Data to microcontroller from sensor)
 *  @param  SPIMOSI
 *          SPI MOSI (Data from microcontroller to sensor)
 *  @param  SPICS
 *          SPI CS PIN
 */
Adafruit_BMP183::Adafruit_BMP183(int8_t SPICLK, 
				 int8_t SPIMISO, 
				 int8_t SPIMOSI, 
				 int8_t SPICS) {
  _cs = SPICS;
  _clk = SPICLK;
  _miso = SPIMISO;
  _mosi = SPIMOSI;
}

/*!
 *  @brief  Setups the HW
 *  @param  mode
 *          selected BMP183 mode
 *  @return true if successful
 */
boolean Adafruit_BMP183::begin(bmp183_mode_t mode) {
  if (_clk == -1) {
    _spi->begin();
    _spi->setDataMode(SPI_MODE0);
#ifdef __AVR__
    _spi->setClockDivider(SPI_CLOCK_DIV16);
#endif
#ifdef __SAM3X8E__
      _spi->setClockDivider(11); // 8-ish MHz (full! speed!)
#endif
  } else {
    pinMode(_clk, OUTPUT);
    digitalWrite(_clk, HIGH);
    pinMode(_mosi, OUTPUT);
    digitalWrite(_mosi, HIGH);
    pinMode(_miso, INPUT);
  }
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);


  /* Mode boundary check */
  if ((mode > BMP183_MODE_ULTRAHIGHRES) || (mode < 0))
  {
    mode = BMP183_MODE_ULTRAHIGHRES;
  }

  /* Set the mode indicator */
  oversampling = mode;


  if (read8(0xD0) != 0x55) return false;

  /* Read calibration data */
  ac1 = read16(BMP183_REGISTER_CAL_AC1);
  ac2 = read16(BMP183_REGISTER_CAL_AC2);
  ac3 = read16(BMP183_REGISTER_CAL_AC3);
  ac4 = read16(BMP183_REGISTER_CAL_AC4);
  ac5 = read16(BMP183_REGISTER_CAL_AC5);
  ac6 = read16(BMP183_REGISTER_CAL_AC6);

  b1 = read16(BMP183_REGISTER_CAL_B1);
  b2 = read16(BMP183_REGISTER_CAL_B2);

  mb = read16(BMP183_REGISTER_CAL_MB);
  mc = read16(BMP183_REGISTER_CAL_MC);
  md = read16(BMP183_REGISTER_CAL_MD);
#if (BMP183_DEBUG == 1)
  Serial.print("ac1 = "); Serial.println(ac1, DEC);
  Serial.print("ac2 = "); Serial.println(ac2, DEC);
  Serial.print("ac3 = "); Serial.println(ac3, DEC);
  Serial.print("ac4 = "); Serial.println(ac4, DEC);
  Serial.print("ac5 = "); Serial.println(ac5, DEC);
  Serial.print("ac6 = "); Serial.println(ac6, DEC);

  Serial.print("b1 = "); Serial.println(b1, DEC);
  Serial.print("b2 = "); Serial.println(b2, DEC);

  Serial.print("mb = "); Serial.println(mb, DEC);
  Serial.print("mc = "); Serial.println(mc, DEC);
  Serial.print("md = "); Serial.println(md, DEC);
#endif

  return true;
}

/*!
 * @brief  Reads raw temperature reading
 * @return Raw Temeperature from BMP183_REGISTER_TEMPDATA.
 */
uint16_t Adafruit_BMP183::readRawTemperature() {
  write8(BMP183_REGISTER_CONTROL, BMP183_REGISTER_READTEMPCMD);
  _delay_ms(5);
#if BMP183_DEBUG == 1
  Serial.print("Raw temp: "); Serial.println(read16(BMP183_REGISTER_TEMPDATA));
#endif
  return read16(BMP183_REGISTER_TEMPDATA);
}

/*!
 * @brief  Reads raw pressure reading
 * @return Raw Pressure from BMP183_REGISTER_PRESSUREDATA
 */
uint32_t Adafruit_BMP183::readRawPressure() {
  uint32_t raw;

  write8(BMP183_REGISTER_CONTROL, BMP183_REGISTER_READPRESSURECMD + (oversampling << 6));

  if (oversampling == BMP183_MODE_ULTRALOWPOWER) 
    _delay_ms(5);
  else if (oversampling == BMP183_MODE_STANDARD) 
    _delay_ms(8);
  else if (oversampling == BMP183_MODE_HIGHRES) 
    _delay_ms(14);
  else 
    _delay_ms(26);

  raw = read16(BMP183_REGISTER_PRESSUREDATA);

  raw <<= 8;
  raw |= read8(BMP183_REGISTER_PRESSUREDATA+2);
  raw >>= (8 - oversampling);

 /* this pull broke stuff, look at it later?
  if (oversampling==0) {
    raw <<= 8;
    raw |= read8(BMP183_PRESSUREDATA+2);
    raw >>= (8 - oversampling);
  }
 */

#if BMP183_DEBUG == 1
  Serial.print("Raw pressure: "); Serial.println(raw);
#endif
  return raw;
}

/*!
 *  @brief  Gets the compensated pressure level in hPa
 *  @return Pressure value in hPa
 */
int32_t Adafruit_BMP183::getPressure() {
  int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
  uint32_t B4, B7;

  UT = readRawTemperature();
  UP = readRawPressure();

#if BMP183_DEBUG == 1
  // use datasheet numbers!
  UT = 27898;
  UP = 23843;
  ac6 = 23153;
  ac5 = 32757;
  mc = -8711;
  md = 2868;
  b1 = 6190;
  b2 = 4;
  ac3 = -14383;
  ac2 = -72;
  ac1 = 408;
  ac4 = 32741;
  oversampling = 0;
#endif

  // do temperature calculations
  X1=(UT-(int32_t)(ac6))*((int32_t)(ac5))/pow(2,15);
  X2=((int32_t)mc*pow(2,11))/(X1+(int32_t)md);
  B5=X1 + X2;

#if BMP183_DEBUG == 1
  Serial.print("X1 = "); Serial.println(X1);
  Serial.print("X2 = "); Serial.println(X2);
  Serial.print("B5 = "); Serial.println(B5);
#endif

  // do pressure calcs
  B6 = B5 - 4000;
  X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
  X2 = ((int32_t)ac2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = ((((int32_t)ac1*4 + X3) << oversampling) + 2) / 4;

#if BMP183_DEBUG == 1
  Serial.print("B6 = "); Serial.println(B6);
  Serial.print("X1 = "); Serial.println(X1);
  Serial.print("X2 = "); Serial.println(X2);
  Serial.print("B3 = "); Serial.println(B3);
#endif

  X1 = ((int32_t)ac3 * B6) >> 13;
  X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
  B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );

#if BMP183_DEBUG == 1
  Serial.print("X1 = "); Serial.println(X1);
  Serial.print("X2 = "); Serial.println(X2);
  Serial.print("B4 = "); Serial.println(B4);
  Serial.print("B7 = "); Serial.println(B7);
#endif

  if (B7 < 0x80000000) {
    p = (B7 * 2) / B4;
  } else {
    p = (B7 / B4) * 2;
  }
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

#if BMP183_DEBUG == 1
  Serial.print("p = "); Serial.println(p);
  Serial.print("X1 = "); Serial.println(X1);
  Serial.print("X2 = "); Serial.println(X2);
#endif

  p = p + ((X1 + X2 + (int32_t)3791)>>4);
#if BMP183_DEBUG == 1
  Serial.print("p = "); Serial.println(p);
#endif
  return p;
}

/*!
 *  @brief  Reads the temperatures in Celsius degrees
 *  @return temperature in Celsius
 */
float Adafruit_BMP183::getTemperature() {
  int32_t UT, X1, X2, B5;     // following ds convention
  float temp;

  UT = readRawTemperature();

#if BMP183_DEBUG == 1
  // use datasheet numbers!
  UT = 27898;
  ac6 = 23153;
  ac5 = 32757;
  mc = -8711;
  md = 2868;
#endif

  // step 1
  X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) / pow(2,15);
  X2 = ((int32_t)mc * pow(2,11)) / (X1+(int32_t)md);
  B5 = X1 + X2;
  temp = (B5+8)/pow(2,4);
  temp /= 10;
  
  return temp;
}

/*!
 *  @brief  Reads the altitude based on provided sea level pressure
 *  @param  sealevelPressure
 *          pressure in hPa
 *  @return altitude value in meters
 */
float Adafruit_BMP183::getAltitude(float sealevelPressure) {
  float altitude;

  float pressure = getPressure(); // in Si units for Pascal
  pressure /= 100;

  altitude = 44330 * (1.0 - pow(pressure /sealevelPressure,0.1903));

  return altitude;
}


/*********************************************************************/

uint8_t Adafruit_BMP183::SPIxfer(uint8_t x) {
  if (_clk == -1) {
    return _spi->transfer(x);
  } else {
    //Serial.println("Software SPI");
    uint8_t reply = 0;
    for (int i=7; i>=0; i--) {
      reply <<= 1;
      digitalWrite(_clk, LOW);
      digitalWrite(_mosi, x & (1<<i));
      digitalWrite(_clk, HIGH);
      if (digitalRead(_miso)) 
	reply |= 1;
    }
    return reply;
  }
}


uint8_t Adafruit_BMP183::read8(uint8_t reg) {
  uint8_t value;

  digitalWrite(_cs, LOW);
  SPIxfer(0x80 | reg);
  value = SPIxfer(0x00);
  digitalWrite(_cs, HIGH);

  return value;
}

uint16_t Adafruit_BMP183::read16(uint8_t reg) {
  uint16_t value;

  digitalWrite(_cs, LOW);
  SPIxfer(0x80 | reg);
  value = SPIxfer(0x00);
  value <<= 8;
  value |= SPIxfer(0x00);
  digitalWrite(_cs, HIGH);

  return value;
}

void Adafruit_BMP183::write8(uint8_t reg, uint8_t value) {
  digitalWrite(_cs, LOW);
  SPIxfer(reg & 0x7F);
  SPIxfer(value);
  digitalWrite(_cs, HIGH);
}
