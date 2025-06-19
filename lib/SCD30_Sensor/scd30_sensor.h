#ifndef SCD30_SENSOR_H
#define SCD30_SENSOR_H

#include <Arduino.h>

void initSCD30();
bool readSCD30(float& co2, float& temperature, float& humidity);

#endif  // SCD30_SENSOR_H
