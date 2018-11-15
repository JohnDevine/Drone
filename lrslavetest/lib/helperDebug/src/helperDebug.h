/*
   helperDebug.h - Debugging for ESP.
   John Devine

 */

#ifndef helperDebug_h
#define helperDebug_h

#include <PubSubClient.h>


class helperDebug {
private:
public:
  void blinkIt(int ledPin, int numberBlinks, int delayMs);
  void MQTTStatusDisplay(int mqtt_status);
  void WiFiStatusDisplay(wl_status_t status);
  void WiFiEventDisplay(WiFiEvent_t event);
  void WiFiRunStatus(esp_err_t status);
};


#endif
