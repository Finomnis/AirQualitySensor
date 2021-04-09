#pragma once

#include "status_leds.hpp"

namespace SensorAM2302
{
    void setup();
    StatusLEDs::Status update();
}