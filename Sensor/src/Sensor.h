//
// Created by alex on 6/1/17.
//

#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"

class Sensor {
public:
    Sensor(int pin, double min_voltage, double max_voltage, double min_value, double max_value);

    float value();

private:
    int pin;
    double min_voltage;
    double max_voltage;
    double min_value;
    double max_value;
};


#endif //SENSOR_H
