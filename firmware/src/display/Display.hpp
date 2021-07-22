#pragma once

#include <U8g2lib.h>

#include "../utils/ValueWatcher.hpp"

class Display_t
{
    static constexpr uint32_t STARTUP_DISPLAY_TIME = 10000;

public:
    Display_t();

    void init();
    void update();

private:
    void redraw();
    void render_startup_screen();

private:
    U8G2 &display;

    bool startup_finished;
    uint32_t startup_end;

    ValueWatcher<float> temperature_value;
    ValueWatcher<float> humidity_value;
    ValueWatcher<uint16_t> co2_value;
};

extern Display_t Display;
