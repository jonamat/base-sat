#include <PubSubClient.h>

#include "../module.h"

extern PubSubClient mqttClient;

/**
 * @brief DigitalOutput
 * @details Provides a digital output on a pin.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 * "ON" set the state to HIGH
 * "OFF" set the state to LOW
 *
 */
class DigitalOutput : public Module
{
private:
  int pin;
  ST state = ST::ST_L;

public:
  DigitalOutput(
    String topic,
    int pin,
    ST init_state = ST::ST_L,
    TSK_PRT task_priority = TSK_PRT::P_M) {
    this->topic = topic;
    this->pin = pin;
    this->task_priority = task_priority;
  }

  void setup() {
    pinMode(this->pin, OUTPUT);
  }

  void onCommand(String* payload) {
    if ((*payload) == "ON" && this->state == ST::ST_L) {
      digitalWrite(this->pin, HIGH);
      this->state = ST::ST_H;
      mqttClient.publish(this->topic.c_str(), this->state == ST::ST_H ? "ON" : "OFF");
    }
    else if ((*payload) == "OFF" && this->state == ST::ST_H) {
      digitalWrite(this->pin, LOW);
      this->state = ST::ST_L;
      mqttClient.publish(this->topic.c_str(), this->state == ST::ST_H ? "ON" : "OFF");
    }
    else if ((*payload) == "STATE") {
      mqttClient.publish(this->topic.c_str(), this->state == ST::ST_H ? "ON" : "OFF");
    }
  }

  BaseType_t start() {
    digitalWrite(this->pin, (int)this->state);
    return pdPASS;
  }
};
