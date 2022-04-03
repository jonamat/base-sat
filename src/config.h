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

/* Task priority */
enum class TSK_PRT {
  P_L = 1,
  P_M = 2,
  P_H = 3,
};
