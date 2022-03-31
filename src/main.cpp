#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <AsyncElegantOTA.h>

#include "config.h"
#include "wifi_conf.h"
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

  init_wifi();
  init_ota(&ota, &webServer);
  init_mqtt(&mqttClient);
  webServer.begin();

  xTaskCreate(mqtt_connection_task, "MQTT connection task", 2048, &mqttClient, TASK_LOW_PRIORITY, NULL);
}

void loop(void)
{
}