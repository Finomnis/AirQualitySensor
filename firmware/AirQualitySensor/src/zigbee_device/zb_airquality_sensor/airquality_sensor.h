#pragma once

#include "co2_cluster.h"

#include <zboss_api.h>

/** @brief Declare attribute list for Basic cluster (AirQuality Sensor version)
 *  @param attr_list [IN] - attribute list name.
 *  @param zcl_version [IN] - pointer to variable storing ZCL version  attribute value.
 *  @param power_source [IN] - pointer to variable storing power source attribute value.
 *  @param manufacturer_name [IN] - pointer to the variable storing manufacturer name.
 *  @param model_id [IN] - pointer to the variable storing model identifier.
 *  @param product_url [IN] - pointer to the variable storing product url.
*/
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_AIRQUALITY_SENSOR(                           \
    attr_list,                                                                        \
    zcl_version,                                                                      \
    power_source,                                                                     \
    manufacturer_name,                                                                \
    model_id)                                                                         \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))             \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))           \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, (manufacturer_name)) \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, (model_id))           \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @brief Declare cluster list for Airquality Sensor device.
    @param cluster_list_name - cluster list variable name
    @param basic_attr_list - attribute list for Basic cluster
    @param identify_attr_list - attribute list for Identify cluster
    @param temperature_sensor_attr_list - attribute list for Temperature Measurement cluster
    @param humidity_sensor_attr_list - attribute list for Humidity Measurement cluster
    @param co2_sensor_attr_list - attribute list for CO2 Measurement cluster
*/
#define ZB_HA_DECLARE_AIRQUALITY_SENSOR_CLUSTER_LIST(                           \
    cluster_list_name,                                                          \
    basic_attr_list,                                                            \
    identify_attr_list,                                                         \
    temperature_sensor_attr_list,                                               \
    humidity_sensor_attr_list,                                                  \
    co2_sensor_attr_list)                                                       \
    zb_zcl_cluster_desc_t cluster_list_name[] =                                 \
        {                                                                       \
            ZB_ZCL_CLUSTER_DESC(                                                \
                ZB_ZCL_CLUSTER_ID_BASIC,                                        \
                ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),              \
                (basic_attr_list),                                              \
                ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
                ZB_ZCL_MANUF_CODE_INVALID),                                     \
            ZB_ZCL_CLUSTER_DESC(                                                \
                ZB_ZCL_CLUSTER_ID_IDENTIFY,                                     \
                ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),           \
                (identify_attr_list),                                           \
                ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
                ZB_ZCL_MANUF_CODE_INVALID),                                     \
            ZB_ZCL_CLUSTER_DESC(                                                \
                ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,                             \
                ZB_ZCL_ARRAY_SIZE(temperature_sensor_attr_list, zb_zcl_attr_t), \
                (temperature_sensor_attr_list),                                 \
                ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
                ZB_ZCL_MANUF_CODE_INVALID),                                     \
            ZB_ZCL_CLUSTER_DESC(                                                \
                ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,                     \
                ZB_ZCL_ARRAY_SIZE(humidity_sensor_attr_list, zb_zcl_attr_t),    \
                (humidity_sensor_attr_list),                                    \
                ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
                ZB_ZCL_MANUF_CODE_INVALID),                                     \
            ZB_ZCL_CLUSTER_DESC(                                                \
                ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,                              \
                ZB_ZCL_ARRAY_SIZE(co2_sensor_attr_list, zb_zcl_attr_t),         \
                (co2_sensor_attr_list),                                         \
                ZB_ZCL_CLUSTER_SERVER_ROLE,                                     \
                ZB_ZCL_MANUF_CODE_INVALID),                                     \
            ZB_ZCL_CLUSTER_DESC(                                                \
                ZB_ZCL_CLUSTER_ID_IDENTIFY,                                     \
                0,                                                              \
                NULL,                                                           \
                ZB_ZCL_CLUSTER_CLIENT_ROLE,                                     \
                ZB_ZCL_MANUF_CODE_INVALID)}

/** @brief Declare simple descriptor for airquality sensor device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param in_clust_num - number of supported input clusters
    @param out_clust_num - number of supported output clusters
*/
#define ZB_ZCL_DECLARE_AIRQUALITY_SENSOR_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
    ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                   \
    ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)                                    \
    simple_desc_##ep_name =                                                                \
        {                                                                                  \
            ep_id,                                                                         \
            ZB_AF_HA_PROFILE_ID,                                                           \
            ZB_HA_CUSTOM_ATTR_DEVICE_ID,                                                   \
            ZB_HA_DEVICE_VER_CUSTOM_ATTR,                                                  \
            0,                                                                             \
            in_clust_num,                                                                  \
            out_clust_num,                                                                 \
            {                                                                              \
                ZB_ZCL_CLUSTER_ID_BASIC,                                                   \
                ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                \
                ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,                                        \
                ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,                                \
                ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,                                         \
                ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                \
            }}

#define ZB_HA_AIRQUALITY_SENSOR_IN_CLUSTER_NUM 5
#define ZB_HA_AIRQUALITY_SENSOR_OUT_CLUSTER_NUM 1
#define ZB_HA_AIRQUALITY_SENSOR_REPORT_ATTR_COUNT 3

/** @brief Declare endpoint for Airquality Sensor device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_AIRQUALITY_SENSOR_EP(ep_name, ep_id, cluster_list)                \
    ZB_ZCL_DECLARE_AIRQUALITY_SENSOR_DESC(ep_name,                                      \
                                          ep_id,                                        \
                                          ZB_HA_AIRQUALITY_SENSOR_IN_CLUSTER_NUM,       \
                                          ZB_HA_AIRQUALITY_SENSOR_OUT_CLUSTER_NUM);     \
    ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info##device_ctx_name,                 \
                                       ZB_HA_AIRQUALITY_SENSOR_REPORT_ATTR_COUNT);      \
    ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id,                                         \
                                ZB_AF_HA_PROFILE_ID,                                    \
                                0,                                                      \
                                NULL,                                                   \
                                ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), \
                                cluster_list,                                           \
                                (zb_af_simple_desc_1_1_t *)&simple_desc_##ep_name,      \
                                ZB_HA_AIRQUALITY_SENSOR_REPORT_ATTR_COUNT, reporting_info##device_ctx_name, 0, NULL)

/** @brief Declare AirQuality Sensor device context.
    @param device_ctx - device context variable name.
    @param ep_name - endpoint variable name.
*/
#define ZB_HA_DECLARE_AIRQUALITY_SENSOR_CTX(device_ctx, ep_name) \
    ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

#define AIRQUALITY_SENSOR_ENDPOINT 1
