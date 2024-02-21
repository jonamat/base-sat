#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt.h"
#include "const.h"
#include "config.h"
#include "module.h"
#include "devices.h"

void init_mqtt(PubSubClient* mqttClient) {
  (*mqttClient).setServer(MQTT_SERVER, 1883);
};

extern String last_status;

int last_heartbeat = 0;
int last_status_update = 0;
int last_error_millis = 0;

void heartbeat(PubSubClient* mqttClient) {
  if (millis() - last_heartbeat > HEARTBEAT_DELAY) {
    last_heartbeat = millis();

    String topic = "sat/" SAT_NAME;
    (*mqttClient).publish(topic.c_str(), last_status.c_str());
  }

  if (millis() - last_status_update > STATUS_UPDATE_DELAY) {
    last_status_update = millis();

    String heapTopic = "sat/" SAT_NAME "/heap";
    (*mqttClient).publish(heapTopic.c_str(), String(ESP.getFreeHeap()).c_str());

    String ipTopic = "sat/" SAT_NAME "/ip";
    (*mqttClient).publish(ipTopic.c_str(), WiFi.localIP().toString().c_str());

    String rssiTopic = "sat/" SAT_NAME "/rssi";
    (*mqttClient).publish(rssiTopic.c_str(), String(WiFi.RSSI()).c_str());

    String uptimeTopic = "sat/" SAT_NAME "/uptime";
    (*mqttClient).publish(uptimeTopic.c_str(), String(millis()).c_str());
  }

  // Blink LED if there was an error
  if (last_status == "ERROR") {
    if (millis() - last_error_millis > 500) {
      last_error_millis = millis();
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}


void mqtt_connection_task(void* parameter) {
  PubSubClient* mqttClient = (PubSubClient*)parameter;

  while (true) {
    if (!(*mqttClient).connected()) {
      while (!(*mqttClient).connected()) {
        String clientId = SAT_NAME "-client-" + String(random(0xffff), HEX);

        if ((*mqttClient).connect(clientId.c_str())) {
          Serial.printf("%s connected to MQTT server %s on port %d as %s\n", SAT_NAME, MQTT_SERVER, MQTT_PORT, clientId.c_str());

          (*mqttClient).subscribe("sat/" SAT_NAME COMMAND_SUBTOPIC);

          for (int i = 0; i < devices.size(); ++i) {
            (*mqttClient).subscribe((devices[i]->topic + COMMAND_SUBTOPIC).c_str());
          }
        }
        else {
          Serial.printf("Cannot connect to MQTT broker. Status: %d. Retrying\n", (*mqttClient).state());
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
    String payload_str;
    for (int i = 0; i < length; i++) {
      payload_str += (char)payload[i];
    }

    // Modules commands
    for (int i = 0; i < _modules.size(); ++i) {
      if (String(topic) == _modules[i]->topic + COMMAND_SUBTOPIC) {
        _modules[i]->onCommand(&payload_str);
      }
    };

    // Sat commands
    if (String(topic) == ("sat/" SAT_NAME COMMAND_SUBTOPIC)) {
      if (payload_str == "REBOOT") {
        ESP.restart();
      }
    };
    };
};
