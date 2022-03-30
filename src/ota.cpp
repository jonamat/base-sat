#include <Arduino.h>
#include <AsyncElegantOTA.h>

#include "config.h"
#include "ota.h"

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];

void InitOTA(AsyncElegantOtaClass &ota, AsyncWebServer &webServer)
{
  ota.begin(&webServer);
}