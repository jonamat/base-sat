#include <PubSubClient.h>

#include "../module.h"

extern PubSubClient mqttClient;

/**
 * @brief DigitalInput
 * @details Read the state of a digital pin and send the new state on change.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param delay Delay between each state check. Default is 1000ms.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 *
 */
class DigitalInput : public Module
{
private:
  int pin;
  ST state = ST::UNKNOWN;
  uint delay;
  bool invert_logic = false;

public:
  DigitalInput(
    String topic,
    int pin,
    uint delay = 10,
    TSK_PRT task_priority = TSK_PRT::P_M,
    bool invert_logic = false
  ) {
    this->topic = topic;
    this->pin = pin;
    this->delay = delay;
    this->task_priority = task_priority;
    this->invert_logic = invert_logic;
  }

  void setup() {
    pinMode(this->pin, INPUT_PULLDOWN);
    ST currentState = (ST)digitalRead(this->pin);
    this->state = currentState;
    mqttClient.publish(this->topic.c_str(), stateToPayload(currentState).c_str());
  }

  static void task(void* param) {
    DigitalInput* pThis = (DigitalInput*)param;

    while (true) {
      ST currentState = (ST)digitalRead(pThis->pin);

      if (currentState != pThis->state) {
        pThis->state = currentState;
        mqttClient.publish(pThis->topic.c_str(), pThis->stateToPayload(currentState).c_str());
      }

      vTaskDelay(pThis->delay / portTICK_PERIOD_MS);
    }
  }

  void onCommand(String* payload) {
    if ((*payload) == "STATE") {
      ST currentState = (ST)digitalRead(this->pin);
      mqttClient.publish(this->topic.c_str(), stateToPayload(currentState).c_str());
    }
  }

  String stateToPayload(ST state) {
    if (this->invert_logic) {
      return state == ST::ST_H ? "OFF" : "ON";
    } else {
      return state == ST::ST_H ? "ON" : "OFF";
    }
  }

  BaseType_t start() {
    return xTaskCreate(
      &DigitalInput::task,
      this->topic.c_str(),
      2048,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
