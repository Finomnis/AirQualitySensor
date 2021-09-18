#include "dht22.h"

#include <zephyr.h>
#include <logging/log.h>
#include <drivers/sensor.h>

LOG_MODULE_REGISTER(dht22);

static void dht22_entry_point(void *, void *, void *);

K_THREAD_DEFINE(sensor_dht22, CONFIG_DHT22_STACK_SIZE,
                dht22_entry_point, NULL, NULL, NULL,
                CONFIG_DHT22_THREAD_PRIORITY, 0, 0);

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

        int rc = sensor_sample_fetch(dht22);

        if (rc != 0)
        {
            LOG_WRN("Sensor fetch failed: %d", rc);
            continue;
        }

        rc = sensor_channel_get(dht22, SENSOR_CHAN_AMBIENT_TEMP,
                                &temperature);
        if (rc == 0)
        {
            rc = sensor_channel_get(dht22, SENSOR_CHAN_HUMIDITY,
                                    &humidity);
        }
        if (rc != 0)
        {
            LOG_WRN("get failed: %d", rc);
            continue;
        }

        LOG_INF("%d.%d Celsius ; %d %d %% Humidity",
                temperature.val1, temperature.val2,
                humidity.val1, humidity.val2);
    }
}
