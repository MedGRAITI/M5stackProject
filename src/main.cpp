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
#include <Preferences.h>
#include <time.h>

#include "scd30_sensor.h"
#include "scd40_sensor.h"
#include "sgp30_sensor.h"
#include "mh-z16_sensor.h"
#include "Laser_pm2-5.h"
#include "wifi_server.h"



HardwareSerial mhzSerial(2);
Mhz16 mhz16(mhzSerial);

float scd30_co2 = 0.0, scd30_temp = 0.0, scd30_hum = 0.0;
uint16_t scd40_co2 = 0;
float scd40_temp = 0.0, scd40_hum = 0.0;
uint16_t sgp30_eco2 = 0, sgp30_tvoc = 0;
int mhz16_co2 = 0;
extern uint16_t pm1, pm2_5, pm10;

extern bool measurementActive;

void setup() {
  M5.begin();
  SPIFFS.begin(true);
  Serial.begin(115200);
  Wire.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.clear();
  


  preferences.begin("wifi", true); // lecture seule
  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("password", "");
  preferences.end();

  if (ssid != "") {
    Serial.println("[WiFi] Connexion automatique à : " + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    unsigned long t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ Connexion réussie");
      Serial.println(WiFi.localIP());
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.printf("✅ WiFi connecté\nSSID: %s\nIP: %s\n", ssid.c_str(), WiFi.localIP().toString().c_str());      
      setupServer();

    } else {
      Serial.println("\n❌ Connexion échouée → mode config");
      Preferences preferences;
      preferences.begin("wifi", false);  // lecture/écriture
      preferences.clear();               // efface ssid/pass
      preferences.end();
      delay(1000);
      setupWiFiConfig();
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.printf("⚙ Config WiFi\nSSID: M5_Config\nIP: 192.168.4.1\n");
    }

  } else {
    Serial.println("[WiFi] Aucune config → mode AP");
    setupWiFiConfig();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("⚙ Config WiFi\nSSID: M5_Config\nIP: 192.168.4.1\n");
  }


  // capteurs, 
  initSCD30();
  initSCD40();
  initSGP30();
  initHM3301();
  mhzSerial.begin(9600, SERIAL_8N1, 13, 14);
  mhz16.begin(9600);
}

void loop() {
  readSCD30(scd30_co2, scd30_temp, scd30_hum);
  readSCD40(scd40_co2, scd40_temp, scd40_hum);
  readSGP30(sgp30_eco2, sgp30_tvoc);
  mhz16_co2 = mhz16.readCO2();
  if (mhz16_co2 > 0) {
    Serial.printf("[MH-Z16] CO2: %d ppm\n", mhz16_co2);
  } else {
    Serial.println("[MH-Z16] Lecture échouée");
  }
  readHM3301(pm1, pm2_5, pm10);

  // Mise à jour valeurs partagées
  updateSensorValues(
    scd30_co2, scd30_temp, scd30_hum,
    scd40_co2, scd40_temp, scd40_hum,
    sgp30_eco2, sgp30_tvoc,
    mhz16_co2,
    pm1, pm2_5, pm10
  );

  // Enregistrement CSV si actif
  if (measurementActive) {
    File f = SPIFFS.open("/mesures.csv", FILE_APPEND);
    if (f) {
        f.printf("%lu;%.1f;%.1f;%.1f;%d;%.2f;%.2f;%d;%d;%d;%u;%u;%u\n",
        millis() / 1000,
        scd30_co2, scd30_temp, scd30_hum,
        scd40_co2, scd40_temp, scd40_hum,
        sgp30_eco2, sgp30_tvoc,
        mhz16_co2,
        pm1, pm2_5, pm10
      );
      f.close();
    }
  }

  handleClient();
  delay(5000);
}
