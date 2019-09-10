/*!
 *  @file Adafruit_FRAM_SPI.h
 *
 *  This is a library for the Adafruit SPI FRAM breakout.
 *
 *  Designed specifically to work with the Adafruit SPI FRAM breakout.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/1897
 *
 *  These module use SPI to communicate, 4 pins are required to interface.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  K.Townsend (Adafruit Industries)
 *
 *  BSD license, all text above must be included in any redistribution
 */
#ifndef _ADAFRUIT_FRAM_SPI_H_
#define _ADAFRUIT_FRAM_SPI_H_

#include <Arduino.h>

#include <SPI.h>

/** Operation Codes **/
typedef enum opcodes_e {
  OPCODE_WREN = 0b0110,    /* Write Enable Latch */
  OPCODE_WRDI = 0b0100,    /* Reset Write Enable Latch */
  OPCODE_RDSR = 0b0101,    /* Read Status Register */
  OPCODE_WRSR = 0b0001,    /* Write Status Register */
  OPCODE_READ = 0b0011,    /* Read Memory */
  OPCODE_WRITE = 0b0010,   /* Write Memory */
  OPCODE_RDID = 0b10011111 /* Read Device ID */
} opcodes_t;

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          FRAM SPI
 */
class Adafruit_FRAM_SPI {
public:
  Adafruit_FRAM_SPI(int8_t cs = -1, SPIClass *theSPI = &SPI);
  Adafruit_FRAM_SPI(int8_t clk, int8_t miso, int8_t mosi, int8_t cs);

  boolean begin(uint8_t nAddressSizeBytes = 2);
  void writeEnable(bool enable);
  void write8(uint32_t addr, uint8_t value);
  void write(uint32_t addr, const uint8_t *values, size_t count);
  uint8_t read8(uint32_t addr);
  void read(uint32_t addr, uint8_t *values, size_t count);
  void getDeviceID(uint8_t *manufacturerID, uint16_t *productID);
  uint8_t getStatusRegister(void);
  void setStatusRegister(uint8_t value);
  void setAddressSize(uint8_t nAddressSize);

  void SPI_TRANSACTION_START();
  void SPI_TRANSACTION_END();

private:
  uint8_t SPItransfer(uint8_t x);
  void writeAddress(uint32_t addr);

  boolean _framInitialised;
  uint8_t _nAddressSizeBytes;
  int8_t _cs, _clk, _mosi, _miso;
  SPIClass *_spi;

  SPISettings spiSettings;
};

#endif
