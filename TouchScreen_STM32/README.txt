
This is another version of the original Adafruit library, ported for STM32.
Not perfect but is easy to use, very fast and reliable.
Tested to work in conjunction with the TFTLCD library (8bit parallel) ported for STM32:
https://github.com/stevstrong/Adafruit_TFTLCD_8bit_STM32
An example how to use can be found here:
https://github.com/stevstrong/Adafruit_TFTLCD_8bit_STM32/blob/master/examples/tftpaint/tftpaint.ino


Features:
- mapping of the coordinate ranges to the display resolution (default: 240*320)
- semi-automatic (user-assisted) easy calibration process (requires touching two opposite corners).
- repeated touch
- double click/touch
