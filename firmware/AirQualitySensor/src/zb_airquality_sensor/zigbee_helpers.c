#include "zigbee_helpers.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(zigbee_helpers);

zb_zcl_status_t publish_zigbee_attribute(zb_uint8_t ep, zb_uint16_t cluster_id,
                                         zb_uint8_t cluster_role, zb_uint16_t attr_id,
                                         void *value)
{
    zb_bool_t reported = zcl_is_attr_reported(ep,
                                              cluster_id,
                                              cluster_role,
                                              attr_id);

    if (!reported)
    {
        zb_ret_t result = zb_zcl_start_attr_reporting(ep,
                                                      cluster_id,
                                                      cluster_role,
                                                      attr_id);

        if (result != RET_OK)
        {
            LOG_INF("Unable to start reporting! Error: %d", result);
        }
    }

    return zb_zcl_set_attr_val(ep,
                               cluster_id,
                               cluster_role,
                               attr_id,
                               (zb_uint8_t *)value,
                               false);
}
