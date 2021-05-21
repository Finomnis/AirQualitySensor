#include "connection_state.hpp"

ConnectionState::ConnectionState()
{
}

void ConnectionState::setup()
{
    Homie.onEvent([this](const HomieEvent &event) { handleHomieEvent(event); });
}

void ConnectionState::handleHomieEvent(const HomieEvent &event)
{
    switch (event.type)
    {
    case HomieEventType::WIFI_CONNECTED:
        wifi_connected = true;
        return;
    case HomieEventType::WIFI_DISCONNECTED:
        wifi_connected = false;
        return;
    case HomieEventType::MQTT_READY:
        mqtt_connected = true;
        return;
    case HomieEventType::MQTT_DISCONNECTED:
        mqtt_connected = false;
        return;
    default:
        return;
    }
}

StatusLEDs::Status ConnectionState::update()
{
    if (!wifi_connected)
    {
        return StatusLEDs::NONE;
    }
    if (!mqtt_connected)
    {
        return StatusLEDs::WARNING_STRONG;
    }
    return StatusLEDs::OK;
}
