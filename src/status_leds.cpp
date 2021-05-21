#include "status_leds.hpp"

#include "pinout.hpp"

#include <algorithm>

StatusLEDs::StatusLEDs(uint16_t num_leds)
    : leds{num_leds, PINS::STATUS_LEDS, NEO_GRB + NEO_KHZ400},
      currentStatus{num_leds},
      currentColor{num_leds}
{
    std::fill(currentStatus.begin(), currentStatus.end(), Status::NONE);
    std::fill(currentColor.begin(), currentColor.end(), 0x0);
}

void StatusLEDs::setup()
{
    leds.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    leds.show();            // Turn OFF all pixels ASAP
    leds.setBrightness(10); // Set brightness
}

void StatusLEDs::startUpdate()
{
    std::fill(currentStatus.begin(), currentStatus.end(), Status::NONE);
}

void StatusLEDs::addStatus(uint16_t led_id, StatusLEDs::Status status)
{
    if (currentStatus[led_id] < status)
    {
        currentStatus[led_id] = status;
    }
}

uint32_t StatusLEDs::statusToColor(StatusLEDs::Status status)
{
    bool blinkState = (millis() / 600) % 2 == 0;

    switch (status)
    {
    case StatusLEDs::Status::NONE:
        return leds.Color(0, 0, 0);
    case StatusLEDs::Status::EXCELLENT:
        return blinkState ? leds.Color(0, 255, 0) : leds.Color(0, 50, 0);
    case StatusLEDs::Status::OK:
        return leds.Color(0, 255, 0);
    case StatusLEDs::Status::WARNING_WEAK:
        return leds.Color(255, 150, 0);
    case StatusLEDs::Status::WARNING_STRONG:
        return leds.Color(255, 0, 0);
    case StatusLEDs::Status::ERROR:
        return blinkState ? leds.Color(255, 0, 0) : leds.Color(50, 0, 0);
    default:
        return leds.Color(150, 150, 150);
    }
}

void StatusLEDs::finishUpdate()
{
    bool needsUpdate = false;
    for (int i = 0; i < leds.numPixels(); i++)
    {
        uint32_t color = statusToColor(currentStatus[i]);
        if (color != currentColor[i])
        {
            currentColor[i] = color;
            needsUpdate = true;
        }
    }

    if (needsUpdate)
    {
        for (int i = 0; i < leds.numPixels(); i++)
        {
            leds.setPixelColor(i, currentColor[i]);
        }
        leds.show();
    }
}
