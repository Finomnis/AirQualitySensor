#include "device.h"

#include "airquality_sensor.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(zb_airquality_sensor);

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

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(basic_attr_list,
                                     &g_attr_basic_zcl_version,
                                     &g_attr_basic_application_version,
                                     &g_attr_basic_stack_version,
                                     &g_attr_basic_hw_version,
                                     &g_attr_basic_manufacturer_name,
                                     &g_attr_basic_model_identifier,
                                     &g_attr_basic_date_code,
                                     &g_attr_basic_power_source,
                                     &g_attr_basic_location_description,
                                     &g_attr_basic_physical_environment,
                                     &g_attr_sw_build_id);

/* Identify cluster attributes data */
zb_uint16_t g_attr_identify_identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

/* Temperature Measurement cluster attributes data */
zb_int16_t g_attr_temp_measurement_value = ZB_ZCL_TEMP_MEASUREMENT_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_temp_measurement_min_value = ZB_ZCL_TEMP_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_temp_measurement_max_value = ZB_ZCL_TEMP_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE;
zb_uint16_t g_attr_temp_measurement_tolerance = 0;

/* Temperature Measurement cluster attributes data */
zb_int16_t g_attr_rel_humid_measurement_value = ZB_ZCL_REL_HUMIDITY_MEASUREMENT_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_rel_humid_measurement_min_value = ZB_ZCL_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_rel_humid_measurement_max_value = ZB_ZCL_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE;

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &g_attr_identify_identify_time);

ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST(temperature_measurement_attr_list,
                                            &g_attr_temp_measurement_value,
                                            &g_attr_temp_measurement_min_value,
                                            &g_attr_temp_measurement_max_value,
                                            &g_attr_temp_measurement_tolerance);

ZB_ZCL_DECLARE_REL_HUMIDITY_MEASUREMENT_ATTRIB_LIST(humidity_sensor_attr_list,
                                                    &g_attr_rel_humid_measurement_value,
                                                    &g_attr_rel_humid_measurement_min_value,
                                                    &g_attr_rel_humid_measurement_max_value);

ZB_HA_DECLARE_AIRQUALITY_SENSOR_CLUSTER_LIST(airquality_sensor_clusters,
                                             basic_attr_list,
                                             identify_attr_list,
                                             temperature_measurement_attr_list,
                                             humidity_sensor_attr_list);

#define AIRQUALITY_SENSOR_ENDPOINT 1
ZB_HA_DECLARE_AIRQUALITY_SENSOR_EP(airquality_sensor_ep, AIRQUALITY_SENSOR_ENDPOINT, airquality_sensor_clusters);
ZB_HA_DECLARE_AIRQUALITY_SENSOR_CTX(device_ctx, airquality_sensor_ep);

void update_zb_airquality_sensor()
{
    LOG_INF("Value: %d", g_attr_temp_measurement_value);
    zb_int16_t new_value = 12345;
    zb_zcl_status_t result = zb_zcl_set_attr_val(
        AIRQUALITY_SENSOR_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
        (zb_uint8_t *)&new_value,
        false);
    LOG_INF("Result: %d", result);

    zb_bool_t reported = zcl_is_attr_reported(
        AIRQUALITY_SENSOR_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID);
    LOG_INF("Reported: %d", reported);
}
