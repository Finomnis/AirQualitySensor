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
/* Basic cluster attributes data */
zb_uint8_t g_attr_basic_zcl_version = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_application_version = ZB_ZCL_BASIC_APPLICATION_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_stack_version = ZB_ZCL_BASIC_STACK_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_hw_version = ZB_ZCL_BASIC_HW_VERSION_DEFAULT_VALUE;
zb_char_t g_attr_basic_manufacturer_name[] = "\016Titan Products";
zb_char_t g_attr_basic_model_identifier[] = "\014TPZRCO2HT-Z3";
zb_char_t g_attr_basic_date_code[] = ZB_ZCL_BASIC_DATE_CODE_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_power_source = ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE;
zb_char_t g_attr_basic_location_description[] = ZB_ZCL_BASIC_LOCATION_DESCRIPTION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_physical_environment = ZB_ZCL_BASIC_PHYSICAL_ENVIRONMENT_DEFAULT_VALUE;
zb_char_t g_attr_sw_build_id[] = ZB_ZCL_BASIC_SW_BUILD_ID_DEFAULT_VALUE;

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(basic_attr_list, &g_attr_basic_zcl_version, &g_attr_basic_application_version, &g_attr_basic_stack_version, &g_attr_basic_hw_version, &g_attr_basic_manufacturer_name, &g_attr_basic_model_identifier, &g_attr_basic_date_code, &g_attr_basic_power_source, &g_attr_basic_location_description, &g_attr_basic_physical_environment, &g_attr_sw_build_id);

/*ZB_ZCL_START_DECLARE_ATTRIB_LIST(basic_attr_list)
ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID(&g_attr_basic_zcl_version),
    ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID(&g_attr_basic_power_source),
    ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID(&g_attr_basic_model_identifier),
    ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID(&g_attr_basic_manufacturer_name),
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST;
*/
/* Identify cluster attributes data */
zb_uint16_t g_attr_identify_identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;
/* Temperature Measurement cluster attributes data */
zb_int16_t g_attr_temp_measurement_value = ZB_ZCL_TEMP_MEASUREMENT_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_temp_measurement_min_value = ZB_ZCL_TEMP_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_temp_measurement_max_value = ZB_ZCL_TEMP_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE;
zb_uint16_t g_attr_temp_measurement_tolerance = 0;

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &g_attr_identify_identify_time);
ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST(temp_measurement_attr_list,
                                            &g_attr_temp_measurement_value,
                                            &g_attr_temp_measurement_min_value,
                                            &g_attr_temp_measurement_max_value,
                                            &g_attr_temp_measurement_tolerance);
/********************* Declare device **************************/
ZB_HA_DECLARE_TEMPERATURE_SENSOR_CLUSTER_LIST(temperature_sensor_clusters, basic_attr_list, identify_attr_list, temp_measurement_attr_list);
ZB_HA_DECLARE_TEMPERATURE_SENSOR_EP(temperature_sensor_ep, 2, temperature_sensor_clusters);
ZB_HA_DECLARE_TEMPERATURE_SENSOR_CTX(device_ctx, temperature_sensor_ep);

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

zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param)
{
    zb_zcl_parsed_hdr_t cmd_info;
    zb_uint8_t lqi = ZB_MAC_LQI_UNDEFINED;
    zb_int8_t rssi = ZB_MAC_RSSI_UNDEFINED;
    TRACE_MSG(TRACE_APP1, "> zcl_specific_cluster_cmd_handler", (FMT__0));
    ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
    zb_uint16_t g_dst_addr = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr;
    ZB_ZCL_DEBUG_DUMP_HEADER(&cmd_info);
    TRACE_MSG(TRACE_APP3, "payload size: %i", (FMT__D, zb_buf_len(param)));
    zb_zdo_get_diag_data(g_dst_addr, &lqi, &rssi);
    TRACE_MSG(TRACE_APP3, "lqi %hd rssi %d", (FMT__H_H, lqi, rssi));
    if (cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI)
    {
        TRACE_MSG(
            TRACE_ERROR,
            "Unsupported \"from server\" command direction",
            (FMT__0));
    }
    TRACE_MSG(TRACE_APP1, "< zcl_specific_cluster_cmd_handler", (FMT__0));
    return ZB_FALSE;
}

void test_device_interface_cb(zb_uint8_t param)
{
    zb_zcl_device_callback_param_t *device_cb_param =
        ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    TRACE_MSG(TRACE_APP1, "> test_device_interface_cb param %hd id %hd", (FMT__H_H, param, device_cb_param->device_cb_id));
    device_cb_param->status = RET_OK;
    switch (device_cb_param->device_cb_id)
    {
    case ZB_ZCL_SET_ATTR_VALUE_CB_ID:
        if (device_cb_param->cb_param.set_attr_value_param.cluster_id == ZB_ZCL_CLUSTER_ID_ON_OFF &&
            device_cb_param->cb_param.set_attr_value_param.attr_id == ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID)
        {
            if (device_cb_param->cb_param.set_attr_value_param.values.data8)
            {
                TRACE_MSG(TRACE_APP1, "set ON", (FMT__0));
#ifdef ZB_USE_BUTTONS
                zb_osif_led_on(0);
#endif
            }
            else
            {
                TRACE_MSG(TRACE_APP1, "set OFF", (FMT__0));
#ifdef ZB_USE_BUTTONS
                zb_osif_led_off(0);
#endif
            }
        }
        break;
    default:
        device_cb_param->status = RET_OK;
        break;
    }
    TRACE_MSG(TRACE_APP1, "< test_device_interface_cb %hd", (FMT__H, device_cb_param->status));
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
    ZB_AF_REGISTER_DEVICE_CTX(&device_ctx);
    //ZB_AF_SET_ENDPOINT_HANDLER(ZB_OUTPUT_ENDPOINT, zcl_specific_cluster_cmd_handler);
    ZB_ZCL_REGISTER_DEVICE_CB(test_device_interface_cb);

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
