#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <AsyncElegantOTA.h>

#include "config.h"
#include "wifi_conf.h"
#include "ota.h"
#include "mqtt.h"
#include "modules/module.h"
#include "modules/modules/analog_output.h"
#include "modules/modules/analog_reader.h"
#include "modules/modules/button.h"
#include "modules/modules/digital_input.h"
#include "modules/modules/digital_output.h"
#include "modules/modules/ir_output.h"
#include "modules/modules/ntc_thermistor.h"
#include "modules/modules/pwm.h"

AsyncWebServer webServer(HTTP_SERVER_PORT);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
AsyncElegantOtaClass ota;


std::vector<Module*> modules = {
  new AnalogOutputDAC("/lights", 33),
  new AnalogReader("/sun-tracer", 34),
  new Button("/light-switch", 35),
  new DigitalInput("/door-sensor", 36),
  new DigitalOutput("/heater", 37),
  new IROutput("/tv-remote", 38),
  new NTCThermistor("/temperature", 39, 10000, 10000),
  new PWM("/fan", 40, 1000, 8, 0, 1023)
};


void setup(void) {
  Serial.begin(SERIAL_BAUD_RATE);

  init_wifi();
  init_ota(&ota, &webServer);
  init_mqtt(&mqttClient);
  mqttClient.setCallback(mqtt_register_callbacks(&mqttClient, modules));
  webServer.begin();

  xTaskCreate(mqtt_connection_task, "MQTT connection task", 4096, &mqttClient, (UBaseType_t)TSK_PRT::P_H, NULL);

  // Wait for the MQTT connection to be established
  while (!mqttClient.connected()) {
    delay(100);
  }

  for (int i = 0; i < modules.size(); ++i) {
    modules[i]->setup();
    BaseType_t status = modules[i]->start();

    if (status == pdPASS) {
      printf("Task: %s started successfully", modules[i]->name.c_str());
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
