#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <AsyncElegantOTA.h>

#include "config.h"
#include "wifi_conf.h"
#include "ota.h"
#include "mqtt.h"
#include "module.h"
#include "modules/actuator.h"

AsyncWebServer webServer(HTTP_SERVER_PORT);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
AsyncElegantOtaClass ota;

std::vector<Module*> modules = {
  // new Actuator("Led cabina", SYSTEM_NAME "/led_cabin", &mqttClient, 32),
  new Actuator("Led attico", SYSTEM_NAME "/led_attic", &mqttClient, 33),
};

void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    delay(500);

  init_wifi();
  init_ota(&ota, &webServer);
  init_mqtt(&mqttClient);

  mqttClient.setCallback(mqtt_register_callbacks(&mqttClient, modules));

  webServer.begin();

  xTaskCreate(mqtt_connection_task, "MQTT connection task", 2048, &mqttClient, TASK_LOW_PRIORITY, NULL);

  for (int i = 0; i < modules.size(); ++i)
  {
    modules[i]->setup();
    modules[i]->startTask();
  }
}

void loop(void)
{
}
