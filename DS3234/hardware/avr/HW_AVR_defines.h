// *** Hardware SPI-specific defines
#define SPI_CLOCK_DIV2		0x04
#define SPI_CLOCK_DIV4		0x00
#define SPI_CLOCK_DIV8		0x05
#define SPI_CLOCK_DIV16		0x01
#define SPI_CLOCK_DIV32		0x06
#define SPI_CLOCK_DIV64		0x02
#define SPI_CLOCK_DIV128	0x03

#define SPI_CLOCK_MASK		0x03
#define SPI_2XCLOCK_MASK	0x01

#define SPI_CLOCK_DEFAULT	0x04  // 0x04
