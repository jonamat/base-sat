#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

#include "config.h"

class Module
{
public:
  virtual void setup() = 0;
  virtual BaseType_t start() = 0;
  virtual void onCommand(String* payload) = 0;

  String name;
  String topic;
  TSK_PRT task_priority;
};

enum class ST {
  ST_L = LOW,
  ST_H = HIGH,
};

enum class MODE {
  PULL_UP = INPUT_PULLUP,
  PULL_DOWN = INPUT_PULLDOWN,
};
