/*      
Nom du fichier : sgp30_sensor.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,  Capteur sgp30
Description : Fichier Header de capteur sgp30 Communication I2C, Affiche des valeur
              dans le terminal et aussi dans LCD de M5Stack.
*/ 

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
    sgp.setHumidity(0);
    Serial.println("[SGP30] Sensor initialized");
}

bool readSGP30(uint16_t& eco2, uint16_t& tvoc) {
    if (!sgp.IAQmeasure()) {
        Serial.println("[SGP30] Measurement failed");
        M5.Lcd.println("[SGP30] Failed\n");

        return false;
    }
    eco2 = sgp.eCO2;
    tvoc = sgp.TVOC;
    if (eco2 == 400 && tvoc == 0) {
        Serial.println("[SGP30] Still warming up...");
        return false;
    }
    Serial.printf("[SGP30] eCO2: %d ppm | TVOC: %d ppb\n", eco2, tvoc);
    M5.Lcd.printf("[SGP30]\neCO2: %d ppm\nTVOC: %d ppb\n\n", eco2, tvoc);
    return true;
}