/***************************************************
** Blinking
** On Initialization
** 1 short blink = board started
** 1 Blink Sensor initialising (may loop here till sensor comes good)
** 2 Blinks: WiFi initialising
** 3 Blinks MQTT initialising
** 4 Blinks: Relay initialising
** 5 Blinks: Bad sensor read
** 1 blink as temp successfully sent to Server
**/

#include "ArduinoTrace.h"
#include <Wire.h>
#include <HTU21D.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LED           2
#define relay_pin_1 15

// WiFi parameters
const char* ssid = "SolarBlue";
const char* password = "7c7e5188";
const int wifiMaxRetries = 20;

// MQTT Paramaters
WiFiClient espClient;
PubSubClient MQTTclient(espClient);
const char* mqttServer = "192.168.1.54";
const int mqttPort = 1883;
const char* mqttUser = "yourInstanceUsername";
const char* mqttPassword = "yourInstancePassword";
const char* mqttTempTopic = "SolarBlue/engine/alternator/temp";
const char* mqttHumidTopic = "SolarBlue/engine/alternator/humid";
const char* mqttFanTopic = "SolarBlue/engine/alternator/fan"; // topic to turn on relay
const int mqttMaxRetries = 20;

// HTU21D paramaters
const int htu21dMaxRetries = 10;
const int htuSda = 21;
const int htuScl = 22;
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);

void blinkIt(int ledPin, int numberBlinks, int delayMs) {
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
/*
   HTU21D(resolution)

   resolution:
   HTU21D_RES_RH12_TEMP14 - RH: 12Bit, Temperature: 14Bit, by default
   HTU21D_RES_RH8_TEMP12  - RH: 8Bit,  Temperature: 12Bit
   HTU21D_RES_RH10_TEMP13 - RH: 10Bit, Temperature: 13Bit
   HTU21D_RES_RH11_TEMP11 - RH: 11Bit, Temperature: 11Bit
 */


void setupHDU21D(int sda, int scl) {
        TRACE();
        /* HTU21D connection check */
        Wire.flush();
        Wire.begin(sda,scl);
        int htu21dRetries = htu21dMaxRetries;
        while (myHTU21D.begin() != true)
        {
                if (htu21dRetries-- > 0) {
                        blinkIt(LED, 1, 0); // indicate waiting for sensor
                        TRACE();
                        delay(5000);
                } else {
                        Serial.println("Could NOT connect to Temp Sensor..");

                        ESP.restart();
                }
        }
        Serial.println("HTU21D sensor is active");
}
void relay(int relayPin, bool swonoff) {
        TRACE();
        if (swonoff == true) {
                digitalWrite(relayPin, HIGH);
        } else {
                digitalWrite(relayPin, LOW);
        }
}

void setupRelay(int relayPin) {
        TRACE();
        pinMode(relayPin, OUTPUT);

}

void conWiFi() {
        TRACE();
        WiFi.disconnect(true);

        WiFi.begin(ssid, password);
        int wifiRetries = wifiMaxRetries;
        while (WiFi.status() != WL_CONNECTED) {
                if (wifiRetries-- > 0) {
                        blinkIt(LED, 2, 0); // indicate waiting for WiFi
                        delay(500);
                        Serial.println("Connecting to WiFi..");
                } else {
                        Serial.println("Could NOT connect to WiFi..");
                        WiFi.disconnect(true); // still not erasing the ssid/pw. Will happily reconnect on next start
                        WiFi.begin("0","0"); // adding this effectively seems to erase the previous stored SSID/PW
                        ESP.restart();
                }
        }

        Serial.println("Connected to the WiFi network");

}

void conMQTT(PubSubClient client, const char *server, int port) {
        TRACE();
        client.setServer(mqttServer, mqttPort);
        int mqttRetries = mqttMaxRetries;
        while (!client.connected()) {
          if (mqttRetries-- > 0) {
               blinkIt(LED, 3, 0); // indicate waiting for WiFi
                Serial.println("Connecting to MQTT...");

                if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

                        Serial.println("MQTT connected");

                } else {

                        Serial.print("MQTT failed with state ");
                        Serial.print(client.state());
                        delay(2000);
                }
              } else {
                Serial.println("Could NOT connect to MQTT..");
                ESP.restart();
              }
        }

}
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");

        if ((char)payload[0] == '1') {
                relay(relay_pin_1, true);
        } else {
                relay(relay_pin_1, false);
        }
        for (int i=0; i<length; i++) {
                Serial.print((char)payload[i]);
        }
        Serial.println();
}


