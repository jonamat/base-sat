#pragma once

/* Init MQTT client */
void init_mqtt(PubSubClient *mqttClient);

/* Connect MQTT client and reconnect if connection is lost  */
void mqtt_connection_task(void *parameter);