/* #include <Arduino.h>
#include <Wire.h>
#include <M5Core2.h>
#include "scd30_sensor.h"
#include "scd40_sensor.h"
#include "sgp30_sensor.h"
#include "mh-z16_sensor.h"

// UART pour MH-Z16 sur GPIO 16/17 (port série 2)
HardwareSerial mhzSerial(2);
Mhz16 mhz16(mhzSerial);

void setup() {
  M5.begin();
  Serial.begin(115200);
  Wire.begin();  // SDA = 21, SCL = 22 (port A)

  // Initialisation des capteurs I2C
  initSCD30();
  initSCD40();
  initSGP30();

  // Initialisation du capteur MH-Z16
  mhz16.begin(9600);
}

void loop() {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);

  // Lecture capteurs I2C
  readSCD30();   // Affiche sur Serial et/ou LCD
  readSCD40();
  readSGP30();

  // Lecture capteur UART MH-Z16
  int co2 = mhz16.readCO2();
  if (co2 > 0) {
    M5.Lcd.printf("[MH-Z16] CO2 : %d ppm\n", co2);
    Serial.printf("[MH-Z16] CO2 : %d ppm\n", co2);
  } else {
    M5.Lcd.println("[MH-Z16] Erreur de lecture !");
    Serial.println("[MH-Z16] Erreur de lecture !");
  }

  delay(3000);
}
 */

#include <Arduino.h>
#include <Wire.h>
#include <M5Core2.h>
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

void setup() {
  M5.begin();
  Serial.begin(115200);
  Wire.begin();
  
  initSCD30();
  initSCD40();
  initSGP30();
  mhz16.begin(9600);
  setupWiFi();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void loop() {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);

/*   float co2, temp, hum;
  if (!readSCD30(co2, temp, hum)) {
    M5.Lcd.println("[SCD30] Error\n");
    Serial.println("[SCD30] Failed to read");
  } */
   if (readSCD30(scd30_co2, scd30_temp, scd30_hum)) {
    Serial.printf("[SCD30] CO2: %.1f ppm | Temp: %.1f °C | RH: %.1f %%\n", scd30_co2, scd30_temp, scd30_hum);
    M5.Lcd.printf("[SCD30]\nCO2: %.1f ppm\nTemp: %.1f °C\nRH: %.1f %%\n\n", scd30_co2, scd30_temp, scd30_hum);
  } else {
    Serial.println("[SCD30] Lecture échouée");
    M5.Lcd.println("[SCD30] Erreur\n");
  }
  readSCD40();
  readSGP30();

/*   int co2ppm = mhz16.readCO2();
  if (co2ppm > 0) {
    M5.Lcd.printf("[MH-Z16]\nCO2: %d ppm\n", co2ppm);
    Serial.printf("[MH-Z16] CO2: %d ppm\n", co2ppm);
  } else {
    M5.Lcd.println("[MH-Z16] Error\n");
    Serial.println("[MH-Z16] Failed to read");
  } */
  mhz16_co2 = mhz16.readCO2();
  if (mhz16_co2 > 0) {
    Serial.printf("[MH-Z16] CO2: %d ppm\n", mhz16_co2);
    M5.Lcd.printf("[MH-Z16]\nCO2: %d ppm\n", mhz16_co2);
  } else {
    Serial.println("[MH-Z16] Lecture échouée");
    M5.Lcd.println("[MH-Z16] Erreur");
  }

  updateSensorValues(
    scd30_co2, scd30_temp, scd30_hum,
    scd40_co2, scd40_temp, scd40_hum,
    sgp30_eco2, sgp30_tvoc,
    mhz16_co2
  );

  handleClient();
  delay(3000);
}
