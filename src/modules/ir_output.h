#include <PubSubClient.h>
#include <IRremote.hpp>

#include "../module.h"

#define RAW_DATA_LEN 68
#define KHZ 38

uint16_t rawDataKeyOn[RAW_DATA_LEN] = {
  9100,
  4400, 650,
  500, 650, 500, 600, 500, 650, 500, 650,
  450, 650, 500, 650, 450, 650, 500, 650,
  1600, 650, 1600, 650, 1600, 650, 1600, 650,
  1600, 650, 1650, 600, 1650, 650, 1600, 650,
  1600, 650, 450, 650, 1600, 650, 500, 650,
  500, 600, 500, 650, 1600, 650, 500, 650,
  450, 650, 1600, 650, 500, 650, 1600, 650,
  1600, 650, 1600, 650, 500, 650, 1600, 650,
  39350
};

uint16_t rawDataKeyOff[RAW_DATA_LEN] = {
  9100,
  4450, 600,
  550, 600, 500, 600, 550, 600, 500, 600,
  550, 600, 550, 600, 500, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 1700, 550, 1700, 600, 1650, 600,
  1650, 600, 1650, 600, 1650, 600, 550, 600,
  500, 600, 550, 600, 1650, 600, 550, 550,
  550, 600, 550, 600, 500, 600, 1650, 600,
  1700, 600, 1650, 600, 500, 600, 1650, 600,
  39400
};

uint16_t rawDataKeyPlus[RAW_DATA_LEN] = {
  9150,
  4400, 650,
  500, 600, 550, 600, 500, 650, 500, 600,
  500, 650, 500, 600, 500, 650, 500, 600,
  1650, 600, 1650, 650, 1600, 650, 1600, 650,
  1600, 650, 1600, 650, 1600, 650, 1650, 600,
  500, 650, 1600, 650, 500, 600, 1650, 600,
  1650, 600, 500, 650, 1650, 600, 500, 600,
  1650, 650, 500, 600, 1650, 600, 500, 650,
  500, 600, 1650, 600, 550, 600, 1650, 600,
  39350
};

uint16_t rawDataKeyMinus[RAW_DATA_LEN] = {
  9100,
  4450, 600,
  550, 600, 500, 600, 550, 600, 500, 650,
  500, 600, 550, 600, 500, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 1650, 600, 1650, 650, 1600, 650,
  500, 600, 550, 600, 500, 600, 1650, 600,
  550, 600, 500, 600, 550, 600, 500, 650,
  1650, 600, 1650, 600, 1650, 600, 500, 600,
  1650, 600, 1650, 650, 1650, 600, 1650, 600,
  39350
};

uint16_t rawDataKey1[RAW_DATA_LEN] = {
  9100,
  4450, 650,
  500, 600, 500, 650, 500, 600, 550, 600,
  500, 600, 550, 600, 500, 650, 500, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 650, 1600, 650, 1650, 600, 1650, 600,
  500, 600, 550, 600, 1650, 600, 500, 650,
  500, 600, 550, 600, 1650, 600, 500, 600,
  1650, 650, 1600, 650, 500, 600, 1650, 600,
  1650, 600, 1650, 600, 550, 600, 1650, 600,
  39400
};

uint16_t rawDataKey2[RAW_DATA_LEN] = {
  9150,
  4400, 650,
  500, 600, 500, 650, 500, 600, 550, 600,
  500, 600, 550, 600, 500, 650, 500, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 650, 1600, 650, 1600, 650, 1600, 650,
  1600, 650, 1600, 650, 500, 600, 550, 600,
  500, 650, 500, 600, 1650, 600, 500, 650,
  500, 600, 500, 650, 1600, 650, 1600, 650,
  1650, 600, 1650, 600, 500, 650, 1600, 650,
  39350
};

uint16_t rawDataKey3[RAW_DATA_LEN] = {
  9100,
  4450, 600,
  550, 600, 500, 600, 550, 600, 500, 600,
  550, 600, 550, 600, 500, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 1700, 550, 1700, 550, 550, 600,
  550, 600, 500, 600, 550, 600, 500, 600,
  550, 600, 500, 600, 550, 600, 1650, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  39400
};


uint16_t rawDataKey4[RAW_DATA_LEN] = {
    9050,
  4450, 600,
  550, 600, 500, 600, 550, 600, 550, 550,
  550, 600, 550, 600, 500, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 550, 600, 500, 600, 1650, 600,
  550, 600, 550, 550, 550, 600, 550, 600,
  500, 600, 1650, 600, 1650, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  39350
};

uint16_t rawDataKey5[RAW_DATA_LEN] = {
    9100,
  4450, 600,
  500, 600, 550, 600, 500, 650, 500, 600,
  500, 650, 500, 600, 500, 650, 500, 600,
  1650, 600, 1650, 600, 1650, 650, 1600, 650,
  1600, 650, 1600, 650, 1600, 650, 1600, 650,
  500, 600, 1650, 600, 1650, 600, 550, 600,
  1650, 600, 500, 650, 500, 600, 500, 650,
  1600, 650, 500, 600, 550, 600, 1650, 600,
  500, 600, 1650, 650, 1600, 650, 1600, 650,
  39350
};

