// based on https://gist.github.com/yaqwsx/ac662c9b600ef39a802da0be1b25d32d
// 2018.07.14 jnogues@gmail.com, Jaume Nogués, rPrim Tech
// This sketch shows the use of 802.11 LR (Low Rate)
// master.ino

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>

#define OnBoardLedPin 2

const char* ssid = "SolarSky";//AP ssid
const char* password = "12345678A";//AP password
const char* ssidRouter = "SolarBlue";//STA router ssid
const char* passwordRouter = "7c7e5188";//STA router password
WiFiUDP udp;
long rssi = 0;
char udpMessage [256];
int loopCount = 0;

void setup() {


        pinMode(OnBoardLedPin, OUTPUT);//builtin Led, for debug
        digitalWrite(OnBoardLedPin, HIGH);
        Serial.begin( 115200 );
        Serial.setDebugOutput(true);
        Serial.println( "Master" );

        WiFi.disconnect(true);
        delay(1000);

        //first, we start STA mode and connect to router
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
        udp.begin( 8888 );
}

void loop()
{
        // udp.beginPacket( { 192, 168, 4, 2 }, 8888 );//send a  message
        udp.beginPacket( { 192, 168, 4, 255 }, 8888 );//send a broadcast message
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
                ESP.restart(); // When the connection is bad, the TCP stack refuses to work
        }
        else{
                Serial.print("SENT IT!! ");
                Serial.println(udpMessage);
        }

        delay( 1000 );//wait a second for the next message
}
