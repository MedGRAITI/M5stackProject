/*      
Nom du fichier : wifi_server.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,
Description : Fichier cpp de la communication wifi, partage d'un point acces, "M5STACK_CO2", "12345678",Avec une server HTML 192.168.4.1
*/ 

/* #include "wifi_server.h"
#include "page_html.h"
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Serveur web sur port 80
WebServer server(80);
bool measurementActive = false;


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
  server.serveStatic("/plotly.min.js", SPIFFS, "/plotly-latest.min.js");
  server.serveStatic("/logo.png", SPIFFS, "/logo.png");
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");

  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/start", []() {
    measurementActive = true;
    server.send(200, "text/plain", "OK");
  });

  server.on("/stop", []() {
    measurementActive = false;
    server.send(200, "text/plain", "STOP");
  });

  server.on("/download", []() {
    File f = SPIFFS.open("/mesures.csv", "r");
    if (!f) {
      server.send(404, "text/plain", "Fichier non trouvé");
      return;
    }
    server.streamFile(f, "text/csv");
    f.close();
  });

  server.on("/reset", []() {
    File f = SPIFFS.open("/mesures.csv", FILE_WRITE);
    if (f) {
      f.println("time;scd30_co2;scd30_temp;scd30_hum;scd40_co2;scd40_temp;scd40_hum;sgp30_eco2;sgp30_tvoc;mhz16_co2");
      f.close();
      server.send(200, "text/plain", "CSV reset done");
    } else {
      server.send(500, "text/plain", "Erreur création CSV");
    }
  });

  server.on("/data", []() {
    if (!isfinite(g_scd30_co2)) g_scd30_co2 = 0;
    if (!isfinite(g_scd30_temp)) g_scd30_temp = 0;
    if (!isfinite(g_scd30_hum)) g_scd30_hum = 0;
    if (!isfinite(g_scd40_temp)) g_scd40_temp = 0;
    if (!isfinite(g_scd40_hum)) g_scd40_hum = 0;

    String json = "{";
    json += "\"time\":" + String(millis() / 1000) + ",";
    json += "\"scd30_co2\":" + String(g_scd30_co2, 1) + ",";
    json += "\"scd30_temp\":" + String(g_scd30_temp, 1) + ",";
    json += "\"scd30_hum\":" + String(g_scd30_hum, 1) + ",";
    json += "\"scd40_co2\":" + String(g_scd40_co2) + ",";
    json += "\"scd40_temp\":" + String(g_scd40_temp, 1) + ",";
    json += "\"scd40_hum\":" + String(g_scd40_hum, 1) + ",";
    json += "\"sgp30_eco2\":" + String(g_sgp30_eco2) + ",";
    json += "\"sgp30_tvoc\":" + String(g_sgp30_tvoc) + ",";
    json += "\"mhz16_co2\":" + String(g_mhz16_co2);
    json += "}";

    Serial.println("[/data] " + json);
    server.send(200, "application/json", json);
  });

  server.on("/load_scd30", []() {
    File f = SPIFFS.open("/mesures.csv", "r");
    if (!f) return server.send(404, "text/plain", "Fichier non trouvé");
    String json = "[";
    f.readStringUntil('\n');
    while (f.available()) {
      String line = f.readStringUntil('\n');
      int sep1 = line.indexOf(';');
      int sep2 = line.indexOf(';', sep1 + 1);
      if (sep1 > 0 && sep2 > 0) {
        String time = line.substring(0, sep1);
        String value = line.substring(sep1 + 1, sep2);
        json += "{\"t\":" + time + ",\"v\":" + value + "},";
      }
    }
    f.close();
    if (json.endsWith(",")) json.remove(json.length() - 1);
    json += "]";
    server.send(200, "application/json", json);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Page non trouvée !");
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
 */
/*
Nom du fichier : wifi_server.cpp
Auteur : GRAITI Mohammed
Modifié : Mode client WiFi + fallback AP + serveur HTTP
*/

