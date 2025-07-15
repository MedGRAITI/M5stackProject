#include "Laser_pm2-5.h"
#include "HM330X.h"
#include <M5Core2.h>

HM330X hm330x;

bool initHM3301() {
    int res = hm330x.init();
    if (res == 0) {
        Serial.println("[HM3301] Capteur initialisé");
        return true;
    } else {
        Serial.printf("[HM3301] Erreur init : %d\n", res);
        return false;
    }
}

bool readHM3301(uint16_t& pm1, uint16_t& pm2_5, uint16_t& pm10) {
    uint8_t buffer[29];
    if (hm330x.read_sensor_value(buffer, 29) != 0) {
        Serial.println("[HM3301] Lecture échouée");
        return false;
    }

    pm1 = (buffer[6] << 8) | buffer[7];
    pm2_5 = (buffer[8] << 8) | buffer[9];
    pm10 = (buffer[10] << 8) | buffer[11];

    Serial.printf("[HM3301] PM1.0: %u | PM2.5: %u | PM10: %u\n", pm1, pm2_5, pm10);
    M5.Lcd.printf("[HM3301]\nPM1: %u\nPM2.5: %u\nPM10: %u\n\n", pm1, pm2_5, pm10);

    return true;
}
