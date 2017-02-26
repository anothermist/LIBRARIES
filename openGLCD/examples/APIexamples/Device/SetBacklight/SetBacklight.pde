/*
 * SetBacklight - openGLCD API example
 *
 * Example of using:
 * SetBacklight()
 *
 * status = SetBacklight(val);
 * val    - brightness level
 *
 * SetBacklight() Sets the backlight brightness
 *
 * If the display circuitry only has the option to turn the 
 * backlight on and off  then any non zero value will turn the display on
 * with maximum brightness
 * The value HIGH should not be used to turn the backlight on since
 * this function is used to control backlight intensity and not
 * for on / off control.
 * A value of HIGH is typically 1 which would be a very dim backlight.
 *
 * Returns:
 *    0 or GLCD_NOERR when successful or non zero error code when unsucessful
 *
 * Note: Backlight control requires additional circuitry
 *       It also requires that the glcdPinBL pin be enabled in the config file
 *       and that whatever pin is used for glcdPinBL be connected to the
 *       BL input of backlight circuit.
 *
 * Example Backlight Circuits:
 *
 * N-CH Mosfet version: (More costly but less power draw and lower part count)
 * ---------------------------------------------------------------------------
 *
 *                (value depends on GLCD, 100ohm is usually safe)
 * (LCD BL anode)---[ resistor ]---(VCC)
 *
 * (LCD BL cathode)---------------+
 *                                |
 *                                D
 *                                |
 * (BL input)----------------G-|-< (2N7000 FET)
 *                                |
 *                                S
 *                                |
 *                              (GND)
 *
 * NOTE: Gate resistor not needed as the mosfet is voltage fed and only really
 *       pulls current while switching.
 *
 * Here are two example backlight circuits that can be used:
 *
 *
 * NPN Transistor version: (Cheaper but more power draw and higher part count)
 * ---------------------------------------------------------------------------
 *
 *                (value depends on GLCD, 100ohm is usually safe)
 * (LCD BL anode)---[ resistor ]---(VCC)
 *
 * (LCD BL cathode)---------------+
 *                                |
 *                                C
 *                                |
 * (BL input)--[ Resistor ]---B-|> (NPN) 2N2222A/2N3904 etc...
 *                  1k            |
 *                                E
 *                                |
 *                              (GND)
 *
 * NOTE: The Base resistor is needed because the NPN is current fed.  For lower
 *       power draw, try a 10k resistor.
 *
 * In either case, when the BL input is HIGH the LCD backlight will turn on.
 * 
 * Created by Bill Perry 2013-08-01
 * bperrybap@opensource.billsworld.billandterrie.com
 */

#include <openGLCD.h>

void setup()
{

	GLCD.Init();
	GLCD.SelectFont(System5x7);
}

void loop(void)
{

	GLCD.DrawString("SetBacklight", gTextfmt_center, gTextfmt_center);
	int status = GLCD.OnBacklight(); // turn on full bight backlight (if configured)
	if(status != GLCD_ENOERR)
	{
		// failed, code here handles error, if needed
		// status GLCD_ENOTSUPPORTED if not configured
		GLCD.DrawString("(unsupported)", gTextfmt_center, gTextfmt_bottom);
	}
	else
	{
		// only do intensity test if backlight control works
		delay(1000);

		/*
		 * Ramp the intensity down, then back up
		 */

		for(int times = 0; times < 3; times++)
		{
			for(int x = 1; x < 16; x++)
			{
				status = GLCD.SetBacklight(256-x * 16);
				if(status != GLCD_ENOERR)
				{
					// failed, code here handles error, if needed
					// status GLCD_ENOTSUPPORTED if not configured
				}
				delay(45);
			}
			for(int x = 1; x < 16; x++)
			{
				status = GLCD.SetBacklight(x * 16);
				if(status != GLCD_ENOERR)
				{
					// failed, code here handles error, if needed
					// status GLCD_ENOTSUPPORTED if not configured
				}
				delay(45);
			}
		}
	}
	delay(1000);
}