#include "wifi_server.h"
#include "page_html.h"
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Paramètres WiFi client
#define WIFI_SSID     "Simo10"
#define WIFI_PASSWORD "0987654321"

WebServer server(80);
bool measurementActive = false;

// Données capteurs (globales)
float g_scd30_co2 = 0.0, g_scd30_temp = 0.0, g_scd30_hum = 0.0;
uint16_t g_scd40_co2 = 0;
float g_scd40_temp = 0.0, g_scd40_hum = 0.0;
uint16_t g_sgp30_eco2 = 0, g_sgp30_tvoc = 0;
int g_mhz16_co2 = 0;
uint16_t pm1 = 0, pm2_5 = 0, pm10 = 0;

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("[WiFi] Connexion à %s", WIFI_SSID);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connecté !");
    Serial.print("📡 IP locale : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WiFi] Échec → mode AP fallback");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("M5STACK_CO2", "12345678");
    Serial.print("📡 Adresse IP : ");
    Serial.println(WiFi.softAPIP());
  }

  // Fichiers statiques (facultatif si Chart.js en CDN)
  server.serveStatic("/logo.png", SPIFFS, "/logo.png");
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");

  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/start", []() {
    measurementActive = true;
    server.send(200, "text/plain", "OK");
  });

  server.on("/stop", []() {
    measurementActive = false;
    server.send(200, "text/plain", "STOP");
  });

  server.on("/download", []() {
    File f = SPIFFS.open("/mesures.csv", "r");
    if (!f) {
      server.send(404, "text/plain", "Fichier non trouvé");
      return;
    }
    server.streamFile(f, "text/csv");
    f.close();
  });

  server.on("/reset", []() {
    File f = SPIFFS.open("/mesures.csv", FILE_WRITE);
    if (f) {
      f.println("time;scd30_co2;scd30_temp;scd30_hum;scd40_co2;scd40_temp;scd40_hum;sgp30_eco2;sgp30_tvoc;mhz16_co2");
      f.close();
      server.send(200, "text/plain", "CSV reset done");
    } else {
      server.send(500, "text/plain", "Erreur création CSV");
    }
  });

  server.on("/data", []() {
    if (!isfinite(g_scd30_co2)) g_scd30_co2 = 0;
    if (!isfinite(g_scd30_temp)) g_scd30_temp = 0;
    if (!isfinite(g_scd30_hum)) g_scd30_hum = 0;
    if (!isfinite(g_scd40_temp)) g_scd40_temp = 0;
    if (!isfinite(g_scd40_hum)) g_scd40_hum = 0;

    String json = "{";
    json += "\"time\":" + String(millis() / 1000) + ",";
    json += "\"scd30_co2\":" + String(g_scd30_co2, 1) + ",";
    json += "\"scd30_temp\":" + String(g_scd30_temp, 1) + ",";
    json += "\"scd30_hum\":" + String(g_scd30_hum, 1) + ",";
    json += "\"scd40_co2\":" + String(g_scd40_co2) + ",";
    json += "\"scd40_temp\":" + String(g_scd40_temp, 1) + ",";
    json += "\"scd40_hum\":" + String(g_scd40_hum, 1) + ",";
    json += "\"sgp30_eco2\":" + String(g_sgp30_eco2) + ",";
    json += "\"sgp30_tvoc\":" + String(g_sgp30_tvoc) + ",";
    json += "\"mhz16_co2\":" + String(g_mhz16_co2)+ ",";
    json += "\"pm1\":" + String(pm1) + ",";
    json += "\"pm2_5\":" + String(pm2_5) + ",";
    json += "\"pm10\":" + String(pm10);
    json += "}";

    server.send(200, "application/json", json);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Page non trouvée !");
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
  int mhz16_co2,
  uint16_t pm1_, uint16_t pm2_5_, uint16_t pm10_
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

  pm1 = pm1_;
  pm2_5 = pm2_5_;
  pm10 = pm10_;
}
