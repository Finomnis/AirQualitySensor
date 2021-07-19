#include "HomieDevice.hpp"

#include "../wifi_credentials.hpp"
#include "../utils/TimeHelpers.hpp"
#include "../sensors/DHT22/DHT22.hpp"

#include "DeviceId.hpp"

HomieDevice_t::HomieDevice_t()
    : temperature_value{&SensorDHT22.get_temperature_value()},
      humidity_value{&SensorDHT22.get_humidity_value()}
{
}

void HomieDevice_t::init()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    client.begin(MQTT_SERVER, net);
}

void HomieDevice_t::do_connection_maintenance()
{
    // If WiFi is not connected, be sad and do nothing
    if (WiFi.status() != WL_CONNECTED)
    {
        connected = false;
        return;
    }

    // If WiFi is connected but MQTT isn't, try to reconnect
    if (!client.connected())
    {
        connected = false;

        if (!event_is_over(next_connection_attempt))
        {
            next_connection_attempt = millis() + 500u;
            return;
        }

        char topic_buffer[128] = {};
        snprintf(topic_buffer, sizeof(topic_buffer), "homie/%s/$state", get_device_identifier());
        client.setWill(topic_buffer, "lost", false, LWMQTT_QOS1);

        if (!client.connect(get_device_identifier()))
            return;
    }

    if (!connected)
    {
        connected = true;
        on_connected();
    }
}

void HomieDevice_t::publish(const char *topic, const char *payload)
{
    char topic_buffer[128] = {};

    snprintf(topic_buffer, sizeof(topic_buffer), "homie/%s/%s", get_device_identifier(), topic);
    client.publish(topic_buffer, payload, true, LWMQTT_QOS1);
}

void HomieDevice_t::publish_values(bool force)
{
    char value_buffer[64];

    bool error = false;

    if (temperature_value.new_value_available() || force)
    {
        if (temperature_value.is_valid())
        {
            snprintf(value_buffer, sizeof(value_buffer), "%.01f", temperature_value.get());
            publish("dht22/temperature", value_buffer);
        }
        else
        {
            error = true;
        }
    }

    if (humidity_value.new_value_available() || force)
    {
        if (humidity_value.is_valid())
        {
            snprintf(value_buffer, sizeof(value_buffer), "%.01f", humidity_value.get());
            publish("dht22/humidity", value_buffer);
        }
        else
        {
            error = true;
        }
    }

    if (error && !previous_errors)
    {
        publish("$state", "alert");
    }

    if (previous_errors && !error)
    {
        publish("$state", "ready");
    }

    previous_errors = error;
}

void HomieDevice_t::on_connected()
{
    previous_errors = false;

    // Initial values
    publish("$state", "init");
    publish("$homie", "4.0.0");
    publish("$name", "Air Quality Sensor");
    publish("$extensions", "");

    // Set node topics
    publish("$nodes", "dht22,s8lp");

    // DHT22
    publish("dht22/$name", "DHT22");
    publish("dht22/$type", "dht22");
    publish("dht22/$properties", "temperature,humidity");

    // DHT22/temperature
    publish("dht22/temperature/$name", "Temperature");
    publish("dht22/temperature/$datatype", "float");
    publish("dht22/temperature/$format", "-40:80");
    publish("dht22/temperature/$unit", "°C");

    // DHT22/humidity
    publish("dht22/humidity/$name", "Humidity");
    publish("dht22/humidity/$datatype", "float");
    publish("dht22/humidity/$format", "0:100");
    publish("dht22/humidity/$unit", "%");

    // S8LP
    publish("s8lp/$name", "S8LP");
    publish("s8lp/$type", "s8lp");
    publish("s8lp/$properties", "co2");

    // S8LP/co2
    publish("s8lp/co2/$name", "CO2");
    publish("s8lp/co2/$datatype", "float");
    publish("s8lp/co2/$format", "0:10000");
    publish("s8lp/co2/$unit", "ppm");

    // Finalize initialization
    publish("$state", "ready");

    // Publish values
    publish_values(true);
}

void HomieDevice_t::update()
{
    client.loop();
    delay(10); // <- fixes some issues with WiFi stability

    // Reconnect if necessary
    do_connection_maintenance();

    // Update values
    publish_values();
}

HomieDevice_t HomieDevice{};
