#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <AsyncElegantOTA.h>

#include "config.h"
#include "module.h"
#include "wifi.h"
#include "ota.h"
#include "mqtt.h"

AsyncWebServer webServer(HTTP_SERVER_PORT);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
AsyncElegantOtaClass ota;


void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    delay(500);

  InitWifi();
  InitOTA(ota, webServer);
  InitMQTT(mqttClient);

  webServer.begin();
}

void loop(void)
{
  MqttConnectionLoop(mqttClient);
}