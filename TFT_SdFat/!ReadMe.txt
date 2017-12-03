Updated by Bodmer 25/10/15

This version of SdFat will only work with the Arduino at full speed 8MHz SPI rate.

The SdSpi::receive(uint8_t* buf, size_t n) function in the SdSpi.h header file
has been modified to use delays between SPI read transactions rather than SPIF
flag checks.  This speeds up block reads from the SD Card by about 12%.

This modification has been made to permit faster rendering of image files to TFT
displays.

The examples have been deleted since they were not updated to work with the renamed
library.

