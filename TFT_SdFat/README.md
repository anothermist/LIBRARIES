# TFT_SdFat
A tweaked version of SdFat to improve block read speed

This version of SdFat will only work with the Arduino at full speed 8MHz SPI rate.

The SdSpi::receive(uint8_t* buf, size_t n) function in the SdSpi.h header file
has been modified to use delays between SPI read transactions rather than SPIF
flag checks.  This speeds up block reads from the SD Card by about 15%.

Other minor tweaks were made to functions in the same file to synchronise the
reading of the SPIF flag to just after the time it changes, (SPIF check loop
only samples every 4 clock cycles) this speeds things up a little bit.

This modification has been made to permit faster rendering of image files to TFT
displays. When used in conjunction with my TFT_ILI9341 library a 320 x 240 image
can be pulled off an SD Card by a humble Arduino UNO (or Mega) and displayed in
just 405ms (includes time to read image and to display it!).

The examples have been deleted since they were not updated to work with the renamed
library.
