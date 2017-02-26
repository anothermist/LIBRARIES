void DS3234::_SPIstart(uint8_t rate)
{
	digitalWrite(SS, HIGH);
	pinMode(SS, OUTPUT);
	digitalWrite(_ss_pin, HIGH);
	pinMode(_ss_pin, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(MISO, INPUT);

	SPCR |= _BV(MSTR) | _BV(SPE) | _BV(CPOL) | _BV(CPHA);

	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

void DS3234::_SPIwrite(byte data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

byte DS3234::_SPIread(void)
{
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
