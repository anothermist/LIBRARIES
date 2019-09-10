# AD7173-Arduino
Analog Devices AD7173 analog digital converter Arduino library

Mostly tested setup for this library:

* 1007 data rate
* external crystal
* continuous conversion mode
* 4 analog inputs, 2 bipolar channels

```c
AD7173.set_channel_config(CH0, true, SETUP0, AIN8, AIN9);
AD7173.set_channel_config(CH1, true, SETUP0, AIN10, AIN11);
AD7173.set_setup_config(SETUP0, BIPOLAR);
AD7173.set_filter_config(FILTER0, SPS_1007);
AD7173.set_adc_mode_config(CONTINUOUS_CONVERSION_MODE, EXTERNAL_CRYSTAL);
```
