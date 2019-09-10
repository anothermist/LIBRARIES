/*
=================================================
example to configure and get data from AD7173 ADC
=================================================
*/
#include <AD7173.h>
#define ENABLED_CH 3 //number of channel

void setup() {
	afio_cfg_debug_ports(AFIO_DEBUG_NONE);// Stop the debug function
	pinMode(PA4, OUTPUT);
	digitalWrite(PA4,LOW);// enable device

	/* initiate serial communication */
	Serial.begin(230400);

	/* initialize SPI connection to the ADC */
	AD7173.init();

	/* sync the ADC */
	AD7173.sync();

	/* reset the ADC registers to default */
	AD7173.reset();

	/* check if the ID register of the ADC is valid */
	if (AD7173.is_valid_id()) Serial.println("AD7173 ID is valid");
	else Serial.println("AD7173 ID is invalid");

	/* set ADC input channel configuration */
	/* enable channel 0 and channel 1 and connect each to 2 analog inputs for bipolar input */
	/* CH0 - CH15 */
	/* true/false to enable/disable channel */
	/* SETUP0 - SETUP7 */
	/* AIN0 - AIN16 */
	// AIN16 is connected to GND 
	AD7173.set_channel_config(CH0, true, SETUP0, AIN2, AIN16);
	AD7173.set_channel_config(CH1, true, SETUP0, AIN3, AIN16);
	AD7173.set_channel_config(CH3, true, SETUP0, AIN4, AIN16);

	/* set the ADC SETUP0 coding mode to BIPLOAR output */
	/* SETUP0 - SETUP7 */
	/* BIPOLAR, UNIPOLAR */
	/* AIN_BUF_DISABLE, AIN_BUF_ENABLE */
	/* REF_EXT, REF_AIN, REF_INT, REF_PWR */
	AD7173.set_setup_config(SETUP0, UNIPOLAR, AIN_BUF_ENABLE, REF_INT);

	/* set ADC OFFSET0 offset value */
	/* OFFSET0 - OFFSET7 */
	AD7173.set_offset_config(OFFSET0, 0);

	/* set the ADC FILTER0 ac_rejection to false and samplingrate to 1007 Hz */
	/* FILTER0 - FILTER7 */
	/* SPS_1, SPS_2, SPS_5, SPS_10, SPS_16, SPS_20, SPS_49, SPS_59, SPS_100, SPS_200 */
	/* SPS_381, SPS_503, SPS_1007, SPS_2597, SPS_5208, SPS_10417, SPS_15625, SPS_31250 */
	AD7173.set_filter_config(FILTER0, SPS_1007);

	/* set the ADC data and clock mode */
	/* CONTINUOUS_CONVERSION_MODE, SINGLE_CONVERSION_MODE */
	/* in SINGLE_CONVERSION_MODE after all setup channels are sampled the ADC goes into STANDBY_MODE */
	/* to exit STANDBY_MODE use this same function to go into CONTINUOUS or SINGLE_CONVERSION_MODE */
	/* INTERNAL_CLOCK, INTERNAL_CLOCK_OUTPUT, EXTERNAL_CLOCK_INPUT, EXTERNAL_CRYSTAL */
	/* REF_DISABLE, REF_ENABLE */
	AD7173.set_adc_mode_config(CONTINUOUS_CONVERSION_MODE, INTERNAL_CLOCK, REF_ENABLE);

	/* enable/disable CONTINUOUS_READ_MODE and appending STATUS register to data */
	/* to exit CONTINUOUS_READ_MODE use AD7173.reset(); */
	/* AD7173.reset(); returns all registers to default state, so everything has to be setup again */
	/* true / false to enable / disable appending status register to data, 4th byte */
	AD7173.set_interface_mode_config(false, true);

	/* wait for ADC */
	delay(10);
}

/* ADC conversion data and STATUS register */
byte data[ENABLED_CH][4];

void loop() {
	/* when ADC conversion is finished */
	for (int i = 0; i <= (ENABLED_CH-1); i++) {
		while (digitalRead(PA6) == HIGH) {
		}
		/* get ADC conversion result */
		AD7173.get_data(data[i], true);
	}

	for (int i = 0; i <= (ENABLED_CH-1); i++) {
		char format[10];
		for (int j = 0; j <= 3; j++) {
		    sprintf(format, "0x%.2X ", data[i][j]);
		    Serial.print(format);
		  }
		Serial.println();
	}

	delay(1000);

}
