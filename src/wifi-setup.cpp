#include "wifi-setup.hpp"
#include <WiFi.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

void setupWiFi() {
  // Connect to WiFi network
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}