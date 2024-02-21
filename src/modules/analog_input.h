#include <PubSubClient.h>
#include "../module.h"

#define SAMPLING_RATE 10

extern PubSubClient mqttClient;

/**
 * @brief AnalogInput
 * @details Read the binary state of a pin and send the new state on change.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param delay Delay between each state check. Default is 100ms.
 * @param tolerance Tolerance to trigger a state change. Default is 50.
 * @param task_priority Priority of the task. Default is P_M (medium).
 * @param samples Number of samples to average. Default is 10.
 * @param delay_between_samples Delay between each sample. Default is 10ms.
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 *
 */
class AnalogInput : public Module
{
private:
  int pin;
  uint delay;
  int tolerance;
  int state;
  int samples;
  int delay_between_samples;

public:
  AnalogInput(
    String topic,
    int pin,
    uint delay = 100,
    int tolerance = 50,
    TSK_PRT task_priority = TSK_PRT::P_M,
    int samples = 10,
    int delay_between_samples = 10
  ) {
    this->topic = topic;
    this->pin = pin;
    this->delay = delay;
    this->tolerance = tolerance;
    this->task_priority = task_priority;
    this->samples = samples;
    this->delay_between_samples = delay_between_samples;
  }

  static void task(void* param) {
    AnalogInput* pThis = (AnalogInput*)param;

    while (true) {
      int sampleSum = 0;
      for (int i = 0; i < pThis->samples; i++) {
        sampleSum += analogRead(pThis->pin);
        vTaskDelay(pThis->delay_between_samples / portTICK_PERIOD_MS);
      }

      int currentState = int(sampleSum / pThis->samples);

      if (currentState > pThis->state + pThis->tolerance || currentState < pThis->state - pThis->tolerance) {
        pThis->state = currentState;

        char c_state[4];
        sprintf(c_state, "%d", pThis->state);
        mqttClient.publish(pThis->topic.c_str(), c_state);
      }

      vTaskDelay(pThis->delay / portTICK_PERIOD_MS);
    }
  }

  void setup() {
  }

  void onCommand(String* payload) {
    if ((*payload) == "STATE") {
      int currentState = analogRead(this->pin);

      char c_state[4];
      sprintf(c_state, "%d", currentState);
      mqttClient.publish(this->topic.c_str(), c_state);
    }
  }

  BaseType_t start() {
    return xTaskCreate(
      &AnalogInput::task,
      this->topic.c_str(),
      4096,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
