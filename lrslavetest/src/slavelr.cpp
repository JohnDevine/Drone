
// based on https://gist.github.com/yaqwsx/ac662c9b600ef39a802da0be1b25d32d
// 2018.07.14 jnogues@gmail.com, Jaume Nogués, rPrim Tech
// This sketch shows the use of 802.11 LR (Low Rate)
// slave.ino

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>

#include <helperDebug.h>

#define OnBoardLedPin 2
const char* ssid = "SolarSky";//AP ssid
const char* password = "12345678A";//AP password

WiFiUDP udp;
int udpReadCount = 0;
char udpMessage [256];
const uint16_t remotePortIDDefault = 888;
int loopCount = 0;
long rssi = 0;

helperDebug debugIt;

const char *toStr( wl_status_t status ) {
        switch( status ) {
        case WL_NO_SHIELD: return "No shield";
        case WL_IDLE_STATUS: return "Idle status";
        case WL_NO_SSID_AVAIL: return "No SSID avail";
        case WL_SCAN_COMPLETED: return "Scan compleded";
        case WL_CONNECTED: return "Connected";
        case WL_CONNECT_FAILED: return "Failed";
        case WL_CONNECTION_LOST: return "Connection lost";
        case WL_DISCONNECTED: return "Disconnected";
        }
        return "Unknown";
}

void setup() {
        Serial.begin( 115200 );
        Serial.setDebugOutput(true);
        Serial.println( "Slave" );
        pinMode(OnBoardLedPin, OUTPUT);        //bultin Led, for debug
        debugIt.blinkIt(OnBoardLedPin, 4, 1000);


        //We start STA mode with LR protocol
        //This ssid is not visible whith our regular devices
        WiFi.mode( WIFI_STA );//for STA mode
        //if mode LR config OK
        int a= esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_LR );
        // int a= esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N );
        if (a==0)
        {
                Serial.println(" ");
                Serial.print("Error = ");
                Serial.print(a);
                Serial.println(" , Mode LR OK!");
        }
        else//if some error in LR config
        {
                Serial.println(" ");
                Serial.print("Error = ");
                Serial.print(a);
                Serial.println(" , Error in Mode LR!");
        }

        WiFi.begin(ssid, password);//this ssid is not visible

        //Wifi connection, we connect to master
        while (WiFi.status() != WL_CONNECTED)
        {
                delay(500);
                Serial.print(".");
        }

        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

        udp.begin( remotePortIDDefault );
}

void loop() {
        //problems whith connection
        if ( WiFi.status() != WL_CONNECTED )
        {
                Serial.println( "|" );
                int tries = 0;
                WiFi.begin( ssid, password );
                while( WiFi.status() != WL_CONNECTED ) {
                        tries++;
                        if ( tries == 5 )
                                return;
                        Serial.println( toStr( WiFi.status() ) );
                        delay( 1000 );
                }
                Serial.print( "Connected " );
                Serial.println( WiFi.localIP() );
        }
        //if connection OK, execute command 'b' from master

        udp.beginPacket( { 192, 168, 4, 2 }, remotePortIDDefault );      //send a  message
        // udp.beginPacket( { 192, 168, 4, 255 }, 8888 );//send a broadcast message
        sprintf(udpMessage, "Count = %d", ++loopCount);
        udp.write((uint8_t *)udpMessage, strlen(udpMessage));
        // udp.write( 'b' );//the payload
        digitalWrite(OnBoardLedPin, !digitalRead(OnBoardLedPin));
        rssi = WiFi.RSSI();
        Serial.print("RSSI = ");
        Serial.println(rssi);

        if ( !udp.endPacket() ) {
                Serial.println("NOT SEND!");
                delay(100);
                // ESP.restart(); // When the connection is bad, the TCP stack refuses to work
        }
        else{
                Serial.print("SENT IT!! ");
                Serial.println(udpMessage);
        }
}
