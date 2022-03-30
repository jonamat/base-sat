#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt.h"
#include "config.h"
#include "utils.h"

void init_mqtt(PubSubClient *mqttClient)
{
  (*mqttClient).setServer(MQTT_SERVER, 1883);
}

void mqtt_connection_task(PubSubClient *mqttClient )
{

    if (!(*mqttClient).connected())
    {
      // TODO remove blocking code
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
