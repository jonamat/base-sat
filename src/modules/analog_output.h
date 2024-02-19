#include <PubSubClient.h>

#include "../module.h"

extern PubSubClient mqttClient;

/**
 * @brief AnalogOutput
 * @details Provides a digital output on a pin.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 * "ON" set the state to 255
 * "OFF" set the state to 0
 * "SET xx" set the state to xx (0 to 255)
 *
 */
class AnalogOutputDAC : public Module
{
private:
  int pin;
  uint8_t state; // 0 to 255

public:
  AnalogOutputDAC(
    String topic,
    int pin
  ) {

    /*
    ESP32	  | DAC_CH_1 | DAC_CH_2
    ESP32	  | GPIO25	 | GPIO26
    ESP32-S2|	GPIO17	 | GPIO18

    check for esp32-s2
    */

    if (pin != 25 || pin != 26) {
      throw std::runtime_error("Pin " + std::to_string(pin) + " does is not provided with DAC. Cannot create AnalogOutput module on this pin. Please use pin 25 or 26.");
    }

    this->topic = topic;
    this->pin = pin;
  }

  void setup() {}

  void onCommand(String* payload) {
    if ((*payload).startsWith("SET")) {
      int value = payload->substring(4).toInt();

      if (value < 0 || value > 255) {
        mqttClient.publish(String(this->topic + "/error").c_str(), "Value must be between 0 and 255");
        return;
      }

      this->state = value;
      dacWrite(this->pin, value);
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else if ((*payload) == "STATE") {
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else if ((*payload) == "ON") {
      this->state = 255;
      dacWrite(this->pin, 255);
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else if ((*payload) == "OFF") {
      this->state = 0;
      dacWrite(this->pin, 0);
      mqttClient.publish(this->topic.c_str(), String(this->state).c_str());
    }
    else {
      mqttClient.publish(String(this->topic + "/error").c_str(), "Unknown command");
    }
  }

  BaseType_t start() {
    dacWrite(this->pin, (int)this->state);
    return pdPASS;
  }
};
