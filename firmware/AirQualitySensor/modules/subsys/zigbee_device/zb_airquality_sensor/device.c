#include "device.h"

#include "airquality_sensor.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(zb_airquality_sensor);

/* Basic cluster attributes data */
zb_uint8_t g_attr_basic_zcl_version = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_power_source = ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE;
zb_char_t g_attr_basic_manufacturer_name[] = "\010Finomnis";
zb_char_t g_attr_basic_model_identifier[] = "\022AirQualitySensor-1";

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_AIRQUALITY_SENSOR(basic_attr_list,
                                                   &g_attr_basic_zcl_version,
                                                   &g_attr_basic_power_source,
                                                   &g_attr_basic_manufacturer_name,
                                                   &g_attr_basic_model_identifier);

/* Identify cluster attributes data */
zb_uint16_t g_attr_identify_identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

/* Temperature Measurement cluster attributes data */
zb_int16_t g_attr_temp_measurement_value = ZB_ZCL_TEMP_MEASUREMENT_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_temp_measurement_min_value = ZB_ZCL_TEMP_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE;
zb_int16_t g_attr_temp_measurement_max_value = ZB_ZCL_TEMP_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE;
zb_uint16_t g_attr_temp_measurement_tolerance = 0;

/* Humidity Measurement cluster attributes data */
zb_uint16_t g_attr_rel_humid_measurement_value = ZB_ZCL_REL_HUMIDITY_MEASUREMENT_VALUE_DEFAULT_VALUE;
zb_uint16_t g_attr_rel_humid_measurement_min_value = ZB_ZCL_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE;
zb_uint16_t g_attr_rel_humid_measurement_max_value = ZB_ZCL_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE;

/* CO2 Measurement cluster attributes data */
float g_attr_co2_measurement_value = ZB_ZCL_CO2_MEASUREMENT_VALUE_DEFAULT_VALUE;

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

ZB_ZCL_DECLARE_CO2_MEASUREMENT_ATTRIB_LIST(co2_sensor_attr_list,
                                           &g_attr_co2_measurement_value);

ZB_HA_DECLARE_AIRQUALITY_SENSOR_CLUSTER_LIST(airquality_sensor_clusters,
                                             basic_attr_list,
                                             identify_attr_list,
                                             temperature_measurement_attr_list,
                                             humidity_sensor_attr_list,
                                             co2_sensor_attr_list);

ZB_HA_DECLARE_AIRQUALITY_SENSOR_EP(airquality_sensor_ep, AIRQUALITY_SENSOR_ENDPOINT, airquality_sensor_clusters);
ZB_HA_DECLARE_AIRQUALITY_SENSOR_CTX(airquality_sensor_device_ctx, airquality_sensor_ep);
