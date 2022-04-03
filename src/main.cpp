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
#include "modules/pwm.h"

AsyncWebServer webServer(HTTP_SERVER_PORT);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
AsyncElegantOtaClass ota;

std::vector<Module*> modules = {
  // new Actuator("Led cabina", SYSTEM_NAME "/led_cabin", &mqttClient, 32),
  new Actuator("Led attico", SYSTEM_NAME "/led_attic", 33),
  new PWM("Scaldasonno", SYSTEM_NAME "/scaldasonno", 22),
};

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(500);

  init_wifi();
  init_ota(&ota, &webServer);
  init_mqtt(&mqttClient);

  mqttClient.setCallback(mqtt_register_callbacks(&mqttClient, modules));

  webServer.begin();

  xTaskCreate(mqtt_connection_task, "MQTT connection task", 2048, &mqttClient, (UBaseType_t)TSK_PRT::P_H, NULL);

  for (int i = 0; i < modules.size(); ++i) {
    modules[i]->setup();
    BaseType_t status = modules[i]->start();

    if (status == pdPASS) {
      log("Task: " + modules[i]->name + " started successfully");
    }
    else {
      log("Unable to allocate enough memory for the task: " + modules[i]->name + ". Task creation failed");
      log("Used " + String(ESP.getFreeHeap()) + " of " + String(ESP.getHeapSize()) + " bytes");
    }
  }
}

void loop(void) {
}
