/*      
Nom du fichier : wifi_server.h
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,
Description : Fichier Header de la communication wifi, 
*/ 
/*#pragma once
#include <WiFi.h>
#include <WebServer.h>

// Déclaration des variables globales
extern float g_scd30_co2, g_scd30_temp, g_scd30_hum;
extern uint16_t g_scd40_co2;
extern float g_scd40_temp, g_scd40_hum;
extern uint16_t g_sgp30_eco2, g_sgp30_tvoc;
extern int g_mhz16_co2;
extern uint16_t pm1, pm2_5, pm10;

void setupWiFi();
void handleClient();
void updateSensorValues(
    float scd30_co2, float scd30_temp, float scd30_hum,
    uint16_t scd40_co2, float scd40_temp, float scd40_hum,
    uint16_t sgp30_eco2, uint16_t sgp30_tvoc,
    int mhz16_co2,
    uint16_t pm1, uint16_t pm2_5, uint16_t pm10
);
extern bool measurementActive;*/

// ===== wifi_server.h =====
#pragma once
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

extern Preferences preferences;

// Déclaration des variables globales
extern float g_scd30_co2, g_scd30_temp, g_scd30_hum;
extern uint16_t g_scd40_co2;
extern float g_scd40_temp, g_scd40_hum;
extern uint16_t g_sgp30_eco2, g_sgp30_tvoc;
extern int g_mhz16_co2;
extern uint16_t pm1, pm2_5, pm10;

// Contrôle
extern bool measurementActive;

// Fonctions principales
void setupWiFiConfig();      // Interface de configuration initiale (SSID/password)
void setupServer();          // Démarrage du serveur après connexion WiFi
void handleClient();         // Appelé dans loop()

// Mise à jour des capteurs
void updateSensorValues(
    float scd30_co2, float scd30_temp, float scd30_hum,
    uint16_t scd40_co2, float scd40_temp, float scd40_hum,
    uint16_t sgp30_eco2, uint16_t sgp30_tvoc,
    int mhz16_co2,
    uint16_t pm1, uint16_t pm2_5, uint16_t pm10
);