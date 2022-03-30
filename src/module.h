#pragma once

#include <Arduino.h>

class AbstractModuleBase
{
public:
  virtual void setup(String name, String topic) = 0;
  virtual void loop() = 0;
  virtual void onMessage(String payload) = 0;

  String name;
  String topic;
  String state;
};
