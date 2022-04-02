#pragma once

#include <Arduino.h>

class Module
{
public:
  virtual void setup() = 0;
  virtual void startTask() = 0;
  virtual void onCommand(String* payload) = 0;

  String name;
  String topic;
};
