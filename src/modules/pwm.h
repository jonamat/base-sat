#include <Arduino.h>
#include <PubSubClient.h>

#include "../module.h"

extern PubSubClient mqttClient;

/**
 * @brief PWM
 * @details Provides a PWM output on a digital pin.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param channel PWM channel to use. Default is 0
 * @param freq_hz Frequency of the PWM signal. Default is 5000Hz.
 * @param duty_res Duty resolution. Default is 8 bit.
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 * "SET xx" set the state to xx (0-255)
 *
 */
class PWM : public Module
{
private:
  int pin;
  int freq_hz;
  uint8_t channel;
  uint8_t state;
  uint8_t duty_res;

public:
  PWM(
    String topic,
    int pin,
    uint8_t init_state = 0,
    uint8_t channel = 0,
    int freq_hz = 5000,
    uint8_t duty_res = 8,
    TSK_PRT task_priority = TSK_PRT::P_M
  ) {
    this->topic = topic;
    this->pin = pin;
    this->state = init_state;
    this->channel = channel;
    this->freq_hz = freq_hz;
    this->duty_res = duty_res;
  }

  void setup() {
    ledcSetup(this->channel, this->freq_hz, this->duty_res);
    ledcAttachPin(this->pin, this->channel);
  }

  void onCommand(String* payload) {
    if ((*payload).startsWith("SET")) {
      int value = payload->substring(4).toInt();

      if (value < 0 || value > 255) {
        mqttClient.publish(this->topic.c_str(), "Invalid value. Value must be between 0 and 255.");
        return;
      }

      this->state = value;
      ledcWrite(this->channel, value);
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else if ((*payload) == "ON") {
      this->state = 255;
      ledcWrite(this->channel, this->state);
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else if ((*payload) == "OFF") {
      this->state = 0;
      ledcWrite(this->channel, this->state);
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else if ((*payload) == "STATE") {
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
  }

  BaseType_t start() {
    ledcWrite(this->channel, this->state);
    return pdPASS;
  }
};
