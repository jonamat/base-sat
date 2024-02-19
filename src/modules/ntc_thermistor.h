#include <PubSubClient.h>

#include "../module.h"

extern PubSubClient mqttClient;

#define SAMPLING_RATE 10
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMP 298.15
#define BETA_K 3380
#define REFERENCE_RES_VAL 10000
#define ADC_RESOLUTION 4095
#define VOLTAGE 3.3

/**
 * @brief NTCThermistor
 * @details Read temperature from a NTC thermistor.
 *
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param delay Delay between each state check. Default is 1000ms.
 * @param tolerance Tolerance to trigger a state change. Default is 1*C.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available commands:
 * "STATE" publish the current state of the device
 *
 * TODO:
 * - Add interval
 *
 */
class NTCThermistor : public Module
{
private:
  int pin;
  int temperature;
  int tolerance;
  uint delay;

public:
  NTCThermistor(
    String topic,
    int pin,
    uint delay = 10,
    int tolerance = 1,
    TSK_PRT task_priority = TSK_PRT::P_M
  ) {
    this->topic = topic;
    this->pin = pin;
    this->delay = delay || 1000;
    this->tolerance = tolerance;
    this->task_priority = task_priority;
  }

  static void task(void* param) {
    NTCThermistor* pThis = (NTCThermistor*)param;

    while (true) {
      int samples = 0;
      for (int i = 0; i < SAMPLING_RATE; i++) {
        samples += analogRead(pThis->pin);
        vTaskDelay(pThis->delay / portTICK_PERIOD_MS);
      }
      float currentState = (float)samples / (float)SAMPLING_RATE;

      float tension = (currentState * VOLTAGE) / (float)ADC_RESOLUTION;
      float thermistorResistance = REFERENCE_RES_VAL / ((VOLTAGE / tension) - 1);
      float temperature = 1 / (1 / NOMINAL_TEMP + (1 / BETA_K) * log(thermistorResistance / NOMINAL_RESISTANCE)) - 273.15;

      if (temperature > pThis->temperature + pThis->tolerance || temperature < pThis->temperature - pThis->tolerance) {
        pThis->temperature = temperature;

        char c_temperature[4];
        sprintf(c_temperature, "%d", pThis->temperature);
        mqttClient.publish(pThis->topic.c_str(), c_temperature);
      }

      vTaskDelay(pThis->delay / portTICK_PERIOD_MS);
    }
  }


  void setup() {
  }

  void onCommand(String* payload) {
    if ((*payload) == "STATE") {
      char c_temperature[4];
      sprintf(c_temperature, "%d", this->temperature);
      mqttClient.publish(this->topic.c_str(), c_temperature);
    }
  }

  BaseType_t start() {
    return xTaskCreate(
      &NTCThermistor::task,
      this->topic.c_str(),
      2048,
      this,
      (UBaseType_t)this->task_priority,
      NULL);
  }
};
