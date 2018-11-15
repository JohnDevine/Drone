/**
 * Debug display of MQTT status
 * @method MQTTStatusDisplay
 * @param  mqtt_status    Status from call
 */

/////////////////////////NOTE DEFINE FOR DEBUGGING!!!!! ///////////////
 #define dbg
///////////////////////////////////////////////////////////////////////

 #include <WiFi.h>
 #include <WiFiUdp.h>
 #include <esp_wifi.h>
#include "helperDebug.h"

/**
 * Blink onboard led defined beforehand number of times. It leaves the led off on completion
 * @param ledPin   led pin on ESP that led is attached to
 * @param   numberBlinks Number of blinks with 1/4 second between blinks
 */

void helperDebug::blinkIt(int ledPin, int numberBlinks, int delayMs) {
        pinMode(ledPin, OUTPUT);//builtin Led, for  debug
        digitalWrite(ledPin, LOW);
        for (int j = 0; j < numberBlinks; j++) {
                digitalWrite(ledPin, HIGH);
                delay(500);
                digitalWrite(ledPin, LOW);
                delay(500);
        }
        digitalWrite(ledPin, LOW);
        if (delayMs > 0) {
                delay(delayMs); // Do this so we can tell blinks apart
        }

}

void helperDebug::MQTTStatusDisplay(int mqtt_status) {
        #ifdef dbg
        switch (mqtt_status) {
        case MQTT_CONNECTION_TIMEOUT:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECTION_TIMEOUT");
                break;
        case MQTT_CONNECTION_LOST:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECTION_LOST");
                break;
        case MQTT_CONNECT_FAILED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECT_FAILED");
                break;
        case MQTT_DISCONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_DISCONNECTED");
                break;
        case MQTT_CONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECTED");
                break;
        case MQTT_CONNECT_BAD_PROTOCOL:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECT_BAD_PROTOCOL");
                break;
        case MQTT_CONNECT_BAD_CLIENT_ID:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECT_BAD_CLIENT_ID");
                break;
        case MQTT_CONNECT_UNAVAILABLE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECT_UNAVAILABLE");
                break;
        case MQTT_CONNECT_BAD_CREDENTIALS:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECT_BAD_CREDENTIALS");
                break;
        case MQTT_CONNECT_UNAUTHORIZED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("MQTT_CONNECT_UNAUTHORIZED");
                break;
        default:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.print( "Other MQTT status = " );
                Serial.println( mqtt_status );
                break;
        }
        #endif
}

/**
 * Debug detail print of status returned from calls
 * @param status                  The status returned from call
 * @date  2018-10-23T18:36:49+070
 */
void helperDebug::WiFiStatusDisplay(wl_status_t status) {
        #ifdef dbg
        switch (status) {
        case WL_CONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status -assigned when connected to a WiFi network");
                break;
        case WL_NO_SHIELD:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - assigned when no WiFi shield is present");
                break;
        case WL_IDLE_STATUS:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED)");
                break;
        case WL_NO_SSID_AVAIL:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - assigned when no SSID are available");
                break;
        case WL_SCAN_COMPLETED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - assigned when the scan networks is completed");
                break;
        case WL_CONNECT_FAILED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - assigned when the connection fails for all the attempts");
                break;
        case WL_CONNECTION_LOST:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - assigned when the connection is lost");
                break;
        case WL_DISCONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println(" Status - assigned when disconnected from a network");
                break;
        default:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.print( "Status - Unknown = " );
                Serial.println( status );
                break;
        }
        #endif

}

/**
 * Debug display of WiFi event actions
 * @param event                   Return from an event .. NB EVENT not Status
 * @date  2018-10-23T18:38:23+070
 */
