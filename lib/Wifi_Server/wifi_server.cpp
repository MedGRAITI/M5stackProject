#include "wifi_server.h"
#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "M5Stack_CO2";
const char* password = "123456789";

WebServer server(80);

// Valeurs globales pour les capteurs
float g_scd30_co2 = 0, g_scd30_temp = 0, g_scd30_hum = 0;
uint16_t g_scd40_co2 = 0;
float g_scd40_temp = 0, g_scd40_hum = 0;
uint16_t g_sgp30_eco2 = 0, g_sgp30_tvoc = 0;
int g_mhz16_co2 = 0;