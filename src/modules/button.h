#include <PubSubClient.h>
#include <Bounce2.h>

#include "../module.h"

extern PubSubClient mqttClient;

/**
 * @brief Button
 * @details Read the state changes of a button handling debounce and send the new state on change.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param mode Mode to use. Default is PULL_DOWN.
 * @param debounce_ms Debounce time in milliseconds. Default is 50ms.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 *
 */
class Button : public Module
{
private:
  int pin;
  ST state;
  Bounce button;
  MODE mode;
  unsigned int debounce_ms;

public:
  Button(
    String topic,
    int pin,
    MODE mode = MODE::PULL_DOWN,
    unsigned int  debounce_ms = 100,
    TSK_PRT task_priority = TSK_PRT::P_M
  ) {
    this->topic = topic;
    this->pin = pin;
    this->mode = mode;
    this->debounce_ms = debounce_ms;
    this->task_priority = task_priority;
  }

  static void task(void* param) {
    Button* pThis = (Button*)param;

    while (true) {
      pThis->button.update();

      if (pThis->button.read() != (int)pThis->state) {
        mqttClient.publish(pThis->topic.c_str(), pThis->state == ST::ST_H ? "ON" : "OFF");
        pThis->state = (ST)pThis->button.read();
      }

      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }

  void setup() {
    this->button = Bounce();
    this->button.attach(this->pin, (int)this->mode);
    this->button.interval(this->debounce_ms);
  }

  void onCommand(String* payload) {
    if ((*payload) == "STATE") {
      String state_topic = this->topic + "/state";
      mqttClient.publish(state_topic.c_str(), this->state == ST::ST_H ? "ON" : "OFF");
    }
  }

  BaseType_t start() {
    this->state = (ST)this->button.read();

    return xTaskCreate(
      &Button::task,
      this->topic.c_str(),
      2048,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
