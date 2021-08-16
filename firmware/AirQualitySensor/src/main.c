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

LOG_MODULE_REGISTER(main);

void main(void)
{
    LOG_INF("Example!");

    int err = dk_leds_init();
    if (err)
    {
        LOG_ERR("Cannot init LEDs (err: %d)", err);
    }

    while (1)
    {
        dk_set_leds(DK_ALL_LEDS_MSK);
        k_sleep(K_MSEC(1000));
        LOG_INF("Ping.");
        dk_set_leds(DK_NO_LEDS_MSK);
        k_sleep(K_MSEC(1000));
        LOG_INF("Pong.");
    }
}
