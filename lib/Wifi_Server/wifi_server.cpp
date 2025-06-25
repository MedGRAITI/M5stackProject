/*      
Nom du fichier : wifi_server.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,
Description : Fichier cpp de la communication wifi, partage d'un point acces, "M5STACK_CO2", "12345678",Avec une server HTML 192.168.4.1
*/ 

#include "wifi_server.h"
#include <WiFi.h>
#include <WebServer.h>

// Serveur web sur port 80
WebServer server(80);

// Variables partagées
float g_scd30_co2 = 0.0, g_scd30_temp = 0.0, g_scd30_hum = 0.0;
uint16_t g_scd40_co2 = 0;
float g_scd40_temp = 0.0, g_scd40_hum = 0.0;
uint16_t g_sgp30_eco2 = 0, g_sgp30_tvoc = 0;
int g_mhz16_co2 = 0;

void setupWiFi() {
  WiFi.mode(WIFI_AP);  // activer le mode point d'accès
  WiFi.softAP("M5STACK_CO2", "12345678");

  Serial.println("[WiFi] Point d’accès créé : M5STACK_CO2");
  Serial.print("[WiFi] Adresse IP : ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    String page = "<html><head><meta http-equiv='refresh' content='5'></head><body>";
    page += "<h2>Données Capteurs CO2</h2>";

    page += "<h3>SCD30</h3>";
    page += "CO2: " + String(g_scd30_co2) + " ppm<br>";
    page += "Température: " + String(g_scd30_temp) + " °C<br>";
    page += "Humidité: " + String(g_scd30_hum) + " %<br><br>";

    page += "<h3>SCD40</h3>";
    page += "CO2: " + String(g_scd40_co2) + " ppm<br>";
    page += "Température: " + String(g_scd40_temp) + " °C<br>";
    page += "Humidité: " + String(g_scd40_hum) + " %<br><br>";

    page += "<h3>SGP30</h3>";
    page += "eCO2: " + String(g_sgp30_eco2) + " ppm<br>";
    page += "TVOC: " + String(g_sgp30_tvoc) + " ppb<br><br>";

    page += "<h3>MH-Z16</h3>";
    page += "CO2: " + String(g_mhz16_co2) + " ppm<br>";

    page += "</body></html>";

    server.send(200, "text/html", page);
  });

  server.begin();
}

void handleClient() {
  server.handleClient();
}

void updateSensorValues(
    float scd30_co2, float scd30_temp, float scd30_hum,
    uint16_t scd40_co2, float scd40_temp, float scd40_hum,
    uint16_t sgp30_eco2, uint16_t sgp30_tvoc,
    int mhz16_co2
) {
  g_scd30_co2 = scd30_co2;
  g_scd30_temp = scd30_temp;
  g_scd30_hum = scd30_hum;

  g_scd40_co2 = scd40_co2;
  g_scd40_temp = scd40_temp;
  g_scd40_hum = scd40_hum;

  g_sgp30_eco2 = sgp30_eco2;
  g_sgp30_tvoc = sgp30_tvoc;

  g_mhz16_co2 = mhz16_co2;
}
