#pragma once

#include "core-modules/module.h"
#include <vector>

/* Init MQTT client */
void init_mqtt(PubSubClient* mqttClient);

/* Connect MQTT client and reconnect if connection is lost  */
void mqtt_connection_task(void* parameter);

std::function<void(char*, uint8_t*, unsigned int)> mqtt_register_callbacks(PubSubClient* _mqttClient, std::vector<Module*> _modules);
