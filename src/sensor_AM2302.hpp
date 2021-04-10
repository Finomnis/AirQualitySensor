#pragma once

#include <Homie.hpp>
#include <DHT_U.h>

#include "status_leds.hpp"

class SensorAM2302
{
public:
    SensorAM2302(uint8_t pin, const char *id, const char *name);
    void setup();
    StatusLEDs::Status update();

private:
    HomieNode homieNode;
    DHT_Unified am2302;
    unsigned long delayMS;
    unsigned long nextUpdate;

    StatusLEDs::Status ledStatus;
};