uint16_t rawDataKey6[RAW_DATA_LEN] = {
  9100,
  4450, 650,
  500, 600, 550, 600, 500, 600, 550, 600,
  500, 650, 500, 600, 500, 650, 500, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 650,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 600, 500, 650, 1600, 650, 1650, 600,
  500, 600, 550, 600, 500, 600, 550, 600,
  500, 650, 1600, 650, 500, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  39350
};

uint16_t rawDataKey7[RAW_DATA_LEN] = {
  9100,
  4450, 600,
  500, 650, 500, 600, 550, 600, 500, 600,
  550, 600, 500, 600, 550, 600, 500, 600,
  1650, 650, 1600, 650, 1600, 650, 1600, 650,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  500, 650, 500, 600, 1650, 600, 1650, 600,
  550, 600, 500, 650, 500, 600, 500, 650,
  1600, 650, 1600, 650, 500, 600, 550, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  39350
};

uint16_t rawDataKey8[RAW_DATA_LEN] = {
  9100,
  4450, 600,
  500, 600, 550, 600, 500, 600, 550, 600,
  500, 650, 500, 600, 550, 600, 500, 600,
  1650, 600, 1650, 600, 1650, 600, 1650, 600,
  1650, 650, 1600, 650, 1650, 600, 1650, 600,
  500, 600, 1650, 600, 1650, 600, 1650, 650,
  1600, 650, 500, 600, 1650, 600, 550, 600,
  1650, 600, 500, 600, 550, 600, 500, 650,
  500, 600, 1650, 600, 550, 600, 1650, 600,
  39350
};



extern PubSubClient mqttClient;

/**
 * @brief IROutput
 * @details Control an IR LED to send IR commands.
 *
 * @param topic Root topic of the device.
 * @param pin Output pin to use.
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 * "ON" set the state to HIGH
 * "OFF" set the state to LOW
 *
 */
class IROutput : public Module
{
private:
  int pin;
  String state;

public:
  IROutput(
    String topic,
    int pin
  ) {
    this->topic = topic;
    this->pin = pin;
  }

  void setup() {
    pinMode(this->pin, INPUT_PULLUP);
    IrSender.begin(this->pin);
  }

  void onCommand(String* payload) {
    if ((*payload).startsWith("KEY")) {
      String key = (*payload).substring(4);

      if (key == "ON") {
        IrSender.sendRaw(rawDataKeyOn, RAW_DATA_LEN, KHZ);
        this->state = "ON";
        mqttClient.publish(this->topic.c_str(), "ON");
      }
      else if (key == "OFF") {
        IrSender.sendRaw(rawDataKeyOff, RAW_DATA_LEN, KHZ);
        this->state = "OFF";
        mqttClient.publish(this->topic.c_str(), "OFF");
      }
      else if (key == "PLUS") {
        IrSender.sendRaw(rawDataKeyPlus, RAW_DATA_LEN, KHZ);
        this->state = "PLUS";
        mqttClient.publish(this->topic.c_str(), "PLUS");
      }
      else if (key == "MINUS") {
        IrSender.sendRaw(rawDataKeyMinus, RAW_DATA_LEN, KHZ);
        this->state = "MINUS";
        mqttClient.publish(this->topic.c_str(), "MINUS");
      }
      else if (key == "1") {
        IrSender.sendRaw(rawDataKey1, RAW_DATA_LEN, KHZ);
        this->state = "1";
        mqttClient.publish(this->topic.c_str(), "1");
      }
      else if (key == "2") {
        IrSender.sendRaw(rawDataKey2, RAW_DATA_LEN, KHZ);
        this->state = "2";
        mqttClient.publish(this->topic.c_str(), "2");
      }
      else if (key == "3") {
        IrSender.sendRaw(rawDataKey3, RAW_DATA_LEN, KHZ);
        this->state = "3";
        mqttClient.publish(this->topic.c_str(), "3");
      }
      else if (key == "4") {
        IrSender.sendRaw(rawDataKey4, RAW_DATA_LEN, KHZ);
        this->state = "4";
        mqttClient.publish(this->topic.c_str(), "4");
      }
      else if (key == "5") {
        IrSender.sendRaw(rawDataKey5, RAW_DATA_LEN, KHZ);
        this->state = "5";
        mqttClient.publish(this->topic.c_str(), "5");
      }
      else if (key == "6") {
        IrSender.sendRaw(rawDataKey6, RAW_DATA_LEN, KHZ);
        this->state = "6";
        mqttClient.publish(this->topic.c_str(), "6");
      }
      else if (key == "7") {
        IrSender.sendRaw(rawDataKey7, RAW_DATA_LEN, KHZ);
        this->state = "7";
        mqttClient.publish(this->topic.c_str(), "7");
      }
      else if (key == "8") {
        IrSender.sendRaw(rawDataKey8, RAW_DATA_LEN, KHZ);
        this->state = "8";
        mqttClient.publish(this->topic.c_str(), "8");
      }
    }
    else if ((*payload) == "STATE") {
      mqttClient.publish(this->topic.c_str(), this->state.c_str());
    }
  }

  BaseType_t start() {
    return pdPASS;
  }
};
