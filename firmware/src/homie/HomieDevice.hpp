#pragma once

#include <WiFi.h>
#include <MQTT.h>

class HomieDevice_t
{
public:
    void init();
    void update();

private:
    void do_connection_maintenance();
    void on_connected();

    void publish(const char *topic, const char *payload);

private:
    WiFiClient net;
    MQTTClient client;
    uint32_t next_connection_attempt{0};
    bool connected{false};
};

extern HomieDevice_t HomieDevice;
