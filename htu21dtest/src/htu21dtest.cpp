#include "ArduinoTrace.h"
#include <Wire.h>
#include <HTU21D.h>

#define LED           2

/*
   HTU21D(resolution)

   resolution:
   HTU21D_RES_RH12_TEMP14 - RH: 12Bit, Temperature: 14Bit, by default
   HTU21D_RES_RH8_TEMP12  - RH: 8Bit,  Temperature: 12Bit
   HTU21D_RES_RH10_TEMP13 - RH: 10Bit, Temperature: 13Bit
   HTU21D_RES_RH11_TEMP11 - RH: 11Bit, Temperature: 11Bit
 */
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);

void setupHDU21D(int sda, int scl) {
        TRACE();
        /* HTU21D connection check */
        Wire.begin(sda,scl);
        while (myHTU21D.begin() != true)
        {
                TRACE();
                delay(5000);
        }
        Serial.println("HTU21D sensor is active");
}

void setup() {
        // put your setup code here, to run once:
        Serial.begin(115200);
        while(!Serial && !Serial.available()) {} // wait for console to come online
        TRACE();


// Blink the LED to say we are in
        pinMode(LED, OUTPUT);
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);

        // Setup HTU21D
        setupHDU21D(21,22);


}

void loop() {
        // put your main code here, to run repeatedly:
        TRACE();
        Serial.print(F("Compensated Humidity: "));
        Serial.print(myHTU21D.readHumidity());
        Serial.println(" +-2%");

        Serial.print(F("Temperature.........: ")); Serial.print(myHTU21D.readTemperature(HTU21D_TRIGGER_TEMP_MEASURE_NOHOLD));
        Serial.println(" +-0.5C");

        Serial.print("DEMO starts over again in 5 sec.");
        delay(5000);

}
