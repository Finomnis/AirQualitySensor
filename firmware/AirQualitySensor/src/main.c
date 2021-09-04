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
#include <zboss_api_addons.h>
#include <zb_mem_config_med.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>
#include <zb_nrf_platform.h>

LOG_MODULE_REGISTER(main);

#define ZIGBEE_NETWORK_STATE_LED DK_LED2

static zb_uint8_t attr_zcl_version = ZB_ZCL_VERSION;
static zb_uint8_t attr_power_source = ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE;
static zb_uint16_t attr_identify_time;

/* Declare attribute list for Basic cluster. */
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(basic_attr_list, &attr_zcl_version,
                                 &attr_power_source);

/* Declare attribute list for Identify cluster. */
ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &attr_identify_time);

/* Declare cluster list for Dimmer Switch device (Identify, Basic, Scenes,
 * Groups, On Off, Level Control).
 * Only clusters Identify and Basic have attributes.
 */
ZB_HA_DECLARE_DIMMER_SWITCH_CLUSTER_LIST(dimmer_switch_clusters,
                                         basic_attr_list,
                                         identify_attr_list);

/* Declare endpoint for Dimmer Switch device. */
#define LIGHT_SWITCH_ENDPOINT 1
ZB_HA_DECLARE_DIMMER_SWITCH_EP(dimmer_switch_ep,
                               LIGHT_SWITCH_ENDPOINT,
                               dimmer_switch_clusters);

ZB_HA_DECLARE_DIMMER_SWITCH_CTX(dimmer_switch_ctx, dimmer_switch_ep);

void zboss_signal_handler(zb_bufid_t bufid)
{
    zb_zdo_app_signal_hdr_t *sig_hndler = NULL;
    zb_zdo_app_signal_type_t sig = zb_get_app_signal(bufid, &sig_hndler);
    zb_ret_t status = ZB_GET_APP_SIGNAL_STATUS(bufid);

    /* Update network status LED. */
    zigbee_led_status_update(bufid, ZIGBEE_NETWORK_STATE_LED);

    switch (sig)
    {
    case ZB_BDB_SIGNAL_DEVICE_REBOOT:
    /* fall-through */
    case ZB_BDB_SIGNAL_STEERING:
        /* Call default signal handler. */
        ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));
        if (status == RET_OK)
        {
            // /* Check the light device address. */
            // if (bulb_ctx.short_addr == 0xFFFF)
            // {
            //     k_timer_start(&bulb_ctx.find_alarm,
            //                   MATCH_DESC_REQ_START_DELAY,
            //                   MATCH_DESC_REQ_TIMEOUT);
            // }
        }
        break;
    default:
        /* Call default signal handler. */
        ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));
        break;
    }

    if (bufid)
    {
        zb_buf_free(bufid);
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

    /* Configure Zigbee */
    zigbee_erase_persistent_storage(false);
    zb_set_ed_timeout(ED_AGING_TIMEOUT_64MIN);
    zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(3000));

    /* Register Zigbee contexts */
    ZB_AF_REGISTER_DEVICE_CTX(&dimmer_switch_ctx);

    /* Start Zigbee default thread. */
    zigbee_enable();

    while (1)
    {
        dk_set_led_on(DK_LED1);
        k_sleep(K_MSEC(3000));
        LOG_INF("Ping.");
        dk_set_led_off(DK_LED1);
        k_sleep(K_MSEC(3000));
        LOG_INF("Pong.");
    }
}
