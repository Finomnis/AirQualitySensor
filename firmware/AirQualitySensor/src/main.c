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

#if CONFIG_SUBSYS_SENSEAIR_S8
#include "senseair_s8.h"
#endif

#if CONFIG_SUBSYS_CO2_LEDS
#include "co2_leds.h"
#endif

LOG_MODULE_REGISTER(main);

#if CONFIG_SUBSYS_DHT22
static void handle_temperature_value(struct sensor_value value)
{
    LOG_INF("Temperature: %d.%06d Celsius", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("Temperature failed.");
    }
}
static void handle_humidity_value(struct sensor_value value)
{
    LOG_INF("Humidity: %d.%06d %%", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("Humidity failed.");
    }
}
#endif

#if CONFIG_SUBSYS_SENSEAIR_S8
static void handle_co2_value(struct sensor_value value)
{
    LOG_INF("CO2: %d.%06d", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("CO2 failed.");
    }
}
#endif

void main(void)
{
    LOG_INF("Finomnis's Air Quality Sensor");

    int err = dk_leds_init();
    if (err)
    {
        LOG_ERR("Cannot init LEDs (err: %d)", err);
    }

#if CONFIG_SUBSYS_ZIGBEE_DEVICE
    // Start zigbee device
    start_zigbee_device();
#endif

#if CONFIG_SUBSYS_DHT22
    // Register sensor value handlers
    dht22_register_temperature_handler(handle_temperature_value);
    dht22_register_humidity_handler(handle_humidity_value);
#if CONFIG_SUBSYS_ZIGBEE_DEVICE
    // Forward dht22 measurements to zigbee
    dht22_register_temperature_handler(publish_temperature);
    dht22_register_humidity_handler(publish_humidity);
#endif
#endif

#if CONFIG_SUBSYS_SENSEAIR_S8
    // Register sensor value handlers
    senseair_s8_register_co2_handler(handle_co2_value);
#if CONFIG_SUBSYS_ZIGBEE_DEVICE
    // Forward measurements to zigbee
    senseair_s8_register_co2_handler(publish_co2);
#endif
#if CONFIG_SUBSYS_CO2_LEDS
    // Forward co2 measurements to CO2 status LEDs
    senseair_s8_register_co2_handler(co2_leds_set_co2_level);
#endif
#endif

    while (1)
    {
        dk_set_led_off(DK_LED1);
        k_sleep(K_MSEC(1000));
        dk_set_led_on(DK_LED1);
        k_sleep(K_FOREVER);
    }
}
