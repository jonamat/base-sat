#include <Arduino.h>
#include <PubSubClient.h>

#include "../module.h"
#include "../utils.h"

extern PubSubClient mqttClient;

/**
 * @brief PWM
 * @details Provides a PWM output on a DAC pin.
 *
 * @param name Name of the device.
 * @param topic Root topic of the device.
 * @param pin Pin to use.
 * @param init_state Starting state from 0 to 255.
 * @param channel PWM channel to use. Default is 0
 * @param freq_hz Frequency of the PWM signal. Default is 5000Hz.
 * @param duty_res Duty resolution. Default is 8 bit.
 * @param task_priority Priority of the task. Default is P_M (medium).
 *
 * @note Available command payloads:
 * "STATE" publish the current state of the device on /state
 * "[uint8_t]" set the state to [uint8_t]
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
    PWM(String name, String topic, int pin, uint8_t init_state = 0, uint8_t channel = 0, int freq_hz = 5000, uint8_t duty_res = 8, TSK_PRT task_priority = TSK_PRT::P_M) {
        this->name = name;
        this->topic = topic;
        this->pin = pin;
        this->state = init_state;
        this->channel = channel;
        this->freq_hz = freq_hz;
        this->duty_res = duty_res;
        this->task_priority = task_priority;
    }

    void setup() {
        ledcSetup(this->channel, this->freq_hz, this->duty_res);
        ledcAttachPin(this->pin, this->channel);
    }

    void onCommand(String* payload) {
        if ((*payload) == "STATE") {
            String state_topic = this->topic + "/state";
            char c_state[4];
            sprintf(c_state, "%d", this->state);
            mqttClient.publish(state_topic.c_str(), c_state);
        }
        else {
            ledcWrite(this->channel, (*payload).toInt());
        }
    }

    BaseType_t start() {
        ledcWrite(this->channel, this->state);
        return pdPASS;
    }
};
