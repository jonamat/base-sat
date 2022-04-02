#pragma once

/* Log via Serial and MQTT /events topic  */
void log(const String msg);

/* Log via Serial and MQTT /events topic  */
void log(const char* msg);

enum class STATE {
  STATE_LOW = LOW,
  STATE_HIGH = HIGH,
};
