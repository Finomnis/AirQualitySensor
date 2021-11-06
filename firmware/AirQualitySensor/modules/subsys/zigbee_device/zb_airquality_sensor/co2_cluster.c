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
    return RET_OK;
}
