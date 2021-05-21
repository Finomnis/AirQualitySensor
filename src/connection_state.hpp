#pragma once

#include "status_leds.hpp"

#include <Homie.hpp>

class ConnectionState
{
public:
    ConnectionState();
    void setup();
    StatusLEDs::Status update();

private:
    void handleHomieEvent(const HomieEvent &event);

private:
    bool wifi_connected{false};
    bool mqtt_connected{false};
};
