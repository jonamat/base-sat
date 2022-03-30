#include <Arduino.h>
#include <PubSubClient.h>

#include "config.h"
#include "utils.h"

extern PubSubClient mqttClient;

void log(const String msg)
{
  if (Serial)
    Serial.println(msg);

  if (mqttClient.connected())
    mqttClient.publish(SAT_NAME "/events", msg.c_str());
}

void log(const char* msg)
{
  if (Serial)
    Serial.println(msg);

  if (mqttClient.connected())
    mqttClient.publish(SAT_NAME "/events", msg);
}

