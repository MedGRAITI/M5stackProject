// HM330X.h - Classe capteur PM2.5 HM3301
#pragma once
#include <Arduino.h>
#include <Wire.h>

#define HM330X_I2C_ADDR 0x40
#define HM330X_FRAME_LENGTH 29

class HM330X {
public:
    int init();
    int read_sensor_value(uint8_t* data, int len);
};
