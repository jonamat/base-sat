#pragma once

/* Init MQTT client */
void InitMQTT(PubSubClient &mqttClient);

/* Connect MQTT client and reconnect if connection is lost  */
void MqttConnectionLoop(PubSubClient &mqttClient);