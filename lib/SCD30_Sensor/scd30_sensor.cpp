/*      
Nom du fichier : scd30_sensor.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur scd30
Description : Fichier cpp de capteur scd30 Communication I2C,  Adresse par defaut 0x61
*/ 

#include "scd30_sensor.h"
#include <Wire.h>
#include <SensirionI2cScd30.h>
#include <M5Core2.h>

SensirionI2cScd30 scd30;        //classe Sensirion de la bibliotheque de SensirionI2cScd30.h

void initSCD30() {
  scd30.begin(Wire, SCD30_I2C_ADDR_61);  //begin deux parametre : Wire (SDA,SCK) de M5STACK et Adresse I2c de mon capteur
  scd30.stopPeriodicMeasurement();
  scd30.softReset();
  delay(2000);
  scd30.startPeriodicMeasurement(0);
}

bool readSCD30(float& co2, float& temperature, float& humidity) { //fonction de lecture de DATA 3 parametre CO2, temp, hum
  uint16_t dataReady = 0;
  scd30.getDataReady(dataReady);
  if (!dataReady) return false;

  int16_t error = scd30.readMeasurementData(co2, temperature, humidity);
  if (error == 0 && co2 > 0 && co2 < 10000) {
    Serial.printf("[SCD30] CO2: %.1f ppm | Temp: %.1f C | RH: %.1f %%\n", co2, temperature, humidity);
    //M5.Lcd.printf("[SCD30]\nCO2: %.1f ppm\nTemp: %.1f C\nRH: %.1f %%\n\n", co2, temperature, humidity);
    return true;
  }
  return false;
}



