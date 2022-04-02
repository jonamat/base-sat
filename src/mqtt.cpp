#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt.h"
#include "config.h"
#include "utils.h"
#include "module.h"

void init_mqtt(PubSubClient* mqttClient)
{
  (*mqttClient).setServer(MQTT_SERVER, 1883);
};

void mqtt_connection_task(void* parameter)
{
  PubSubClient* mqttClient = (PubSubClient*)parameter;

  while (true)
  {
    if (!(*mqttClient).connected())
    {
      while (!(*mqttClient).connected())
      {
        String clientId = SAT_NAME "-client-" + String(random(0xffff), HEX);

        if ((*mqttClient).connect(clientId.c_str()))
        {
          log(SAT_NAME " connected to MQTT broker " MQTT_SERVER " on port " + String(MQTT_PORT) + " as " + clientId);
          (*mqttClient).subscribe(SYSTEM_NAME "/#");
        }
        else
        {
          log("cannot connect to MQTT broker. Status:" + String((*mqttClient).state()) + ". Retring...");
          delay(RECONNECTION_TIME);
        }
      }
    }

    (*mqttClient).loop();
  }
};

std::function<void(char*, uint8_t*, unsigned int)> mqtt_register_callbacks(PubSubClient* _mqttClient, std::vector<Module*> _modules)
{
  return [_mqttClient, _modules](char* topic, uint8_t* payload, unsigned int length)
  {
    {
      String payload_str;
      for (int i = 0; i < length; i++)
      {
        payload_str += (char)payload[i];
      }

      for (int i = 0; i < _modules.size(); ++i)
      {
        if (strcmp(topic, _modules[i]->topic.c_str()) == 0)
        {
          _modules[i]->onCommand(&payload_str);
        }
      };
    };
  };
};
