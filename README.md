# Satellite base template

Use a single ESP32 to control multiple devices and sensors via MQTT.<br />
The ESP32 act as an MQTT bridge for your domotic system, and it's designed to be remotely managed and updated.<br />
It provides low level functionalities to develop IOT devices<br />

## Features

- Control multiple devices and sensors with a single ESP32
- Remote management and update
- Minimal configuration
- Extendable modules and easy to use
- Parallel tasks and low level functionalities

## Setup

1. Rename the file secrets.template.h to secrets.h and fill it with your wifi and mqtt credentials.

```cpp
#define WIFI_SSID "My wifi network"
#define WIFI_PASSWORD "My-wifi-password123"
```

2. Fill the basic configuration in the config.cpp file.

```cpp
#define SAT_NAME "satellite-1" // The name of the satellite
#define MQTT_SERVER "test.mosquitto.org"
```

3. Define your devices and sensors in the modules vector in the main.cpp file.

```cpp
std::vector<Module*> modules = {
  new AnalogOutputDAC("/lights", 33),
  new AnalogReader("/sun-tracer", 34),
  new Button("/light-switch", 35),
  new DigitalInput("/door-sensor", 36),
  new DigitalOutput("/heater", 37),
  new IROutput("/tv-remote", 38),
  new NTCThermistor("/temperature", 39, 10000, 10000),
  new PWM("/fan", 40, 1000, 8, 0, 1023)
};
```

4. Upload the code to your ESP32 and check the serial monitor for the satellite status.
```bash
pio run -t upload
```

## Modules

Modules are the basic building blocks of the satellite. They control 1 (or multiple) GPIO pins. They can be sensors or actuators. Each module has some basic commands. By default, the command topic is the module topic with the suffix `/set`. The module sends a message to the command topic when the state changes.

### DigitalInput

Check the state of a digital input pin. It sends a message to the defined topic when the state changes.

#### Commands
`STATE` - Publish the current state of the input pin.

### DigitalOutput

Set the state of a digital output pin. It listens to the command topic and changes the state of the pin when a message is received. A new message is sent to the main topic when the state changes.

#### Commands
`STATE` - Publish the current state of the output pin.
`ON` - Set the pin to HIGH.
`OFF` - Set the pin to LOW.

### AnalogReader

Read the value of an analog pin. It sends a message to the defined topic when the value changes.

#### Commands
`STATE` - Publish the current value of the analog pin.

### AnalogOutputDAC

! This module is available only on DAC pins, and for the ESP32 are 25 and 26.
Set the value of an analog output pin using the integrated DAC. It listens to the command topic and changes the value of the pin when a message is received. A new message is sent to the main topic when the value changes.

#### Commands
`STATE` - Publish the current value of the output pin.
`SET xxx` - Set the value of the output pin to xxx. The value must be between 0 and 255.
`ON` - Set the pin to 255.
`OFF` - Set the pin to 0.

### PWM
Set the value of an analog output pin using PWM. It listens to the command topic and changes the value of the pin when a message is received. A new message is sent to the main topic when the value changes.

#### Commands
`STATE` - Publish the current value of the output pin.
`SET xxx` - Set the value of the output pin to xxx. The value must be between 0 and 255.
`ON` - Set the pin to 255.
`OFF` - Set the pin to 0.

### IROutput
Send IR signals using the IRremote library. It listens to the command topic and sends the IR signal when a message is received.

#### Commands
`KEY xxx` - Send the IR signal with the code xxx. Available codes are currently: `ON`, `OFF`, `1` to `8`, `PLUS`, `MINUS`

### NTCThermistor
! WIP
Read the value of a thermistor and convert it to temperature. It sends a message to the defined topic when the value changes.

#### Commands
`STATE` - Publish the current value of the thermistor.

### Button
! WIP
Check the state of a button. It sends a message to the defined topic when the state changes. It use the Bounce2 library to debounce the button.

#### Commands
`STATE` - Publish the current state of the button.


## Licence

MIT
