#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "wifi_conf.h"
#include "config.h"
#include "utils.h"

void init_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  char buf[255];
  sprintf(buf, "Connecting to SSID  %s...", WIFI_SSID);
  log(buf);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  char buf[255];
  sprintf(buf, "Connected to  %s. IP Address: ", WIFI_SSID, WiFi.localIP().toString());
  log(buf);
}

