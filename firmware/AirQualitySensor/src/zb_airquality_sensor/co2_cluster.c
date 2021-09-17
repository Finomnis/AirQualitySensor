#include "co2_cluster.h"

zb_ret_t check_value_co2_measurement_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_co2_measurement_init_server()
{
    zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,
                                ZB_ZCL_CLUSTER_SERVER_ROLE,
                                check_value_co2_measurement_server,
                                (zb_zcl_cluster_write_attr_hook_t)NULL,
                                (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_co2_measurement_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
    zb_ret_t ret = RET_OK;

    switch (attr_id)
    {
    case ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID:
        if (ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_UNKNOWN == ZB_ZCL_ATTR_GET16(value))
        {
            ret = RET_OK;
        }
        else
        {
            zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(
                endpoint,
                ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,
                ZB_ZCL_CLUSTER_SERVER_ROLE,
                ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_ID);

            ZB_ASSERT(attr_desc);

            ret = (ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) ==
                       ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_UNDEFINED ||
                   ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) <= ZB_ZCL_ATTR_GET16(value))
                      ? RET_OK
                      : RET_ERROR;

            if (ret == RET_OK)
            {
                attr_desc = zb_zcl_get_attr_desc_a(
                    endpoint,
                    ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,
                    ZB_ZCL_CLUSTER_SERVER_ROLE,
                    ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_ID);

                ZB_ASSERT(attr_desc);

                ret = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) == ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_UNDEFINED ||
                              ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc)
                          ? RET_OK
                          : RET_ERROR;
            }
        }
        break;

    case ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_ID:
        ret = (
#if ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_MIN_VALUE != 0
                  ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_MIN_VALUE <= ZB_ZCL_ATTR_GET16(value) &&
#endif
                  (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_MAX_VALUE))
                  ? RET_OK
                  : RET_ERROR;
        break;

    case ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_ID:
        ret = ((ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_MIN_VALUE <= ZB_ZCL_ATTR_GET16(value)) &&
               (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_MAX_VALUE))
                  ? RET_OK
                  : RET_ERROR;
        break;

    default:
        break;
    }

    return ret;
}