void helperDebug::WiFiEventDisplay(WiFiEvent_t event) {
        #ifdef dbg
        switch (event) {
        case SYSTEM_EVENT_WIFI_READY:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_WIFI_READY: ESP32 WiFi ready");
                break;
        case SYSTEM_EVENT_SCAN_DONE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_SCAN_DONE: ESP32 finish scanning AP");
                break;
        case SYSTEM_EVENT_STA_START:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_START: ESP32 station start");
                break;
        case SYSTEM_EVENT_STA_STOP:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_STOP: ESP32 station stop");
                break;
        case SYSTEM_EVENT_STA_CONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_CONNECTED: ESP32 station connected to AP");
                break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_DISCONNECTED: ESP32 station disconnected from AP");
                break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_AUTHMODE_CHANGE: the auth mode of AP connected by ESP32 station changed");
                break;
        case SYSTEM_EVENT_STA_GOT_IP:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_GOT_IP: ESP32 station got IP from connected AP");
                break;
        case SYSTEM_EVENT_STA_LOST_IP:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_LOST_IP: ESP32 station lost IP and the IP is reset to 0");
                break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_WPS_ER_SUCCESS: ESP32 station wps succeeds in enrollee mode");
                break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_WPS_ER_FAILED: ESP32 station wps fails in enrollee mode");
                break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_WPS_ER_TIMEOUT: ESP32 station wps timeout in enrollee mode");
                break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_STA_WPS_ER_PIN: ESP32 station wps pin code in enrollee mode");
                break;
        case SYSTEM_EVENT_AP_START:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_AP_START: ESP32 soft-AP start");
                break;
        case SYSTEM_EVENT_AP_STOP:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_AP_STOP: ESP32 soft-AP stop");
                break;
        case SYSTEM_EVENT_AP_STACONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_AP_STACONNECTED: a station connected to ESP32 soft-AP");
                break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_AP_STADISCONNECTED: a station disconnected from ESP32 soft-AP");
                break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_AP_PROBEREQRECVED: Receive probe request packet in soft-AP interface");
                break;
        case SYSTEM_EVENT_GOT_IP6:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_GOT_IP6: ESP32 station or ap or ethernet interface v6IP addr is preferred");
                break;
        case SYSTEM_EVENT_ETH_START:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_ETH_START: ESP32 ethernet start");
                break;
        case SYSTEM_EVENT_ETH_STOP:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_ETH_STOP: ESP32 ethernet stop");
                break;
        case SYSTEM_EVENT_ETH_CONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_ETH_CONNECTED: ESP32 ethernet phy link up");
                break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_ETH_DISCONNECTED: ESP32 ethernet phy link down");
                break;
        case SYSTEM_EVENT_ETH_GOT_IP:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("SYSTEM_EVENT_ETH_GOT_IP: ESP32 ethernet got IP from connected AP");
                break;
        default:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.print( "Other event = " );
                Serial.println( event );
                break;
        }
        #endif
}

void helperDebug::WiFiRunStatus(esp_err_t status) {
        #ifdef dbg
        switch (status) {
        case ESP_OK:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t value indicating success (no error)");
                break;
        case ESP_FAIL:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("Generic esp_err_t code indicating failure");
                break;

        case ESP_ERR_NO_MEM:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Out of memory");
                break;
        case ESP_ERR_INVALID_ARG:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Invalid argument");
                break;
        case ESP_ERR_INVALID_STATE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Invalid state");
                break;
        case ESP_ERR_INVALID_SIZE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Invalid size");
                break;
        case ESP_ERR_NOT_FOUND:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Requested resource not found");
                break;
        case ESP_ERR_NOT_SUPPORTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Operation or feature not supported");
                break;
        case ESP_ERR_TIMEOUT:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t code Operation timed out");
                break;
        case ESP_ERR_INVALID_RESPONSE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t Received response was invalid");
                break;

        case ESP_ERR_INVALID_CRC:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t CRC or checksum was invalid");
                break;

        case ESP_ERR_INVALID_VERSION:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t Version was invalid");
                break;
        case ESP_ERR_INVALID_MAC:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t MAC address was invalid");
                break;
        case ESP_ERR_WIFI_NOT_INIT:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi driver was not installed by esp_wifi_init");
                break;
        case ESP_ERR_WIFI_NOT_STARTED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi driver was not started by esp_wifi_start");
                break;
        case ESP_ERR_WIFI_NOT_STOPPED:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi driver was not stopped by esp_wifi_stop");
                break;
        case ESP_ERR_WIFI_IF:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t  WiFi interface error");
                break;
        case ESP_ERR_WIFI_MODE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t  WiFi mode error");
                break;
        case ESP_ERR_WIFI_STATE:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi internal state error ");
                break;
        case ESP_ERR_WIFI_CONN:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi internal control block of station or soft-AP error ");
                break;
        case ESP_ERR_WIFI_NVS:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi internal NVS module error");
                break;
        case ESP_ERR_WIFI_MAC:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t  MAC address is invalid");
                break;
        case ESP_ERR_WIFI_SSID:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t  SSID is invalid");
                break;
        case ESP_ERR_WIFI_PASSWORD:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t Password is invalid");
                break;
        case ESP_ERR_WIFI_TIMEOUT:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t Timeout error");
                break;
        case ESP_ERR_WIFI_WAKE_FAIL:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t WiFi is in sleep state(RF closed) and wakeup fail");
                break;
        case ESP_ERR_WIFI_WOULD_BLOCK:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t The caller would block");
                break;
        case ESP_ERR_WIFI_NOT_CONNECT:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.println("esp_err_t Station still in disconnect status");
                break;
        default:
                Serial.printf("file = %s, line = %d \n", __FILE__, __LINE__);
                Serial.print( "esp_err_t Other event = " );
                Serial.println( status );
                break;
        }
        #endif
}
