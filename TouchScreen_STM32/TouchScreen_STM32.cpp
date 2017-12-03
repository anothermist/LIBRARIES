// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

// Ported to STM32 by Jaret Burkett https://github.com/jaretburkett
// Simplified.

#include "wiring_private.h"
#include <WProgram.h>
#include "TouchScreen_STM32.h"

// increase or decrease the touch screen oversampling. This is a little different than you make think:
// 1 is no oversampling, whatever data we get is immediately returned
// 2 is double-sampling and we only return valid data if both points are the same
// 3+ uses insert sort to get the median value.
// We found 2 is precise yet not too slow so we suggest sticking with it!

#define NUMSAMPLES		7 // must be on odd number!
#define MIN_PRESSURE	100
#define X_RANGE		240
#define Y_RANGE		320

#define remapPins() { pinMode(_yp, OUTPUT); pinMode(_xm, OUTPUT); }

TSPoint p0;
int samples[NUMSAMPLES];
/*****************************************************************************/
TSPoint::TSPoint(void) {
  x = y = z = 0;
  x_old = 0; y_old = 0;
  repeat = 0;
  dbl = 0;
}
/*****************************************************************************/
TSPoint::TSPoint(int16_t x0, int16_t y0) {
  x = x0;
  y = y0;
  x_old = 0; y_old = 0;
  repeat = 0;
  dbl = 0;
}
/*****************************************************************************/
TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym)
{
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	pressureThreshhold = MIN_PRESSURE;
	// set default values
	x_range = X_RANGE-1;
	y_range = Y_RANGE-1;
	xmax = ymax = 0; xmin = ymin = 1023;
}
/*****************************************************************************/
void TouchScreen::rangeSet(int xr, int yr)
{
	x_range = xr-1;	// 240
	y_range = yr-1;	// 320
}
/*****************************************************************************/
void TouchScreen::calibratePoint(void)
{
	while ( !getPoint() ) delay(50); // wait till user touches the surface
	do {
		if (p0.x>xmax) xmax = p0.x;
		if (p0.x<xmin) xmin = p0.x;
		if (p0.y>ymax) ymax = p0.y;
		if (p0.y<ymin) ymin = p0.y;
		delay(50); // wait till user releases the surface
	} while ( getPoint() );
}
/*****************************************************************************/
void sortArray(int pos)
{
	// it will sort the position in the array from lower to higher value order
	while ( (pos--)>0 && (samples[pos]<samples[pos-1]) ) {
		// swap array values
		int tmp = samples[pos-1];
		samples[pos-1] = samples[pos];
		samples[pos] = tmp;
	}
}
/*****************************************************************************/
int TouchScreen::readADC(uint8_t pin)
{
	for (int i=0; i<NUMSAMPLES; i++) {
		samples[i] = analogRead(pin)/4; // to reduce deviation 
		sortArray(i);
	}
	return samples[(NUMSAMPLES-1)/2]; // get the median value
}
/*****************************************************************************/
void TouchScreen::readTouchX(void)
{
	// read pressure at the beginning to ensure that display is touched
	if (readPressure()<pressureThreshhold) {
		p0.x = 0;
		return;
	}
	pinMode(_xp, OUTPUT);
	pinMode(_xm, OUTPUT);
	digitalWrite(_xp, HIGH);
	digitalWrite(_xm, LOW);
	pinMode(_ym, INPUT);
	pinMode(_yp, INPUT_ANALOG);
	p0.x = readADC(_yp);
	// read pressure at the end to ensure that in between was always touched
	if (readPressure()<pressureThreshhold) {
		p0.x = 0;
	}
}
/*****************************************************************************/
void TouchScreen::readTouchY(void)
{
	// read pressure at the beginning to ensure that display is touched
	if (readPressure()<pressureThreshhold) {
		p0.y = 0;
		return;
	}
	pinMode(_yp, OUTPUT);
	pinMode(_ym, OUTPUT);
	digitalWrite(_yp, HIGH);
	digitalWrite(_ym, LOW);
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT_ANALOG);
	p0.y = readADC(_xm);
	// read pressure at the end to ensure that in between was always touched
	if (readPressure()<pressureThreshhold) {
		p0.y = 0;
	}
}
/*****************************************************************************/
// returns the difference of ADC readings
/*****************************************************************************/
int TouchScreen::readPressure(void)
{
	// Set X+ to ground
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, LOW);
	// Set Y- to VCC
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, HIGH);
	// Hi-Z X- and Y+
	pinMode(_xm, INPUT);
	pinMode(_yp, INPUT);
	// read ADC
	int z1 = readADC(_xm);
	int z2 = readADC(_yp);
	p0.z = 1023-z2+z1;
	return p0.z;
}
/*****************************************************************************/
bool TouchScreen::getPoint(void)
{
	readTouchX();	// get X coordinate
	readTouchY();	// get Y coordinate
	// the coordinates are only valid when both pressure values are OK
	if ( p0.x==0 || p0.y==0 ) {
		p0.z = 0;
	}
	remapPins();
	return p0.z;
}
/*****************************************************************************/
bool TouchScreen::getPoint(TSPoint* tsp)
{
	if ( getPoint()==0 )
		return false;
	// valid touch detected
	uint32_t crt_mic = micros();
/**/
	// map coordinates to display ranges
	int x0 = map(p0.x, xmin, xmax, 0, x_range);
	tsp->x = x0;
	int y0 = map(p0.y, ymin, ymax, 0, y_range);
	tsp->y = y0;
	tsp->z = p0.z;
	
	// check repetition
	if ( p0.repeat==0 ) {
		x_rep = p0.x_old; y_rep = p0.y_old;	// initial repeat position
	}
#define REPEAT_POS_RANGE 2
	if ( x0>=(x_rep-REPEAT_POS_RANGE) && x0<=(x_rep+REPEAT_POS_RANGE) && y0>=(y_rep-REPEAT_POS_RANGE) && y0<=(y_rep+REPEAT_POS_RANGE) ) {
		//Serial.println("*** REPEAT ***");
		p0.repeat++;
	} else {
		p0.repeat = 0;
	}
	tsp->repeat = p0.repeat/50; // experimental value, to be adjusted dependent on the LCD processing speed

	// check double click
#define DBL_POS_RANGE 15
#define DBL_GAP_LOW  100000 // micros
#define DBL_GAP_HIGH 300000 // micros
	if ( (crt_mic-last_touch_time)>DBL_GAP_LOW && (crt_mic-last_touch_time)<DBL_GAP_HIGH ) {
		//Serial.println("dbl gap");
		if ( x0>=(p0.x_old-DBL_POS_RANGE) && x0<=(p0.x_old+DBL_POS_RANGE) && y0>=(p0.y_old-DBL_POS_RANGE) && y0<=(p0.y_old+DBL_POS_RANGE) ) {
			//Serial.println("*** DOUBLE CLICK ***");
			p0.dbl++;
		}
	} else {
		p0.dbl = 0;
	}
	tsp->dbl = p0.dbl;

	// update old values
	p0.x_old = x0;
	p0.y_old = y0;
	last_touch_time = crt_mic;
	
	return true;
}
