
#include "sensor_AM2302.hpp"

#include <Homie.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4
#define DHTTYPE DHT22 // DHT 22 (AM2302)

namespace
{
    HomieNode homieNode("am2302", "AM2302", "airsensor");
    DHT_Unified am2302(DHTPIN, DHTTYPE);
    unsigned long delayMS;
    unsigned long nextUpdate = 0;
}

namespace SensorAM2302
{
    void setup()
    {
        // Initialize device.
        Serial.println(F("Initializing AM2302 ..."));
        am2302.begin();

        // Get sensor polling frequency
        sensor_t sensor;
        am2302.temperature().getSensor(&sensor);
        delayMS = sensor.min_delay / 1000;

        Serial.print(F("Got sensor polling delay of "));
        Serial.print(delayMS);
        Serial.println(F(" ms."));

        // Initialize homie
        homieNode.advertise("temperature").setName("Temperature").setDatatype("float").setUnit("ºC");
        homieNode.advertise("humidity").setName("Humidity").setDatatype("float").setUnit("%");

        Serial.println("AM2302 initialization finished.");
    }

    void update()
    {
        if (millis() > nextUpdate)
        {
            // Get temperature event and broadcast its value.
            sensors_event_t event;
            am2302.temperature().getEvent(&event);
            if (isnan(event.temperature))
            {
                Serial.println(F("Error reading temperature!"));
            }
            else
            {
                homieNode.setProperty("temperature").send(String(event.temperature));
            }

            // Get humidity event and broadcast its value.
            am2302.humidity().getEvent(&event);
            if (isnan(event.relative_humidity))
            {
                Serial.println(F("Error reading humidity!"));
            }
            else
            {
                homieNode.setProperty("humidity").send(String(event.relative_humidity));
            }

            nextUpdate = millis() + delayMS;
        }
    }
}