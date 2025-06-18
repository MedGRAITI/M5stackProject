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

HardwareSerial mhzSerial(2);
Mhz16 mhz16(mhzSerial);

void setup() {
  M5.begin();
  Serial.begin(115200);
  Wire.begin();

  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);

  initSCD30();
  initSCD40();
  initSGP30();
  mhz16.begin(9600);
}

void loop() {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);

  float co2, temp, hum;
  if (!readSCD30(co2, temp, hum)) {
    M5.Lcd.println("[SCD30] Error\n");
    Serial.println("[SCD30] Failed to read");
  }
  readSCD40();
  readSGP30();

  int co2ppm = mhz16.readCO2();
  if (co2ppm > 0) {
    M5.Lcd.printf("[MH-Z16]\nCO2: %d ppm\n", co2ppm);
    Serial.printf("[MH-Z16] CO2: %d ppm\n", co2ppm);
  } else {
    M5.Lcd.println("[MH-Z16] Error\n");
    Serial.println("[MH-Z16] Failed to read");
  }

  delay(3000);
}
