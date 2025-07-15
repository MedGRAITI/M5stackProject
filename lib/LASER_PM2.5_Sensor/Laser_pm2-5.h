#pragma once
#include <Arduino.h>

bool initHM3301();
bool readHM3301(uint16_t& pm1, uint16_t& pm2_5, uint16_t& pm10);
