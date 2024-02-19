#include <Arduino.h>
#include <AsyncElegantOTA.h>

#include "ota.h"
#include "const.h"

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];

void init_ota(AsyncElegantOtaClass* ota, AsyncWebServer* webServer) {
  (*ota).begin(webServer);
}
