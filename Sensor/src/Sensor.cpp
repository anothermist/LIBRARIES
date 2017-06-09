#include "Arduino.h"
#include "Sensor.h"

#define NUMSAMPLES 5

Sensor::Sensor(int pin, double min_voltage, double max_voltage, double min_value, double max_value)
        : pin(pin), min_voltage(min_voltage), max_voltage(max_voltage), min_value(min_value), max_value(max_value) {}


float Sensor::value() {
  float value;
  for (unsigned int i = 0; i < NUMSAMPLES; i++) {
    value += analogRead(pin);
  }

  value /= NUMSAMPLES;

  return map(value, min_voltage, max_voltage, min_value, max_value);
}
