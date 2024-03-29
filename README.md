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

## Installation

1. Install PlatformIO
```bash
pip install -U platformio
```

2. Clone the repository
```bash
git clone git@github.com:jonamat/base-sat.git
```

## Update

The repo is designed to keep your configuration (include/config.h and include/devices.h) separated from the main code. You can update the repo without losing your configuration by pulling the latest changes from the repository. If you have made changes to the main code, you can stash them before pulling the changes and then apply them back.

```bash
git stash # Stash your changes if you have made any in the main code (in src folder)
git pull
git stash apply # Apply your changes back
```

## Setup

1. Rename the `include/config.template.h` file to `include/config.h` and fill your configuration.

```bash
mv include/config.template.h include/config.h
```

2. Fill the basic configuration in the `include/config.h` file.

```cpp
#define SAT_NAME "sat-1"
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"
#define MQTT_SERVER "test.mosquitto.org"
```

3. Rename the `include/devices.template.h` file to `include/devices.h` and define your devices and sensors.

```bash
mv include/devices.template.h include/devices.h
```

1. Edit the vector `devices` in the `include/devices.h` file to insert your modules. See [Modules](#modules) to know how to do.

```cpp
std::vector<Module*> devices = {
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

---

### DigitalInput

Check the state of a digital input pin. It sends a message to the defined topic when the state changes.

#### Commands

| Command | Description                                 |
| ------- | ------------------------------------------- |
| STATE   | Publish the current state of the input pin. |

---

### DigitalOutput

Set the state of a digital output pin. It listens to the command topic and changes the state of the pin when a message is received. A new message is sent to the main topic when the state changes.

#### Commands

| Command | Description                                  |
| ------- | -------------------------------------------- |
| STATE   | Publish the current state of the output pin. |
| ON      | Set the pin to HIGH.                         |
| OFF     | Set the pin to LOW.                          |

---

### AnalogReader

Read the value of an analog pin. It sends a message to the defined topic when the value changes.

#### Commands

| Command | Description                                  |
| ------- | -------------------------------------------- |
| STATE   | Publish the current value of the analog pin. |

---

### AnalogOutputDAC

! This module is available only on DAC pins, and for the ESP32 are 25 and 26.
Set the value of an analog output pin using the integrated DAC. It listens to the command topic and changes the value of the pin when a message is received. A new message is sent to the main topic when the value changes.

#### Commands

| Command | Description                                        |
| ------- | -------------------------------------------------- |
| STATE   | Publish the current value of the output pin.       |
| SET xxx | Set the value of the output pin to xxx (0 to 255). |
| ON      | Set the pin to 255.                                |
| OFF     | Set the pin to 0.                                  |

---

### PWM

Set the value of an analog output pin using PWM. It listens to the command topic and changes the value of the pin when a message is received. A new message is sent to the main topic when the value changes.

#### Commands

| Command | Description                                        |
| ------- | -------------------------------------------------- |
| STATE   | Publish the current value of the output pin.       |
| SET xxx | Set the value of the output pin to xxx (0 to 255). |
| ON      | Set the pin to 255.                                |
| OFF     | Set the pin to 0.                                  |

---

### IROutput

Send IR signals using the IRremote library. It listens to the command topic and sends the IR signal when a message is received.

#### Commands

| Command | Description                              |
| ------- | ---------------------------------------- |
| KEY xxx | Send the IR signal with the code xxx.    |
|         | Available codes are currently:           |
|         | `ON`, `OFF`, `1` to `8`, `PLUS`, `MINUS` |

---

### NTCThermistor 🚧

Read the value of a thermistor and convert it to temperature. It sends a message to the defined topic when the value changes.

#### Commands

| Command | Description                                  |
| ------- | -------------------------------------------- |
| STATE   | Publish the current value of the thermistor. |

---

### Button 🚧

Check the state of a button. It sends a message to the defined topic when the state changes. It uses the Bounce2 library to debounce the button.

#### Commands

| Command | Description                              |
| ------- | ---------------------------------------- |
| STATE   | Publish the current state of the button. |



## Licence

MIT
