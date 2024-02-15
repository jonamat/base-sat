#pragma once

#include "../secrets.h"

#define SAT_NAME "satellite-1"

#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_DEFAULT_QOS 0

#define MSG_BUFFER_SIZE 50
#define RECONNECTION_TIME 1000
#define HTTP_SERVER_PORT 80

#define COMMAND_SUBTOPIC "/set"
#define LED_BUILTIN 2
#define HEARTBEAT_DELAY 1000
#define SERIAL_BAUD_RATE 115200
