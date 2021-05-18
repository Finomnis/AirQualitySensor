
#include "sensor_AM2302.hpp"

#include <Homie.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

SensorAM2302::SensorAM2302(uint8_t pin, const char *id, const char *name)
    : homieNode{id, name, "airsensor"},
      am2302{pin, DHT22},
      delayMS{0},
      nextUpdate{0},
      ledStatus{StatusLEDs::ERROR}
{
}

void SensorAM2302::setup()
{
    // Initialize device.
    Homie.getLogger() << F("Initializing AM2302 ...") << endl;
    am2302.begin();

    // Get sensor polling frequency
    sensor_t sensor;
    am2302.temperature().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;

    Homie.getLogger() << F("Got sensor polling delay of ") << delayMS << F(" ms.") << endl;

    // Initialize homie
    homieNode.advertise("temperature").setName("Temperature").setDatatype("float").setUnit("ºC");
    homieNode.advertise("humidity").setName("Humidity").setDatatype("float").setUnit("%");

    Homie.getLogger() << F("AM2302 initialization finished.") << endl;
}

StatusLEDs::Status SensorAM2302::update()
{
    if (millis() > nextUpdate)
    {
        // Get temperature event and broadcast its value.
        sensors_event_t event;
        am2302.temperature().getEvent(&event);
        if (isnan(event.temperature))
        {
            Homie.getLogger() << F("Error reading temperature!") << endl;
        }
        else
        {
            if (Homie.isConnected())
            {
                homieNode.setProperty("temperature").send(String(event.temperature));
            }
        }

        // Get humidity event and broadcast its value.
        am2302.humidity().getEvent(&event);
        if (isnan(event.relative_humidity))
        {
            Homie.getLogger() << F("Error reading humidity!") << endl;
            ledStatus = StatusLEDs::ERROR;
        }
        else
        {
            if (Homie.isConnected())
            {
                homieNode.setProperty("humidity").send(String(event.relative_humidity));
            }

            if (event.relative_humidity < 47.0)
            {
                ledStatus = StatusLEDs::EXCELLENT;
            }
            else if (event.relative_humidity < 55.0)
            {
                ledStatus = StatusLEDs::OK;
            }
            else if (event.relative_humidity < 60.0)
            {
                ledStatus = StatusLEDs::WARNING_WEAK;
            }
            else
            {
                ledStatus = StatusLEDs::WARNING_STRONG;
            }
        }

        nextUpdate = millis() + delayMS;
    }

    return ledStatus;
}
