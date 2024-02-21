#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <AsyncElegantOTA.h>

#include "config.h"
#include "const.h"
#include "wifi_conf.h"
#include "ota.h"
#include "mqtt.h"
#include "devices.h"

AsyncWebServer webServer(HTTP_SERVER_PORT);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
AsyncElegantOtaClass ota;

String last_status = "ALIVE";

void setup(void) {
  Serial.begin(SERIAL_BAUD_RATE);

  init_wifi();
  init_ota(&ota, &webServer);
  init_mqtt(&mqttClient);
  mqttClient.setCallback(mqtt_register_callbacks(&mqttClient, devices));
  webServer.begin();

  xTaskCreate(mqtt_connection_task, "MQTT connection task", 4096, &mqttClient, (UBaseType_t)TSK_PRT::P_H, NULL);

  // Wait for the MQTT connection to be established
  while (!mqttClient.connected()) {
    delay(100);
  }

  for (int i = 0; i < devices.size(); ++i) {
    devices[i]->setup();
    BaseType_t status = devices[i]->start();

    if (status == pdPASS) {
      Serial.printf("Task %s started successfully\n", devices[i]->topic.c_str());
    }
    else {
      last_status = "ERROR";
      String error = "Task " + devices[i]->topic + " failed to start";
      Serial.println(error);
      mqttClient.publish("sat/" SAT_NAME "/error", error.c_str());
    }
  }
}

void loop(void) {
}
