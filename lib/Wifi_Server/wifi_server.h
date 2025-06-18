#pragma once
#include <stdint.h>

void setupWiFi();
void handleClient();

// pour envoyer les valeurs à afficher
void updateSensorValues(
    float scd30_co2, float scd30_temp, float scd30_hum,
    uint16_t scd40_co2, float scd40_temp, float scd40_hum,
    uint16_t sgp30_eco2, uint16_t sgp30_tvoc,
    int mhz16_co2
);