/**
 * Blink onboard led defined beforehand number of times. It leaves the led off on completion
 * @param ledPin   led pin on ESP that led is attached to
 * @param   numberBlinks Number of blinks with 1/4 second between blinks
 */



void setup() {
        // put your setup code here, to run once:
        Serial.begin(115200);
        while(!Serial && !Serial.available()) {} // wait for console to come online
        TRACE();


// Blink the LED to say we are in
        blinkIt(LED, 1, 0);

        // Setup HTU21D
        setupHDU21D(htuSda,htuScl);

        // Connect to WiFi
        conWiFi();

        // Connect to MQTT
        conMQTT(MQTTclient, mqttServer, mqttPort);
        MQTTclient.setCallback(MQTTcallback);
        MQTTclient.subscribe(mqttFanTopic);

        // Setup relayPin
        setupRelay(relay_pin_1);


}



void loop() {
        // put your main code here, to run repeatedly:
        StaticJsonBuffer<300> JSONbuffer;
        char JSONmessageBuffer[100];
        float temperatureRead;



        TRACE();
        temperatureRead = myHTU21D.readTemperature(HTU21D_TRIGGER_TEMP_MEASURE_NOHOLD);
        int htu21dRetries = htu21dMaxRetries;
        while (temperatureRead == 255) {
          if (htu21dRetries-- > 0) {
                  blinkIt(LED, 5, 0); // indicate bad sensor read
                  TRACE();
                  delay(200);
          } else {
                  Serial.println("Could NOT read The Temp Sensor..");

                  ESP.restart();
          }

          myHTU21D.readTemperature(HTU21D_TRIGGER_TEMP_MEASURE_NOHOLD);
        }
        // Serial.print(F("Compensated Humidity: "));
        // Serial.print(myHTU21D.readHumidity());
        // Serial.println(" +-2%");

        Serial.print(F("Temperature.........: ")); Serial.print(temperatureRead);
        Serial.println(" +-0.5C");


        // Set up JSON
        // Send off Temperature
        JsonObject& JSONencoder = JSONbuffer.createObject();
        // JSONencoder["device"] = "ESP32";
        // JSONencoder["sensorType"] = "Temperature";
        JSONencoder["Value"] = temperatureRead;
        // JSONencoder["sensorType"] = "Humidity";
        // JSONencoder["Value"] = myHTU21D.readHumidity();


        // JsonArray& values = JSONencoder.createNestedArray("values");
        // values.add(20);
        // values.add(21);
        // values.add(23);


        JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println("Sending message to MQTT topic..");
        Serial.println(JSONmessageBuffer);

        if (MQTTclient.publish(mqttTempTopic, JSONmessageBuffer) == true) {
                Serial.println("Success sending message");
        } else {
                Serial.println("Error sending message");
        }

        // Send off Humidity
        //   JsonObject& JSONencoder = JSONbuffer.createObject();
        // JSONencoder["device"] = "ESP32";
        // JSONencoder["sensorType"] = "Humidity";
        // JSONencoder["Value"] = myHTU21D.readHumidity();

        // JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        // Serial.println("Sending message to MQTT topic..");
        // Serial.println(JSONmessageBuffer);

        // if (MQTTclient.publish(mqttHumidTopic, JSONmessageBuffer) == true) {
        //         Serial.println("Success sending message");
        //         blinkIt(LED, 2, 0); //Blink LED to show we sent it
        // } else {
        //         Serial.println("Error sending message");
        //         blinkIt(LED, 4, 0); //Blink LED to show we failed to send
        //}


blinkIt(LED, 1, 0);  // Blink once every message sent
        Serial.println("-------------");

        for (int i=0; i<5; i++) {
                MQTTclient.loop(); // check for incoming messages
                delay(200);
        }



}
