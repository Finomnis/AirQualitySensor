#pragma once

#include "../utils/ValueWatcher.hpp"

class Startup : public ValueWatcher<bool>
{
public:
    Startup(uint32_t duration);
    void update();

private:
    Value<bool> value;
    uint32_t startup_end;
    ValueWatcher<bool> sensor_error_value;
};
