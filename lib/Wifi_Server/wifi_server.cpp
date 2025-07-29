/*      
Nom du fichier : wifi_server.cpp
Auteur :  GRAITI Mohammed
Responsable : Laurent Oyhenart
Materiel : M5stack Core 2,
Description : Fichier cpp de la communication wifi, partage d'un point acces, "M5STACK_CO2", "12345678",Avec une server HTML 192.168.4.1
*/ 

#include "wifi_server.h"
#include "page_html.h"
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

Preferences preferences;

WebServer server(80);
bool measurementActive = false;

float g_scd30_co2 = 0.0, g_scd30_temp = 0.0, g_scd30_hum = 0.0;
uint16_t g_scd40_co2 = 0;
float g_scd40_temp = 0.0, g_scd40_hum = 0.0;
uint16_t g_sgp30_eco2 = 0, g_sgp30_tvoc = 0;
int g_mhz16_co2 = 0;
uint16_t pm1 = 0, pm2_5 = 0, pm10 = 0;

const char* wifiConfigPage = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>Configuration WiFi – M5Stack</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      margin: 0;
      padding: 0;
      background-color: #10142a;
      color: white;
      font-family: 'Segoe UI', sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .container {
      background: #1a1f3c;
      padding: 30px;
      border-radius: 12px;
      box-shadow: 0 0 15px #00000088;
      max-width: 400px;
      width: 90%;
    }
    h2 {
      text-align: center;
      margin-bottom: 20px;
      color: #00bfff;
    }
    label {
      display: block;
      margin-bottom: 6px;
      font-weight: bold;
    }
    input[type="text"],
    input[type="password"] {
      width: 100%;
      padding: 10px;
      margin-bottom: 15px;
      border: none;
      border-radius: 8px;
      font-size: 16px;
    }
    input[type="submit"] {
      width: 100%;
      padding: 12px;
      background-color: #00bfff;
      color: white;
      border: none;
      border-radius: 8px;
      font-size: 16px;
      cursor: pointer;
      transition: 0.3s;
    }
    input[type="submit"]:hover {
      background-color: #009acd;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>🔧 Connexion WIFI</h2>
    <form action="/connectwifi">
      <label for="ssid">Nom du réseau (SSID)</label>
      <input name="ssid" type="text" id="ssid" placeholder="ex: MonWiFi" required>
      
      <label for="password">Mot de passe</label>
      <input name="password" type="password" id="password" placeholder="••••••••" required>
      
      <input type="submit" value="Se connecter">
    </form>
  </div>
</body>
</html>
)rawliteral"
;

void setupWiFiConfig() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("M5_Config", "12345678");
  Serial.println("[WiFi] Mode config AP: M5_Config");

  server.on("/", []() {
    server.send(200, "text/html", wifiConfigPage);
  });

  server.on("/connectwifi", []() {
    String ssid = server.arg("ssid");
    String pass = server.arg("password");

    server.send(200, "text/plain", "Connexion... Redémarrage");
    delay(1000);
    
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", pass);
    preferences.end();
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    unsigned long t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) {
      delay(500);
      Serial.print(".");
    }
if (WiFi.status() == WL_CONNECTED) {
  Serial.println("\n✅ [WiFi] Connecté !");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  delay(1500);
  ESP.restart();
} else {
  Serial.println("\n❌ [WiFi] Échec de connexion.");
  Serial.print("Dernier SSID tenté : ");
  Serial.println(ssid);
  Serial.println("➡ Redémarrage en mode config...");
  delay(3000);
  ESP.restart();
}

  });

  server.begin();
}

void setupServer() {
  server.on("/", []() {
    extern const char* htmlPage;
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
      f.println("Temps;scd30_co2;scd30_temp;scd30_hum;scd40_co2;scd40_temp;scd40_hum;sgp30_eco2;sgp30_tvoc;mhz16_co2");
      f.close();
      server.send(200, "text/plain", "CSV reset done");
    } else {
      server.send(500, "text/plain", "Erreur création CSV");
    }
  });
  configTime(0, 0, "pool.ntp.org");
    server.on("/data", []() {
/*     time_t now;
    struct tm timeinfo;
    char datetime[25] = "0000-00-00T00:00:00";
    if (getLocalTime(&timeinfo)) {
      strftime(datetime, sizeof(datetime), "%Y-%m-%dT%H:%M:%S", &timeinfo);
    } */

    String json = "{";
    json += "\"temps\":" + String(millis() / 1000) + ",";
    json += "\"scd30_co2\":" + String(g_scd30_co2, 1) + ",";
    json += "\"scd30_temp\":" + String(g_scd30_temp, 1) + ",";
    json += "\"scd30_hum\":" + String(g_scd30_hum, 1) + ",";
    json += "\"scd40_co2\":" + String(g_scd40_co2) + ",";
    json += "\"scd40_temp\":" + String(g_scd40_temp, 1) + ",";
    json += "\"scd40_hum\":" + String(g_scd40_hum, 1) + ",";
    json += "\"sgp30_eco2\":" + String(g_sgp30_eco2) + ",";
    json += "\"sgp30_tvoc\":" + String(g_sgp30_tvoc) + ",";
    json += "\"mhz16_co2\":" + String(g_mhz16_co2) + ",";
    json += "\"pm1\":" + String(pm1) + ",";
    json += "\"pm2_5\":" + String(pm2_5) + ",";
    json += "\"pm10\":" + String(pm10);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.print("[WiFi] IP locale : ");
  Serial.println(WiFi.localIP());
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
