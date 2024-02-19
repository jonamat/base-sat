#pragma once
#include <vector>

#include "module.h"
#include "modules/analog_output.h"
#include "modules/analog_input.h"
#include "modules/button.h"
#include "modules/digital_input.h"
#include "modules/digital_output.h"
#include "modules/ir_output.h"
#include "modules/ntc_thermistor.h"
#include "modules/pwm.h"

/*
 * Customize the following definitions to match your setup
 * This file is ignored by git, so you can keep updated with the upstream base-sat repo
 */

std::vector<Module*> devices = {
  new AnalogOutputDAC("/lights", 33),
  new AnalogInput("/sun-tracer", 34),
  new Button("/light-switch", 35),
  new DigitalInput("/door-sensor", 36),
  new DigitalOutput("/heater", 37),
  new IROutput("/tv-remote", 38),
  new NTCThermistor("/temperature", 39, 10000, 10000),
  new PWM("/fan", 40, 1000, 8, 0, 1023)
};
