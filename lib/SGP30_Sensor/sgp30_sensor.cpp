#include "sgp30_sensor.h"
#include <Wire.h>
#include <Adafruit_SGP30.h>
#include <M5Core2.h>

Adafruit_SGP30 sgp;

void initSGP30() {
    if (!sgp.begin()) {
        Serial.println("[SGP30] Sensor not found!");
        M5.Lcd.println("[SGP30] Not found!");
        while (1);
    }
    sgp.IAQinit();
    delay(1000);
    Serial.println("[SGP30] Sensor initialized");
}

void readSGP30() {
    if (!sgp.IAQmeasure()) {
        Serial.println("[SGP30] Measurement failed");
        M5.Lcd.println("[SGP30] Failed\n");

        return;
    }
    Serial.printf("[SGP30] eCO2: %d ppm | TVOC: %d ppb\n", sgp.eCO2, sgp.TVOC);
    M5.Lcd.printf("[SGP30]\neCO2: %d ppm\nTVOC: %d ppb\n\n", sgp.eCO2, sgp.TVOC);
}
