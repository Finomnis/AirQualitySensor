#pragma once

#include <Adafruit_NeoPixel.h>

class StatusLEDs
{
public:
    enum Status
    {
        EXCELLENT = 1,
        OK = 2,
        WARNING_WEAK = 3,
        WARNING_STRONG = 4,
        ERROR = 5,
        NONE = 6,
    };

    StatusLEDs(uint16_t num_leds);
    void setup();
    void startUpdate();
    void addStatus(uint16_t led_id, Status status);
    void finishUpdate();

private:
    Adafruit_NeoPixel leds;
};
