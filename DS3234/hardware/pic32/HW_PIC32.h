void DS3234::_SPIstart(uint8_t rate)
{
	static p32_spi *	spi = (p32_spi *)_SPI2_BASE_ADDRESS;
	static int			irq = _SPI2_ERR_IRQ;
	static p32_regset *	iec = ((p32_regset *)&IEC0) + (irq / 32);
	static p32_regset *	ifs = ((p32_regset *)&IFS0) + (irq / 32);
	static int			vec = _SPI_2_VECTOR;
	p32_regset *		ipc = ((p32_regset *)&IPC0) + (vec / 4);
	int					ipl_shift = 8 * (vec % 4);

	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);
	pinMode(_ss_pin, OUTPUT);
	digitalWrite(_ss_pin, HIGH);

	iec->clr = 0x07 << (irq % 32);
	spi->sxCon.reg = 0;
	uint8_t tmp = spi->sxBuf.reg;
	ifs->clr = 0x07 << (irq % 32);
	ipc->clr = (0x1F << ipl_shift);
	ipc->set = ((_SPI2_IPL_IPC << 2) + _SPI2_SPL_IPC) << ipl_shift;
	spi->sxBrg.reg = rate;
	spi->sxStat.clr = (1 << _SPISTAT_SPIROV);
	spi->sxCon.set = (1 << _SPICON_MSTEN) + SPI_MODE3;
	spi->sxCon.set = (1 << _SPICON_ON);
}

void DS3234::_SPIwrite(byte data)
{
	static p32_spi * spi = (p32_spi *)_SPI2_BASE_ADDRESS;

	while ((spi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0 );
	spi->sxBuf.reg = data;

	while ((spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0 );
	uint8_t tmp = spi->sxBuf.reg;
}

byte DS3234::_SPIread(void)
{
	static p32_spi * spi = (p32_spi *)_SPI2_BASE_ADDRESS;

	while ((spi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0 );
	spi->sxBuf.reg = 0;

	while ((spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0 );
	return spi->sxBuf.reg;
}

