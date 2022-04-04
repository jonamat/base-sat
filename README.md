# base-sat

*WIP*<br />
MQTT bridge interface for ESP32, with cloud-free over-the-air updates.<br />
It provides low level functionalities to develop IOT devices<br />
Base template for rvmatic satellites

## Setup

Add devices by creating a new desired module object and put in the *modules* vector.

```cpp
std::vector<Module*> modules = {
  new Actuator("Attic lights", SYSTEM_NAME "/led_attic", 33),
  new PWM("Thermal blanket", SYSTEM_NAME "/thermal_blanket", 22),
  new AnalogReader("Temperature", SYSTEM_NAME "/temperature", 27)
};
```

## System structure

## RVmatic integration

## Generic MQTT brokers integration

## Supported boards

## Licence
