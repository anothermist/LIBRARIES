void DS3234::_SPIstart(uint8_t rate)
{
	digitalWrite(SS, HIGH);
	pinMode(SS, OUTPUT);
	digitalWrite(_ss_pin, HIGH);
	pinMode(_ss_pin, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(MISO, INPUT);

	PIO_Configure(g_APinDescription[PIN_SPI_MOSI].pPort, g_APinDescription[PIN_SPI_MOSI].ulPinType, g_APinDescription[PIN_SPI_MOSI].ulPin, g_APinDescription[PIN_SPI_MOSI].ulPinConfiguration);
	PIO_Configure(g_APinDescription[PIN_SPI_MISO].pPort, g_APinDescription[PIN_SPI_MISO].ulPinType, g_APinDescription[PIN_SPI_MISO].ulPin, g_APinDescription[PIN_SPI_MISO].ulPinConfiguration);
	PIO_Configure(g_APinDescription[PIN_SPI_SCK].pPort,  g_APinDescription[PIN_SPI_SCK].ulPinType,  g_APinDescription[PIN_SPI_SCK].ulPin,  g_APinDescription[PIN_SPI_SCK].ulPinConfiguration);
	pmc_enable_periph_clk(ID_SPI0);

	pmc_enable_periph_clk(ID_DMAC);
	DMAC->DMAC_EN &= (~DMAC_EN_ENABLE);
	DMAC->DMAC_GCFG = DMAC_GCFG_ARB_CFG_FIXED;
	DMAC->DMAC_EN = DMAC_EN_ENABLE;

	Spi* pSpi = SPI0;
	uint8_t scbr = 255;
	if (rate < 14)
	{
		scbr = (2 | (rate & 1)) << (rate/2);
	}
	scbr = rate;
	pSpi->SPI_CR = SPI_CR_SPIDIS;
	pSpi->SPI_CR = SPI_CR_SWRST;
	pSpi->SPI_MR = SPI_PCS(SPI_CHIP_SEL) | SPI_MR_MODFDIS | SPI_MR_MSTR;
	pSpi->SPI_CSR[SPI_CHIP_SEL] = SPI_CSR_SCBR(scbr) | SPI_CSR_CPOL;
	pSpi->SPI_CR |= SPI_CR_SPIEN;

}

void DS3234::_SPIwrite(byte data)
{
	Spi* pSpi = SPI0;

	pSpi->SPI_TDR = data;
	while ((pSpi->SPI_SR & SPI_SR_RDRF) == 0) {}
	data = pSpi->SPI_RDR;
}

byte DS3234::_SPIread(void)
{
	Spi* pSpi = SPI0;

	pSpi->SPI_TDR = 0xFF;
	while ((pSpi->SPI_SR & SPI_SR_RDRF) == 0) {}
	return pSpi->SPI_RDR;
}
