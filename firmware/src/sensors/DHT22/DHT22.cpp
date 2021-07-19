#include "DHT22.hpp"

#include "../../pinout.hpp"

#include "../../utils/TimeHelpers.hpp"

SensorDHT22_t::SensorDHT22_t()
    : dht{PINS::AM2302, DHT22}
{
}

void SensorDHT22_t::init()
{
    dht.begin();

    // Get measurement period
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    measurement_period = sensor.min_delay / 1000;
}

void SensorDHT22_t::update()
{
    if (event_is_over(next_measurement))
    {
        next_measurement = millis() + measurement_period;

        sensors_event_t event;

        // Get temperature data
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature))
        {
            temperature_value.update(0, false);
        }
        else
        {
            temperature_value.update(event.temperature);
        }

        // Get humidity data
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity))
        {
            humidity_value.update(0, false);
        }
        else
        {
            humidity_value.update(event.relative_humidity);
        }
    }
}

SensorDHT22_t SensorDHT22{};
