#include <PubSubClient.h>

#include "../module.h"
#include "../utils.h"

enum class ST {
  ST_L = LOW,
  ST_H = HIGH,
};

extern PubSubClient mqttClient;

/**
 * @brief Actuator
 * @details Provides a digital output on a pin.
 *
 * @param name Name of the device.
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param init_state Starting state from. Default is LOW.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device on /state
 * "ON" set the state to HIGH
 * "OFF" set the state to LOW
 *
 */
class Actuator : public Module
{
private:
  int pin;
  ST state;

public:
  Actuator(
    String name,
    String topic,
    int pin,
    ST init_state = ST::ST_L,
    TSK_PRT task_priority = TSK_PRT::P_M) {
    this->name = name;
    this->topic = topic;
    this->pin = pin;
    this->state = init_state;
    this->task_priority = task_priority;
  }

  void setup() {
    pinMode(this->pin, OUTPUT);
  }

  void onCommand(String* payload) {
    if ((*payload) == "ON" && this->state == ST::ST_L) {
      digitalWrite(this->pin, HIGH);
      this->state = ST::ST_H;
    }
    else if ((*payload) == "OFF" && this->state == ST::ST_H) {
      digitalWrite(this->pin, LOW);
      this->state = ST::ST_L;
    }
    else if ((*payload) == "STATE") {
      String state_topic = this->topic + "/state";
      mqttClient.publish(state_topic.c_str(), this->state == ST::ST_H ? "ON" : "OFF");
    }
  }

  BaseType_t start() {
    digitalWrite(this->pin, (int)this->state);
    return pdPASS;
  }
};
