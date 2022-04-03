#include <Arduino.h>
#include <PubSubClient.h>

#include "utils.h"
#include "config.h"

extern PubSubClient mqttClient;

void log(String msg)
{
  if (Serial)
    Serial.println(msg);

  if (mqttClient.connected())
    mqttClient.publish(SYSTEM_NAME SAT_NAME "/events", msg.c_str());
}

void log(char* msg)
{
  if (Serial)
    Serial.println(msg);

  if (mqttClient.connected())
    mqttClient.publish(SYSTEM_NAME SAT_NAME "/events", msg);
}