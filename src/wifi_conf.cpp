#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "utils.h"
#include "wifi_conf.h"

void init_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  log("Connecting to SSID " + String(WIFI_SSID) + "...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  log("Connected to " WIFI_SSID ". IP Address: " + WiFi.localIP().toString());
}
