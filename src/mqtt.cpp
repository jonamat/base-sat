#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt.h"
#include "config.h"
#include "modules/module.h"

void init_mqtt(PubSubClient* mqttClient) {
  (*mqttClient).setServer(MQTT_SERVER, 1883);
};

int last_heartbeat = 0;
void heartbeat(PubSubClient* mqttClient) {
  if (millis() - last_heartbeat > HEARTBEAT_DELAY) {
    last_heartbeat = millis();

    String heapTopic = "sat/" SAT_NAME "/heap";
    (*mqttClient).publish(heapTopic.c_str(), String(ESP.getFreeHeap()).c_str());

    String ipTopic = "sat/" SAT_NAME "/ip";
    (*mqttClient).publish(ipTopic.c_str(), WiFi.localIP().toString().c_str());

    String rssiTopic = "sat/" SAT_NAME "/rssi";
    (*mqttClient).publish(rssiTopic.c_str(), String(WiFi.RSSI()).c_str());

    String uptimeTopic = "sat/" SAT_NAME "/uptime";
    (*mqttClient).publish(uptimeTopic.c_str(), String(millis()).c_str());
  }
}


void mqtt_connection_task(void* parameter) {
  PubSubClient* mqttClient = (PubSubClient*)parameter;

  while (true) {
    if (!(*mqttClient).connected()) {
      while (!(*mqttClient).connected()) {
        String clientId = SAT_NAME "-client-" + String(random(0xffff), HEX);

        if ((*mqttClient).connect(clientId.c_str())) {
          char buf[255];
          sprintf(buf, "%s connected to MQTT server %s on port %d as %s", SAT_NAME, MQTT_SERVER, MQTT_PORT, clientId.c_str());
          Serial.println(buf);

          (*mqttClient).subscribe("dev/#");
        }
        else {
          char buf[255];
          sprintf(buf, "Cannot connect to MQTT broker. Status: %d. Retrying", (*mqttClient).state());
          Serial.println(buf);

          vTaskDelay(RECONNECTION_TIME / portTICK_PERIOD_MS);
        }
      }
    }

    heartbeat(mqttClient);

    (*mqttClient).loop();
  }
};

std::function<void(char*, uint8_t*, unsigned int)> mqtt_register_callbacks(PubSubClient* _mqttClient, std::vector<Module*> _modules) {
  return [_mqttClient, _modules](char* topic, uint8_t* payload, unsigned int length) {
    {
      String payload_str;
      for (int i = 0; i < length; i++) {
        payload_str += (char)payload[i];
      }

      for (int i = 0; i < _modules.size(); ++i) {
        if (String(topic) == _modules[i]->topic + COMMAND_SUBTOPIC) {
          _modules[i]->onCommand(&payload_str);
        }
      };
    };
    };
};
