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

#include "zigbee_device.h"
#include "dht22.h"

LOG_MODULE_REGISTER(main);

/********************* Declare device **************************/

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

    // Start DHT22 sensor sampling
    start_dht22_sampling();

    zb_int16_t temp = 12345;
    zb_int16_t humid = 123;
    float co2 = 0.00001;
    while (1)
    {
        LOG_INF("Updating sensor values ...");
        publish_temperature(temp++);
        publish_humidity(humid++);
        co2 += 0.00001;
        publish_co2(co2);
        dk_set_led_on(DK_LED1);
        k_sleep(K_MSEC(3000));
        LOG_INF("Ping.");
        dk_set_led_off(DK_LED1);
        k_sleep(K_MSEC(3000));
        LOG_INF("Pong.");
    }
}
