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
        // Actuator* pThis = (Actuator*)param;

        // while (true) {
        //     Serial.println(pThis->state == STATE::STATE_HIGH ? "ON" : "OFF");
        //     vTaskDelay(1000 / portTICK_PERIOD_MS);
        // }
    }

    Actuator(String name, String topic, PubSubClient* mqttClient, int pin, STATE initState = STATE::STATE_LOW, TSK_PRT task_priority = TSK_PRT::P_M)
    {
        this->name = name;
        this->topic = topic;
        this->task_priority = task_priority;
        this->mqttClient = mqttClient;
        this->pin = pin;
        this->state = initState;
    }

    void setup()
    {
        pinMode(this->pin, OUTPUT);
    }

    void onCommand(String* payload)
    {
        if ((*payload) == "ON" && this->state == STATE::STATE_LOW)
        {
            digitalWrite(this->pin, HIGH);
            this->state = STATE::STATE_HIGH;
        }
        else if ((*payload) == "OFF" && this->state == STATE::STATE_HIGH)
        {
            digitalWrite(this->pin, LOW);
            this->state = STATE::STATE_LOW;
        }
        else if ((*payload) == "STATE")
        {
            String state_topic = this->topic + "/state";
            (*this->mqttClient).publish(state_topic.c_str(), this->state == STATE::STATE_HIGH ? "ON" : "OFF");
        }
    }

    BaseType_t start()
    {
        // return xTaskCreate(
        //     &Actuator::task,
        //     this->name.c_str(),
        //     2048,
        //     this,
        //     (UBaseType_t)this->task_priority,
        //     NULL);

        return pdPASS;
    }
};
