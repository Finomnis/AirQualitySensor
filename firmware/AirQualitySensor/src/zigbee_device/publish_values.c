#include "publish_values.h"

#include "zb_airquality_sensor/airquality_sensor.h"

#include <logging/log.h>
#include <zboss_api.h>

LOG_MODULE_DECLARE(zigbee_device);

typedef union zigbee_attribute_value_t
{
    float val_co2;
    zb_int32_t val_temperature;
    zb_uint32_t val_humidity;
} zigbee_attribute_value;

/**
 * @brief Sets and publishes a zigbee attribute value
 *
 * @param ep - endpoint number
 * @param cluster_id - cluster ID (see @ref zb_zcl_cluster_id_t)
 * @param cluster_role - role (see @ref zcl_cluster_role)
 * @param attr_id - ID of the attribute being set
 * @param value - pointer to the value
 *
 * @note given endpoint with given cluster ID should exist
 */
void publish_zigbee_attribute(zb_uint8_t ep, zb_uint16_t cluster_id,
                              zb_uint8_t cluster_role, zb_uint16_t attr_id,
                              zigbee_attribute_value value)
{
    LOG_INF("Publishing %d!", cluster_id);
}

void zb_airquality_sensor_publish_temperature(zb_int16_t value)
{
    zigbee_attribute_value attribute_value = {.val_temperature = value};
    publish_zigbee_attribute(AIRQUALITY_SENSOR_ENDPOINT,
                             ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
                             attribute_value);
}

void zb_airquality_sensor_publish_humidity(zb_uint16_t value)
{
    zigbee_attribute_value attribute_value = {.val_humidity = value};
    publish_zigbee_attribute(AIRQUALITY_SENSOR_ENDPOINT,
                             ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID,
                             attribute_value);
}

void zb_airquality_sensor_publish_co2(float value)
{
    zigbee_attribute_value attribute_value = {.val_co2 = value};
    publish_zigbee_attribute(AIRQUALITY_SENSOR_ENDPOINT,
                             ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID,
                             attribute_value);
}

void publish_temperature(struct sensor_value value)
{
    if (value.val2 < 0)
    {
        zb_airquality_sensor_publish_temperature(ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_UNKNOWN);
        return;
    }

    int64_t converted_value = ((int64_t)value.val1) * 100 + value.val2 / 10000;
    if (converted_value > ZB_ZCL_ATTR_TEMP_MEASUREMENT_MAX_VALUE_MAX_VALUE ||
        converted_value < ZB_ZCL_ATTR_TEMP_MEASUREMENT_MIN_VALUE_MIN_VALUE)
    {
        zb_airquality_sensor_publish_temperature(ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_UNKNOWN);
        return;
    }

    zb_airquality_sensor_publish_temperature((zb_int16_t)converted_value);
}

void publish_humidity(struct sensor_value value)
{
    if (value.val2 < 0)
    {
        zb_airquality_sensor_publish_humidity(ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_UNKNOWN);
        return;
    }

    int64_t converted_value = ((int64_t)value.val1) * 100 + value.val2 / 10000;
    if (converted_value > ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_MAX_VALUE ||
        converted_value < ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_MIN_VALUE)
    {
        zb_airquality_sensor_publish_humidity(ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_UNKNOWN);
        return;
    }

    zb_airquality_sensor_publish_humidity((zb_uint16_t)converted_value);
}
