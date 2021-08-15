#pragma once

#include "BlinkState.hpp"

#include "../utils/NoMoveNoCopy.hpp"

class LEDs_t : NoMoveNoCopy
{
private:
    constexpr static uint32_t BLINK_PERIOD = 600;
    enum LED_STATE
    {
        LED_STATE_EXCELLENT,
        LED_STATE_OK,
        LED_STATE_WARN_WEAK,
        LED_STATE_WARN_STRONG,
        LED_STATE_ERROR,
        LED_STATE_NONE,
    };

public:
    LEDs_t();
    void init();
    void update();

private:
    LED_STATE compute_led_state();
    void update_leds();

private:
    BlinkState blink_state;
    ValueWatcher<bool> blink_value;
    ValueWatcher<uint16_t> co2_value;
};

extern LEDs_t LEDs;
