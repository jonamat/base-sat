#include <PubSubClient.h>

#include "../module.h"
#include "../utils.h"

enum class ST {
  ST_L = LOW,
  ST_H = HIGH,
};

extern PubSubClient mqttClient;

/**
 * @brief DigitalReader
 * @details Read the state of a digital pin and send the new state on change.
 *
 * @param name Name of the device.
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param delay Delay between each state check. Default is 1000ms.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available command payloads:
 * "STATE" publish the current state of the device on /state
 *
 */
class DigitalReader : public Module
{
private:
  int pin;
  ST state;
  uint delay;

public:
  DigitalReader(
    String name,
    String topic,
    int pin,
    uint delay = 1000,
    TSK_PRT task_priority = TSK_PRT::P_M
  ) {
    this->name = name;
    this->topic = topic;
    this->pin = pin;
    this->delay = delay;
    this->task_priority = task_priority;
  }

  static void task(void* param) {
    DigitalReader* pThis = (DigitalReader*)param;

    while (true) {
      int currentState = digitalRead(pThis->pin);

      if ((ST)currentState != pThis->state) {
        pThis->state = (ST)currentState;

        String state_topic = pThis->topic + "/state";
        mqttClient.publish(state_topic.c_str(), pThis->state == ST::ST_H ? "ON" : "OFF");
      }

      vTaskDelay(pThis->delay / portTICK_PERIOD_MS);
    }
  }

  void setup() {
    pinMode(this->pin, OUTPUT);
  }

  void onCommand(String* payload) {
    if ((*payload) == "STATE") {
      int currentState = digitalRead(this->pin);

      String state_topic = this->topic + "/state";
      mqttClient.publish(state_topic.c_str(), this->state == ST::ST_H ? "ON" : "OFF");
    }
  }

  BaseType_t start() {
    return xTaskCreate(
      &DigitalReader::task,
      this->name.c_str(),
      2048,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
