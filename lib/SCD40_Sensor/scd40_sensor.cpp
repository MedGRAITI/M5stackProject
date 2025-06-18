#include "scd40_sensor.h"
#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include <M5Core2.h>

SensirionI2cScd4x scd4x;

void initSCD40() {
    scd4x.begin(Wire,0x62);
    scd4x.stopPeriodicMeasurement();
    delay(500);
    scd4x.startPeriodicMeasurement();
}

void readSCD40() {
    bool isDataReady = false;
    scd4x.getDataReadyStatus(isDataReady);
    if (!isDataReady) return;

    uint16_t co2;
    float temp, humidity;
    uint16_t error = scd4x.readMeasurement(co2, temp, humidity);

    if (error) {
        Serial.println("[SCD40] Read failed");
        M5.Lcd.println("[SCD40] Failed\n");
    } else {
        Serial.printf("[SCD40] CO2: %d ppm | Temp: %.2f °C | RH: %.2f %%\n", co2, temp, humidity);
        M5.Lcd.printf("[SCD40]\nCO2: %d ppm\nTemp: %.2f °C\nRH: %.2f %%\n\n", co2, temp, humidity);
    }
}
