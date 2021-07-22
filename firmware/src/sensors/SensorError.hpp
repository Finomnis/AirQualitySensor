#pragma once

#include "../utils/ValueWatcher.hpp"
#include "../utils/NoMoveNoCopy.hpp"

class SensorError_t : NoMoveNoCopy
{
public:
    SensorError_t();
    void update();
    Value<bool> const &get_value() { return value; }

private:
    ValueWatcher<float> temperature_sensor;
    ValueWatcher<float> humidity_sensor;
    ValueWatcher<uint16_t> co2_sensor;

    Value<bool> value;
};

extern SensorError_t SensorError;
