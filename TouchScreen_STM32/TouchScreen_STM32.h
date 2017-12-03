// Touch screen library with X, Y and Z (pressure) readings
// includes simple averaging samples to de-bounce

#ifndef _ADAFRUIT_TOUCHSCREEN_STM_H_
#define _ADAFRUIT_TOUCHSCREEN_STM_H_

#include <stdint.h>

class TSPoint {
 public:
	TSPoint(void);
	TSPoint(int16_t x, int16_t y);
	int x, y, z, x_old, y_old, repeat, dbl;
};

class TouchScreen {
  public:
	TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym);

	void rangeSet(int xmax, int ymax);
	bool getPoint(TSPoint* tsp);
	void calibratePoint(void);
	int pressureThreshhold;

  private:
	bool getPoint(void);
	int readADC(uint8_t pin);
	int readPressure(void);
	void readTouchY(void);
	void readTouchX(void);
	void waitForTouch(void);
	void waitForRelease(void);
	uint8_t _yp, _ym, _xm, _xp;	// resistive plate pins
	int xmax, ymax, xmin, ymin, x_range, y_range;
	int x_rep, y_rep;
	uint32_t last_touch_time;
};

#endif
