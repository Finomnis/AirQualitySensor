#pragma once

#include <U8g2lib.h>

#include "../utils/ValueWatcher.hpp"

class Display_t
{
public:
    Display_t();

    void init();
    void update();

private:
    U8G2 &display;

    ValueWatcher<float> temperature_value;
    ValueWatcher<float> humidity_value;
    ValueWatcher<uint16_t> co2_value;
};

extern Display_t Display;
