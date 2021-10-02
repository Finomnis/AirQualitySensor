/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Simple Zigbee light bulb implementation.
 */

#include <zephyr/types.h>
#include <zephyr.h>
#include <logging/log.h>
#include <dk_buttons_and_leds.h>
#include <drivers/sensor.h>

#if CONFIG_SUBSYS_ZIGBEE_DEVICE
#include "zigbee_device.h"
#endif

#if CONFIG_SUBSYS_DHT22
#include "dht22.h"
#endif

LOG_MODULE_REGISTER(main);

#if CONFIG_SUBSYS_DHT22
static void handle_temperature_value(struct sensor_value value)
{
    LOG_INF("Temperature: %d.%d Celsius", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("Temperature failed.");
    }
}
static void handle_humidity_value(struct sensor_value value)
{
    LOG_INF("Humidity: %d.%d %%", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("Humidity failed.");
    }
}
#endif

void main(void)
{
    LOG_INF("Example!");

    int err = dk_leds_init();
    if (err)
    {
        LOG_ERR("Cannot init LEDs (err: %d)", err);
    }

#if CONFIG_SUBSYS_ZIGBEE_DEVICE
    // Start zigbee device
    start_zigbee_device();

#if CONFIG_SUBSYS_DHT22
    // Forward dht22 measurements to zigbee
    dht22_register_temperature_handler(publish_temperature);
    dht22_register_humidity_handler(publish_humidity);
#endif

#endif

#if CONFIG_SUBSYS_DHT22
    // Register sensor value handlers
    dht22_register_temperature_handler(handle_temperature_value);
    dht22_register_humidity_handler(handle_humidity_value);
#endif

#if CONFIG_SENSAIR_S8
    const char *const label = DT_LABEL(DT_PATH(co2sensor));
    const struct device *co2_sensor = device_get_binding(label);
    if (!co2_sensor)
    {
        LOG_ERR("Failed to find sensor %s!", label);
        return;
    }

    struct sensor_value co2_val;

    while (1)
    {
        k_sleep(K_MSEC(4000));

        int success;

        success = sensor_sample_fetch(co2_sensor);
        if (success != 0)
        {
            LOG_WRN("Sensor fetch failed: %d", success);
            continue;
        }

        success = sensor_channel_get(co2_sensor, SENSOR_CHAN_CO2,
                                     &co2_val);
        if (success != 0)
        {
            LOG_WRN("get failed: %d", success);
        }
        else
        {
            LOG_INF("got CO2: %d.%d", co2_val.val1, co2_val.val2);
        }
    }
#endif

    float co2 = 0.00001f;
    while (1)
    {
        co2 += 0.00001f;
        //zb_airquality_sensor_publish_co2(co2);
        dk_set_led_on(DK_LED1);
        k_sleep(K_MSEC(1000));
        //LOG_INF("Ping.");
        dk_set_led_off(DK_LED1);
        k_sleep(K_MSEC(1000));
        //LOG_INF("Pong.");
    }
}
