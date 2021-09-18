#include "dht22.h"

#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(dht22);

static void dht22_entry_point(void *, void *, void *);

K_THREAD_DEFINE(sensor_dht22, CONFIG_DHT22_STACK_SIZE,
                dht22_entry_point, NULL, NULL, NULL,
                CONFIG_DHT22_THREAD_PRIORITY, 0, 0);

#define REGISTER_PUBLISHABLE_SENSOR_VALUE(name, max_count)               \
    static dht22_value_cb name##_callbacks[max_count];                   \
    static int num_##name##_callbacks_registered = 0;                    \
    K_MUTEX_DEFINE(dht22_##name##_callback_mutex);                       \
                                                                         \
    void dht22_register_##name##_handler(dht22_value_cb cb)              \
    {                                                                    \
        bool success = true;                                             \
                                                                         \
        k_mutex_lock(&dht22_##name##_callback_mutex, K_FOREVER);         \
                                                                         \
        if (num_##name##_callbacks_registered < max_count)               \
        {                                                                \
            name##_callbacks[num_##name##_callbacks_registered] = cb;    \
            num_##name##_callbacks_registered++;                         \
        }                                                                \
        else                                                             \
        {                                                                \
            success = false;                                             \
        }                                                                \
                                                                         \
        k_mutex_unlock(&dht22_##name##_callback_mutex);                  \
                                                                         \
        if (!success)                                                    \
        {                                                                \
            LOG_ERR("Unable to register " STRINGIFY(name) " callback!"); \
        }                                                                \
    }                                                                    \
                                                                         \
    static void publish_##name##_value(struct sensor_value value)        \
    {                                                                    \
        k_mutex_lock(&dht22_##name##_callback_mutex, K_FOREVER);         \
        for (int i = 0; i < num_##name##_callbacks_registered; i++)      \
        {                                                                \
            name##_callbacks[i](value);                                  \
        }                                                                \
        k_mutex_unlock(&dht22_##name##_callback_mutex);                  \
    }

REGISTER_PUBLISHABLE_SENSOR_VALUE(temperature, CONFIG_DHT22_CALLBACK_MAX_COUNT_TEMPERATURE);
REGISTER_PUBLISHABLE_SENSOR_VALUE(humidity, CONFIG_DHT22_CALLBACK_MAX_COUNT_HUMIDITY);

static void dht22_entry_point(void *u1, void *u2, void *u3)
{
    // Initialize DHT22 Temp+Humidity sensor
    const char *const label = DT_LABEL(DT_INST(0, aosong_dht));
    const struct device *dht22 = device_get_binding(label);
    if (!dht22)
    {
        LOG_ERR("Failed to find sensor %s!", label);
        return;
    }

    struct sensor_value temperature;
    struct sensor_value humidity;

    while (1)
    {
        k_sleep(K_MSEC(CONFIG_DHT22_SAMPLING_RATE_MS));

        int success;

        success = sensor_sample_fetch(dht22);
        if (success != 0)
        {
            LOG_WRN("Sensor fetch failed: %d", success);
            publish_temperature_value(DHT22_ERROR_VALUE);
            publish_humidity_value(DHT22_ERROR_VALUE);
            continue;
        }

        success = sensor_channel_get(dht22, SENSOR_CHAN_AMBIENT_TEMP,
                                     &temperature);
        if (success != 0)
        {
            LOG_WRN("get failed: %d", success);
            publish_temperature_value(DHT22_ERROR_VALUE);
        }
        else
        {
            publish_temperature_value(temperature);
        }

        success = sensor_channel_get(dht22, SENSOR_CHAN_HUMIDITY,
                                     &humidity);
        if (success != 0)
        {
            LOG_WRN("get failed: %d", success);
            publish_humidity_value(DHT22_ERROR_VALUE);
        }
        else
        {
            publish_humidity_value(humidity);
        }
    }
}
