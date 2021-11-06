#include "senseair_s8.h"

#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(senseair_s8);

static void senseair_s8_entry_point(void *, void *, void *);

K_THREAD_DEFINE(sensor_senseair_s8, CONFIG_SUBSYS_SENSEAIR_S8_STACK_SIZE,
                senseair_s8_entry_point, NULL, NULL, NULL,
                CONFIG_SUBSYS_SENSEAIR_S8_THREAD_PRIORITY, 0, 0);

#define REGISTER_PUBLISHABLE_SENSOR_VALUE(name, max_count)               \
    static senseair_s8_value_cb name##_callbacks[max_count];             \
    static int num_##name##_callbacks_registered = 0;                    \
    K_MUTEX_DEFINE(senseair_s8_##name##_callback_mutex);                 \
                                                                         \
    void senseair_s8_register_##name##_handler(senseair_s8_value_cb cb)  \
    {                                                                    \
        bool success = true;                                             \
                                                                         \
        k_mutex_lock(&senseair_s8_##name##_callback_mutex, K_FOREVER);   \
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
        k_mutex_unlock(&senseair_s8_##name##_callback_mutex);            \
                                                                         \
        if (!success)                                                    \
        {                                                                \
            LOG_ERR("Unable to register " STRINGIFY(name) " callback!"); \
        }                                                                \
    }                                                                    \
                                                                         \
    static void publish_##name##_value(struct sensor_value value)        \
    {                                                                    \
        k_mutex_lock(&senseair_s8_##name##_callback_mutex, K_FOREVER);   \
        for (int i = 0; i < num_##name##_callbacks_registered; i++)      \
        {                                                                \
            name##_callbacks[i](value, false);                           \
        }                                                                \
        k_mutex_unlock(&senseair_s8_##name##_callback_mutex);            \
    }                                                                    \
                                                                         \
    static void publish_##name##_value_error()                           \
    {                                                                    \
        struct sensor_value error_value = {0, 0};                        \
        k_mutex_lock(&senseair_s8_##name##_callback_mutex, K_FOREVER);   \
        for (int i = 0; i < num_##name##_callbacks_registered; i++)      \
        {                                                                \
            name##_callbacks[i](error_value, true);                      \
        }                                                                \
        k_mutex_unlock(&senseair_s8_##name##_callback_mutex);            \
    }

REGISTER_PUBLISHABLE_SENSOR_VALUE(co2, CONFIG_SUBSYS_SENSEAIR_S8_CALLBACK_MAX_COUNT_CO2);

static void senseair_s8_entry_point(void *u1, void *u2, void *u3)
{
    // Initialize DHT22 Temp+Humidity sensor
    const char *const label = DT_LABEL(DT_PATH(senseair_s8));
    const struct device *senseair_s8 = device_get_binding(label);
    if (!senseair_s8)
    {
        LOG_ERR("Failed to find sensor %s!", label);
        return;
    }

    struct sensor_value co2;

    while (1)
    {
        k_sleep(K_MSEC(CONFIG_SUBSYS_SENSEAIR_S8_SAMPLING_RATE_MS));

        int success;

        success = sensor_sample_fetch(senseair_s8);
        if (success != 0)
        {
            LOG_WRN("Sensor fetch failed: %d", success);
            publish_co2_value_error();
            continue;
        }

        success = sensor_channel_get(senseair_s8, SENSOR_CHAN_CO2,
                                     &co2);
        if (success != 0)
        {
            LOG_WRN("get failed: %d", success);
            publish_co2_value_error();
        }
        else
        {
            publish_co2_value(co2);
        }
    }
}
