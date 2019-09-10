/*!
 *  @file Adafruit_TPA2016.cpp
 *
 *  @mainpage Adafruit TPA2016D2 breakout
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for Adafruit TPA2016D2 Class D Amplifier Breakout
 *
 * 	This is a library for the Adafruit TPA2016D2 breakout
 * 	http://www.adafruit.com/products/1712
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section author Author
 *
 *  Limor Fried/Ladyada (Adafruit Industries).
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#include <Adafruit_TPA2016.h>

Adafruit_TPA2016::Adafruit_TPA2016() {}

/*!
 *    @brief  Setups the i2c and calls powerUp function.
 *    @param  addr
 *            i2c address
 *    @param  theWire
 *            wire object
 *    @return True if initialization was successful, otherwise false.
 *
 */
boolean Adafruit_TPA2016::begin(uint8_t addr, TwoWire *theWire) {
  _i2caddr = addr;
  _wire = theWire;
  _wire->begin();

  return true;
}

/*!
 *    @brief  Set gain in dB by writing to TPA2016_GAIN.
 *    @param  g
 *            value in dB (clamped between -28 to 30)
 */
void Adafruit_TPA2016::setGain(int8_t g) {
  if (g > 30)
    g = 30;
  if (g < -28)
    g = -28;

  write8(TPA2016_GAIN, g);
}

/*!
 *    @brief  Gets the gain value by readig from TPA2016_GAIN.
 *    @return Returns gain value in dB
 */
int8_t Adafruit_TPA2016::getGain() { 
  int8_t gain = int8_t(read8(TPA2016_GAIN));
  gain = gain << 2;
  if ((gain & 0x80) > 0) gain = (gain >> 2) | 0xC0;   // it's a negative value
  else gain = gain >> 2;                              // it's a positive value
  return gain;
}

/*!
 *    @brief  Turns on / off right and left channels by writing to
 * TPA2016_SETUP.
 *    @param  r
 *            right channel
 *    @param  l
 *            left channel
 *
 */
void Adafruit_TPA2016::enableChannel(boolean r, boolean l) {

  uint8_t setup = read8(TPA2016_SETUP);
  if (r)
    setup |= TPA2016_SETUP_R_EN;
  else
    setup &= ~TPA2016_SETUP_R_EN;
  if (l)
    setup |= TPA2016_SETUP_L_EN;
  else
    setup &= ~TPA2016_SETUP_L_EN;

  write8(TPA2016_SETUP, setup);
}

/*!
 *    @brief  Sets AGC Compression by writing to TPA2016_AGC
 *    @param  x
 *            TPA2016_AGC_2 1:2
 *            TPA2016_AGC_4 1:4
 *            TPA2016_AGC_8 1:8
 */
void Adafruit_TPA2016::setAGCCompression(uint8_t x) {
  if (x > 3)
    return; // only 2 bits!

  uint8_t agc = read8(TPA2016_AGC);
  agc &= ~(0x03); // mask off bottom 2 bits
  agc |= x;       // set the compression ratio.
  write8(TPA2016_AGC, agc);
}

/*!
 *    @brief  Sets AGC Release time by writing to TPA2016_REL.
 *    @param  release
 *            release value (only 6 bits)
 */
void Adafruit_TPA2016::setReleaseControl(uint8_t release) {
  if (release > 0x3F)
    return; // only 6 bits!

  write8(TPA2016_REL, release);
}

/*!
 *    @brief  Sets AGC Attack time by writing to TPA2016_ATK.
 *    @param  attack
 *            attack value (only 6 bits)
 */
void Adafruit_TPA2016::setAttackControl(uint8_t attack) {
  if (attack > 0x3F)
    return; // only 6 bits!

  write8(TPA2016_ATK, attack);
}

/*!
 *    @brief  Sets AGC Hold time by writing to TPA2016_HOLD.
 *    @param  hold
 *            hold time value (only 6 bits)
 */
void Adafruit_TPA2016::setHoldControl(uint8_t hold) {
  if (hold > 0x3F)
    return; // only 6 bits!

  write8(TPA2016_HOLD, hold);
}

/*!
 *    @brief  Turns Power limiter on by writing to TPA2016_AGCLIMIT.
 */
void Adafruit_TPA2016::setLimitLevelOn() {
  uint8_t agc = read8(TPA2016_AGCLIMIT);
  agc &= ~(0x80); // mask off top bit
  write8(TPA2016_AGCLIMIT, agc);
}

/*!
 *    @brief  Turns Power limiter off by writing to TPA2016_AGCLIMIT.
 */
void Adafruit_TPA2016::setLimitLevelOff() {
  uint8_t agc = read8(TPA2016_AGCLIMIT);
  agc |= 0x80; // turn on top bit
  write8(TPA2016_AGCLIMIT, agc);
}

/*!
 *    @brief  Sets limit level by writing to TPA2016_AGCLIMIT.
 *    @param  limit
 *            value (max 31)
 */
void Adafruit_TPA2016::setLimitLevel(uint8_t limit) {
  if (limit > 31)
    return;

  uint8_t agc = read8(TPA2016_AGCLIMIT);

  agc &= ~(0x1F); // mask off bottom 5 bits
  agc |= limit;   // set the limit level.

  write8(TPA2016_AGCLIMIT, agc);
}

/*!
 *    @brief  Sets max gain by writing to TPA2016_AGC.
 *    @param  x
 *            value (max 12)
 */
void Adafruit_TPA2016::setAGCMaxGain(uint8_t x) {
  if (x > 12)
    return; // max gain max is 12 (30dB)

  uint8_t agc = read8(TPA2016_AGC);
  agc &= ~(0xF0);  // mask off top 4 bits
  agc |= (x << 4); // set the max gain
  write8(TPA2016_AGC, agc);
}

// Read 1 byte from the i2c bus at 'address'
uint8_t Adafruit_TPA2016::read8(uint8_t address) {
  _wire->beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(TPA2016_I2CADDR, 1);

  return Wire.read();
}

// write 1 byte
void Adafruit_TPA2016::write8(uint8_t address, uint8_t data) {
  Wire.beginTransmission(TPA2016_I2CADDR);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
