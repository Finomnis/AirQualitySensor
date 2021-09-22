#include "zigbee_device.h"

#include "zb_airquality_sensor/device.h"

#include <zboss_api.h>
#include <zboss_api_addons.h>
#include <zb_mem_config_med.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>
#include <zb_nrf_platform.h>

#include <dk_buttons_and_leds.h>
#include <logging/log.h>

#define ZIGBEE_NETWORK_STATE_LED DK_LED2

#define ZIGBEE_DEVICE_STACK_SIZE 2048
#define ZIGBEE_DEVICE_THREAD_PRIORITY 3

LOG_MODULE_REGISTER(zigbee_device);

static void zigbee_device_main_loop(void *, void *, void *);

K_THREAD_DEFINE(zigbee_device, ZIGBEE_DEVICE_STACK_SIZE,
                zigbee_device_main_loop, NULL, NULL, NULL,
                ZIGBEE_DEVICE_THREAD_PRIORITY, 0, -1);

// Signal handler
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

void airquality_sensor_device_interface_cb(zb_uint8_t param)
{
    zb_zcl_device_callback_param_t *device_cb_param =
        ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);

    device_cb_param->status = RET_OK;
    switch (device_cb_param->device_cb_id)
    {
    case ZB_ZCL_SET_ATTR_VALUE_CB_ID:
        /*if (device_cb_param->cb_param.set_attr_value_param.cluster_id == ZB_ZCL_CLUSTER_ID_ON_OFF &&
            device_cb_param->cb_param.set_attr_value_param.attr_id == ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID)
        {
            if (device_cb_param->cb_param.set_attr_value_param.values.data8)
            {
#ifdef ZB_USE_BUTTONS
                zb_osif_led_on(0);
#endif
            }
            else
            {
#ifdef ZB_USE_BUTTONS
                zb_osif_led_off(0);
#endif
            }
        }*/
        break;
    default:
        break;
    }
}

static void zigbee_device_main_loop(void *unused1, void *unused2, void *unused3)
{
    /* Configure Zigbee */
    zigbee_erase_persistent_storage(false);
    zb_set_ed_timeout(ED_AGING_TIMEOUT_64MIN);
    zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(3000));

    /* Register Zigbee event callback */
    ZB_ZCL_REGISTER_DEVICE_CB(airquality_sensor_device_interface_cb);

    /* Register Zigbee contexts */
    ZB_AF_REGISTER_DEVICE_CTX(&airquality_sensor_device_ctx);

    /* Initialize zigbee context */
    if (RET_OK != zboss_start())
    {
        LOG_ERR("Error when starting zboss stack!");
        return;
    }

    while (1)
    {
        zboss_main_loop_iteration();

        publish_pending_attributes();
    }
}

void initialize_zigbee_device()
{
    k_thread_start(zigbee_device);
}
