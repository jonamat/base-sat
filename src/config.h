#pragma once

/*
secrets.h template:
#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "passwd"
*/
#include "../secrets.h"

#define SYSTEM_NAME "camperA123"
#define SAT_NAME "ganymede"

#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_DEFAULT_QOS 0

#define MSG_BUFFER_SIZE 50
#define RECONNECTION_TIME 1000
#define HTTP_SERVER_PORT 80

#define TASK_LOW_PRIORITY 1
#define TASK_MEDIUM_PRIORITY 2
#define TASK_HIGH_PRIORITY 3
