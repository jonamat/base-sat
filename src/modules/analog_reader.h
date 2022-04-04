#include <PubSubClient.h>

#include "../module.h"
#include "../utils.h"

enum class ST {
  ST_L = LOW,
  ST_H = HIGH,
};

extern PubSubClient mqttClient;

/**
 * @brief AnalogReader
 * @details Read the state of a digital pin and send the new state on change.
 *
 * @param name Name of the device.
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param delay Delay between each state check. Default is 1000ms.
 * @param tolerance Tolerance to trigger a state change. Default is 10.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device on /state
 *
 * @note Available events:
 * on topic "/events" with payload "[int]" the state of the device has changed
 *
 * TODO:
 * - Add resolution
 * - Add cycles
 * - Add samples
 *
 */
class AnalogReader : public Module
{
private:
  int pin;
  int state;
  int tolerance;
  uint delay;

public:
  AnalogReader(
    String name,
    String topic,
    int pin,
    uint delay = 1000,
    int tolerance = 10,
    TSK_PRT task_priority = TSK_PRT::P_M
  ) {
    this->name = name;
    this->topic = topic;
    this->pin = pin;
    this->delay = delay;
    this->tolerance = tolerance;
    this->task_priority = task_priority;
  }

  static void task(void* param) {
    AnalogReader* pThis = (AnalogReader*)param;

    while (true) {
      int currentState = analogRead(pThis->pin);

      if (currentState > pThis->state + pThis->tolerance || currentState < pThis->state - pThis->tolerance) {
        pThis->state = currentState;

        String state_topic = pThis->topic + "/events";
        char c_state[4];
        sprintf(c_state, "%d", pThis->state);
        mqttClient.publish(state_topic.c_str(), c_state);
      }

      vTaskDelay(pThis->delay / portTICK_PERIOD_MS);
    }
  }

  void setup() {
  }

  void onCommand(String* payload) {
    if ((*payload) == "STATE") {
      int currentState = analogRead(this->pin);

      String state_topic = this->topic + "/state";
      char c_state[4];
      sprintf(c_state, "%d", this->state);
      mqttClient.publish(state_topic.c_str(), c_state);
    }
  }

  BaseType_t start() {
    return xTaskCreate(
      &AnalogReader::task,
      this->name.c_str(),
      2048,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
