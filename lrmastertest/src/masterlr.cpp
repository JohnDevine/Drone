// based on https://gist.github.com/yaqwsx/ac662c9b600ef39a802da0be1b25d32d
// 2018.07.14 jnogues@gmail.com, Jaume Nogués, rPrim Tech
// This sketch shows the use of 802.11 LR (Low Rate)
// master.ino

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>


#include <helperDebug.h>

#define OnBoardLedPin 2

const char* ssid = "SolarSky";//AP ssid
const char* password = "12345678A";//AP password
const char* ssidRouter = "SolarBlue";//STA router ssid
const char* passwordRouter = "7c7e5188";//STA router password
WiFiUDP udp;
int udpReadCount = 0;
char udpMessage [256];
int udpinPacketsize;
uint8_t udpBeginSts = 0;
long rssi = 0;

uint16_t remotePortID = 0;
const uint16_t remotePortIDDefault = 888;
int loopCount = 0;
helperDebug debugIt;

/*
 * WiFi Events
 * https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/wifi.html

   SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
   SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
   SYSTEM_EVENT_STA_START                < ESP32 station start
   SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
   SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
   SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
   SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
   SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
   SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
   SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
   SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
   SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
   SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
   SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
   SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
   SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
   SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
   SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
   SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
   SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
   SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
   SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
   SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
   SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
   SYSTEM_EVENT_MAX
 */

void WiFiEvent(WiFiEvent_t event)
{
        Serial.printf("[WiFi-event] event: %d\n", event);

        debugIt.WiFiEventDisplay(event);
        debugIt.blinkIt(OnBoardLedPin, 2, 500);

        switch (event) {
        case SYSTEM_EVENT_AP_STACONNECTED:
                remotePortID = udp.remotePort();
                if (remotePortID == 0)
                        remotePortID = remotePortIDDefault;
                // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
                udpBeginSts = udp.begin( remotePortID );
                Serial.print("Remote port = ");
                Serial.println(remotePortID);
                Serial.print("udp Begin status  = ");
                Serial.println(udpBeginSts);
                break;
        default:
                break;
        }

}

void setup() {


        pinMode(OnBoardLedPin, OUTPUT);//builtin Led, for debug
        digitalWrite(OnBoardLedPin, HIGH);
        debugIt.blinkIt(OnBoardLedPin, 4, 1000);
        Serial.begin( 115200 );
        Serial.setDebugOutput(true);
        Serial.println( "Master" );

        WiFi.disconnect(true);
        delay(1000);

        //first, we start STA mode and connect to router
        WiFi.onEvent(WiFiEvent);
        WiFi.mode( WIFI_AP_STA );
        WiFi.begin(ssidRouter,passwordRouter);



        //Wifi connection
        while (WiFi.status() != WL_CONNECTED)
        {
                delay(500);
                Serial.print(".");
        }
        WiFi.printDiag(Serial);
        Serial.println("---------1---------");
        Serial.print("Channel = ");
        Serial.println(WiFi.channel());
        rssi = WiFi.RSSI();
        Serial.print("RSSI = ");
        Serial.println(rssi);
        Serial.println("Router WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

        //second, we start AP mode with LR protocol
        //This AP ssid is not visible whith our regular devices
        WiFi.mode( WIFI_AP );//for AP mode
        //here config LR mode
        int a= esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_LR );
        // int a= esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N );
        Serial.println(a);



        WiFi.softAP(ssid, password, WiFi.channel());
        Serial.println( WiFi.softAPIP() );
        Serial.println("#");//for debug
        delay( 1000 );
        WiFi.printDiag(Serial);
        Serial.println("---------2---------");
        digitalWrite(OnBoardLedPin, LOW);

}

void loop()
{
        debugIt.blinkIt(OnBoardLedPin, 2, 500);
        // Start processing the next available incoming packet
        // Returns the size of the packet in bytes, or 0 if no packets are available
        udpinPacketsize = udp.parsePacket();
        if ( udpinPacketsize == 0 ) {
                delay( 1000 );//wait a second for the next message
                return;
        }

        // Read up to len bytes from the current packet and place them into buffer
        // Returns the number of bytes read, or 0 if none are available
        udpinPacketsize = udp.read(udpMessage, 255);

        if ( udpinPacketsize > 0 ) {
                digitalWrite(OnBoardLedPin, !digitalRead(OnBoardLedPin));//toggle Led
                Serial.print("RECEIVED: ");
                Serial.println(udpMessage);

        } else {
                Serial.println("RECEIVED but no data");
        }
        // Finish reading the current packet
        udp.flush();
        delay( 1000 );//wait a second for the next message
}
