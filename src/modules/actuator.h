#include <PubSubClient.h>

#include "../module.h"
#include "../utils.h"

class Actuator : public Module
{
private:
    int pin;
    STATE state;
    PubSubClient* mqttClient;

public:
    static void task(void* param) {
        Actuator* pThis = (Actuator*)param;

        while (true) {
            Serial.println(pThis->state == STATE::STATE_HIGH ? "ON" : "OFF");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

    Actuator(String name, String topic, PubSubClient* mqttClient, int pin, STATE initState = STATE::STATE_LOW)
    {
        this->name = name;
        this->topic = topic;

        this->pin = pin;
        this->state = initState;
        this->mqttClient = mqttClient;
    }

    void setup()
    {
        pinMode(this->pin, OUTPUT);
    }

    void onCommand(String* payload)
    {
        if ((*payload) == "on" && this->state == STATE::STATE_LOW)
        {
            digitalWrite(this->pin, HIGH);
            this->state = STATE::STATE_HIGH;
        }
        else if ((*payload) == "off" && this->state == STATE::STATE_HIGH)
        {
            digitalWrite(this->pin, LOW);
            this->state = STATE::STATE_LOW;
        }
    }

    void startTask()
    {
        xTaskCreate(
            &Actuator::task,
            this->name.c_str(),
            2048,
            this,
            TASK_HIGH_PRIORITY,
            NULL);
    }
};
