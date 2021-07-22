#pragma once

#include "../utils/ValueWatcher.hpp"

class Sensors_t
{
public:
    void init();
    void update();

    Value<float> const &get_temperature_value();
    Value<float> const &get_humidity_value();
    Value<uint16_t> const &get_co2_value();
};

extern Sensors_t Sensors;
