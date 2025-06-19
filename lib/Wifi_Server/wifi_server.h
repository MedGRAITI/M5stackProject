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

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connexion WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WiFi] Connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    String page = "<h2>Données Capteurs CO2</h2>";
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


//http://192.168.4.1