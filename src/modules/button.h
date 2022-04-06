#include <PubSubClient.h>
#include <Bounce2.h>

#include "../module.h"
#include "../utils.h"

extern PubSubClient mqttClient;

/**
 * @brief Derivative of DigitalReader with debounce support.
 * @details Read the state changes of a button and send the new state on change.
 *
 * @param name Name of the device.
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param mode Mode to use. Default is PULL_UP.
 * @param debounce_ms Debounce time in milliseconds. Default is 50ms.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device on /state
 *
 * @note Available events:
 * on topic "/events" with payload "[ON|OFF]" the state of the button has changed
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
    String name,
    String topic,
    int pin,
    MODE mode = MODE::PULL_DOWN,
    unsigned int  debounce_ms = 100,
    TSK_PRT task_priority = TSK_PRT::P_M
  ) {
    this->name = name;
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
        String state_topic = pThis->topic + "/events";
        mqttClient.publish(state_topic.c_str(), pThis->state == ST::ST_H ? "ON" : "OFF");

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
      this->name.c_str(),
      2048,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
