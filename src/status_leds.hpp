#pragma once

#include <Adafruit_NeoPixel.h>
#include <vector>

class StatusLEDs
{
public:
    enum Status
    {
        NONE = 1,
        EXCELLENT = 2,
        OK = 3,
        WARNING_WEAK = 4,
        WARNING_STRONG = 5,
        ERROR = 6,
    };

    StatusLEDs(uint16_t num_leds);
    void setup();
    void startUpdate();
    void addStatus(uint16_t led_id, Status status);
    void finishUpdate();

private:
    uint32_t statusToColor(Status status);

private:
    Adafruit_NeoPixel leds;
    std::vector<Status> currentStatus;
    std::vector<uint32_t> currentColor;
};
