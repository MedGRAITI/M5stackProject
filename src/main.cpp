/*      
Nom du fichier : main.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2, MHZ16, SCD30, SCD40, SGP30,
Description : Fichier main de mon projet
*/ 
#include <Arduino.h>
#include <Wire.h>
#include <M5Core2.h>
#include <FS.h>
#include <SPIFFS.h>

#include "scd30_sensor.h"
#include "scd40_sensor.h"
#include "sgp30_sensor.h"
#include "mh-z16_sensor.h"
#include "wifi_server.h"


HardwareSerial mhzSerial(2);
Mhz16 mhz16(mhzSerial);

float scd30_co2 = 0.0, scd30_temp = 0.0, scd30_hum = 0.0;
uint16_t scd40_co2 = 0;
float scd40_temp = 0.0, scd40_hum = 0.0;
uint16_t sgp30_eco2 = 0, sgp30_tvoc = 0;
int mhz16_co2 = 0;

extern bool measurementActive;

void setup() {
  M5.begin();
  Serial.begin(115200);
  Wire.begin();
  setupWiFi();
  initSCD30();
  initSCD40();
  initSGP30();
  mhz16.begin(9600);

  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);

  File f = SPIFFS.open("/mesures.csv", FILE_WRITE);
  if (f.size() == 0) {
    f.println("time;scd30_co2;scd30_temp;scd30_hum;scd40_co2;scd40_temp;scd40_hum;sgp30_eco2;sgp30_tvoc;mhz16_co2");
  }
  f.close();
}

/* void loop() {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);

  if (readSCD30(scd30_co2, scd30_temp, scd30_hum)){
    M5.Lcd.printf("[SCD30]\nCO2: %.1f ppm\nTemp: %.1f °C\nRH: %.1f %%\n\n", scd30_co2, scd30_temp, scd30_hum);
    Serial.printf("[SCD30] CO2: %.1f ppm | Temp: %.1f °C | RH: %.1f %%\n", scd30_co2, scd30_temp, scd30_hum);
  } else {
    M5.Lcd.println("[SCD30] Erreur\n");
    Serial.println("[SCD30] Lecture échouée");
  }

  if (readSCD40(scd40_co2, scd40_temp, scd40_hum)) {
    M5.Lcd.printf("[SCD40]\nCO2: %d ppm\nTemp: %.2f °C\nRH: %.2f %%\n\n", scd40_co2, scd40_temp, scd40_hum);
    Serial.printf("[SCD40] CO2: %d ppm | Temp: %.2f °C | RH: %.2f %%\n", scd40_co2, scd40_temp, scd40_hum);
  } else {
    M5.Lcd.println("[SCD40] Erreur\n");
    Serial.println("[SCD40] Lecture échouée");
  }

  if (readSGP30(sgp30_eco2, sgp30_tvoc)) {
    M5.Lcd.printf("[SGP30]\neCO2: %d ppm\nTVOC: %d ppb\n\n", sgp30_eco2, sgp30_tvoc);
    Serial.printf("[SGP30] eCO2: %d ppm | TVOC: %d ppb\n", sgp30_eco2, sgp30_tvoc);
  } else {
    M5.Lcd.println("[SGP30] Erreur\n");
    Serial.println("[SGP30] Lecture échouée");
  }

  mhz16_co2 = mhz16.readCO2();
  if (mhz16_co2 > 0) {
    M5.Lcd.printf("[MH-Z16]\nCO2: %d ppm\n", mhz16_co2);
    Serial.printf("[MH-Z16] CO2: %d ppm\n", mhz16_co2);
  } else {
    M5.Lcd.println("[MH-Z16] Erreur");
    Serial.println("[MH-Z16] Lecture échouée");
  }

  updateSensorValues(
    scd30_co2, scd30_temp, scd30_hum,
    scd40_co2, scd40_temp, scd40_hum,
    sgp30_eco2, sgp30_tvoc,
    mhz16_co2
  );

  handleClient();
  delay(3000);
} */

void loop() {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);

  readSCD30(scd30_co2, scd30_temp, scd30_hum);
  readSCD40(scd40_co2, scd40_temp, scd40_hum);
  readSGP30(sgp30_eco2, sgp30_tvoc);
  mhz16_co2 = mhz16.readCO2();

  M5.Lcd.printf("[SCD30] CO2: %.1f ppm\nTemp: %.1f °C\nRH: %.1f %%\n\n", scd30_co2, scd30_temp, scd30_hum);
  M5.Lcd.printf("[SCD40] CO2: %d ppm\nTemp: %.2f °C\nRH: %.2f %%\n\n", scd40_co2, scd40_temp, scd40_hum);
  M5.Lcd.printf("[SGP30] eCO2: %d ppm\nTVOC: %d ppb\n\n", sgp30_eco2, sgp30_tvoc);
  M5.Lcd.printf("[MH-Z16] CO2: %d ppm\n", mhz16_co2);

  updateSensorValues(
    scd30_co2, scd30_temp, scd30_hum,
    scd40_co2, scd40_temp, scd40_hum,
    sgp30_eco2, sgp30_tvoc,
    mhz16_co2
  );

  if (measurementActive) {
    File f = SPIFFS.open("/mesures.csv", FILE_APPEND);
    if (f) {
      f.printf("%lu;%.1f;%.1f;%.1f;%d;%.2f;%.2f;%d;%d;%d\n",
        millis() / 1000,
        scd30_co2, scd30_temp, scd30_hum,
        scd40_co2, scd40_temp, scd40_hum,
        sgp30_eco2, sgp30_tvoc,
        mhz16_co2
      );
      f.close();
    }
  }

  handleClient();
  delay(5000);
}
