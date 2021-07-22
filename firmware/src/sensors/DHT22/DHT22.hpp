#pragma once

#include "../../utils/ValueWatcher.hpp"
#include "../../utils/NoMoveNoCopy.hpp"

#include <DHT_U.h>

class SensorDHT22_t : NoMoveNoCopy
{
public:
    SensorDHT22_t();

    void init();
    void update();

    Value<float> const &get_temperature_value() { return temperature_value; }
    Value<float> const &get_humidity_value() { return humidity_value; }

private:
    Value<float> temperature_value{0, false};
    Value<float> humidity_value{0, false};
    DHT_Unified dht;

    uint32_t next_measurement{0};
    uint32_t measurement_period{2000};
};

extern SensorDHT22_t SensorDHT22;
