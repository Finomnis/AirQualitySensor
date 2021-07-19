#pragma once

#include "../utils/NoMoveNoCopy.hpp"
#include "../utils/ValueWatcher.hpp"

#include <WiFi.h>
#include <MQTT.h>

class HomieDevice_t : NoMoveNoCopy
{
public:
    HomieDevice_t();
    void init();
    void update();

private:
    void do_connection_maintenance();
    void on_connected();

    void publish(const char *topic, const char *payload);

    void publish_values(bool force = false);

private:
    WiFiClient net;
    MQTTClient client;
    uint32_t next_connection_attempt{0};
    bool connected{false};

    ValueWatcher<float> temperature_value;
    ValueWatcher<float> humidity_value;

    bool previous_errors{false};
};

extern HomieDevice_t HomieDevice;
