/* #include "scd30_sensor.h"
#include <Wire.h>
#include <SensirionI2CScd30.h>
#include <M5Core2.h>

SensirionI2cScd30 scd30;


void initSCD30() {
    scd30.begin(Wire,0x61);
    scd30.startPeriodicMeasurement(1025);
}

void readSCD30() {
    float co2;
    float temp, humidity;

    if (scd30.readMeasurementData(co2, temp, humidity) == 0) {
        Serial.printf("[SCD30] CO2: %d ppm | Temp: %.1f °C | RH: %.1f %%\n", co2, temp, humidity);
        M5.Lcd.printf("[SCD30]\nCO2: %d ppm\nTemp: %.1f °C\nRH: %.1f %%\n\n", (int)co2, temp, humidity);
    } else {
        Serial.println("[SCD30] Failed to read measurement.");
        M5.Lcd.println("[SCD30] Failed");
    }
}
  */
#include "scd30_sensor.h"
#include <Wire.h>
#include <SensirionI2cScd30.h>
#include <M5Core2.h>

SensirionI2cScd30 scd30;

void initSCD30() {
  scd30.begin(Wire, SCD30_I2C_ADDR_61);
  scd30.stopPeriodicMeasurement();
  scd30.softReset();
  delay(2000);
  scd30.startPeriodicMeasurement(0);
}

bool readSCD30(float& co2, float& temperature, float& humidity) {
  uint16_t dataReady = 0;
  scd30.getDataReady(dataReady);
  if (!dataReady) return false;

  int16_t error = scd30.readMeasurementData(co2, temperature, humidity);
  if (error == 0 && co2 > 0 && co2 < 10000) {
    Serial.printf("[SCD30] CO2: %.1f ppm | Temp: %.1f C | RH: %.1f %%\n", co2, temperature, humidity);
    M5.Lcd.printf("[SCD30]\nCO2: %.1f ppm\nTemp: %.1f C\nRH: %.1f %%\n\n", co2, temperature, humidity);
    return true;
  }
  return false;
}



