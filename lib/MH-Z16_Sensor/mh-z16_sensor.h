/* #pragma once

void initMHZ16();
void readMHZ16(); */
#ifndef MHZ16_SENSOR_H
#define MHZ16_SENSOR_H

#include <Arduino.h>
#include <HardwareSerial.h>

class Mhz16 {
  private:
    HardwareSerial& serial;
    uint8_t gasConcentration[9] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
    uint8_t zeroPoint[9]         = { 0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78 };

  public:
    Mhz16(HardwareSerial& s) : serial(s) {}
    void begin(unsigned long baud);
    int readCO2();
    void calibrateZeroPoint();
};

#endif