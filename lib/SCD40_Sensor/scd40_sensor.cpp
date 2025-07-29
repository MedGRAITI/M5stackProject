/*      
Nom du fichier : scd40_sensor.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur scd40
Description :   Fichier cpp de capteur scd40 Communication I2C, adresse par defaut : 0x62, Affiche des valeur
                dans le terminal et aussi dans LCD de M5Stack.
*/ 

#include "scd40_sensor.h"
#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include <M5Core2.h>

SensirionI2cScd4x scd4x;

void initSCD40() {  // Initialisation de capteur,
    scd4x.begin(Wire,0x62); // Appele de Wire(SDA,SCL) avec adresse de I2c
    scd4x.stopPeriodicMeasurement();
    delay(500);
    scd4x.startPeriodicMeasurement();
}

bool readSCD40(uint16_t& co2, float& temp, float& humidity) {  // fonction de la lecture de DATA
    bool isDataReady = false;
    scd4x.getDataReadyStatus(isDataReady);
    if (!isDataReady) return false;

    uint16_t error = scd4x.readMeasurement(co2, temp, humidity);

    if (error) {
        Serial.println("[SCD40] Read failed");
        //M5.Lcd.println("[SCD40] Failed\n");
        return false;
    } else {
        Serial.printf("[SCD40] CO2: %d ppm | Temp: %.2f °C | RH: %.2f %%\n", co2, temp, humidity);
        //M5.Lcd.printf("[SCD40]\nCO2: %d ppm\nTemp: %.2f °C\nRH: %.2f %%\n\n", co2, temp, humidity);
        return true;
    }
}
