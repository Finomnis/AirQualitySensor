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
#include <zboss_api.h>

#include "zigbee_device/zigbee_device.h"
#include "dht22.h"

LOG_MODULE_REGISTER(main);

/********************* Declare device **************************/

void handle_temperature_value(struct sensor_value value)
{
    LOG_INF("Temperature: %d.%d Celsius", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("Temperature failed.");
    }
}
void handle_humidity_value(struct sensor_value value)
{
    LOG_INF("Humidity: %d.%d %%", value.val1, value.val2);
    if (value.val2 < 0)
    {
        LOG_WRN("Humidity failed.");
    }
}

void main(void)
{
    LOG_INF("Example!");

    int err = dk_leds_init();
    if (err)
    {
        LOG_ERR("Cannot init LEDs (err: %d)", err);
    }

    // Start zigbee device
    initialize_zigbee_device();

    // Register sensor value handlers
    dht22_register_temperature_handler(handle_temperature_value);
    dht22_register_humidity_handler(handle_humidity_value);
    //dht22_register_temperature_handler(publish_temperature);
    //dht22_register_humidity_handler(publish_humidity);

    float co2 = 0.00001f;
    while (1)
    {
        co2 += 0.00001f;
        //zb_airquality_sensor_publish_co2(co2);
        dk_set_led_on(DK_LED1);
        k_sleep(K_MSEC(10));
        //LOG_INF("Ping.");
        dk_set_led_off(DK_LED1);
        k_sleep(K_MSEC(10));
        //LOG_INF("Pong.");
    }
}
