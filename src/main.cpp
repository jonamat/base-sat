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
// #include "core-modules/module.h"

AsyncWebServer webServer(HTTP_SERVER_PORT);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
AsyncElegantOtaClass ota;

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
      char buf[255];
      sprintf(buf, "Task %s started successfully", devices[i]->topic.c_str());
      Serial.println(buf);
    }
    else {

      // Blink the builtin LED to signal an error
      while (true) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(300);
        digitalWrite(LED_BUILTIN, LOW);
        delay(300);
      }
    }
  }
}

void loop(void) {
}
