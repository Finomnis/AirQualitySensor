#include "include/publish_values.h"

#include "zb_airquality_sensor/airquality_sensor.h"

#include <logging/log.h>
#include <zboss_api.h>

LOG_MODULE_DECLARE(zigbee_device);

typedef struct zigbee_attribute_update_t
{
    zb_uint8_t ep;
    zb_uint16_t cluster_id;
    zb_uint8_t cluster_role;
    zb_uint16_t attr_id;
    union
    {
        float co2;
        zb_int32_t temperature;
        zb_uint32_t humidity;
    } value;
} zigbee_attribute_update;

K_MSGQ_DEFINE(aql_attribute_report_queue, sizeof(zigbee_attribute_update), 10, 4);

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
static void publish_zigbee_attribute(zigbee_attribute_update data)
{
    if (0 != k_msgq_put(&aql_attribute_report_queue, &data, K_NO_WAIT))
    {
        k_msgq_purge(&aql_attribute_report_queue);
    }
}

void publish_pending_attributes()
{
    zigbee_attribute_update data;

    while (1)
    {
        /* get a data item */
        if (0 != k_msgq_get(&aql_attribute_report_queue, &data, K_NO_WAIT))
            return;

        /* process data item */
        zb_zcl_status_t result = zb_zcl_set_attr_val(data.ep,
                                                     data.cluster_id,
                                                     data.cluster_role,
                                                     data.attr_id,
                                                     (zb_uint8_t *)&data.value,
                                                     false);
        if (ZB_ZCL_STATUS_SUCCESS != result)
        {
            LOG_WRN("Unable to publish attribute %d! Error: %d", data.cluster_id, result);
        }
    }
}

static void zb_airquality_sensor_publish_temperature(zb_int16_t value)
{
    zigbee_attribute_update data = {
        .ep = AIRQUALITY_SENSOR_ENDPOINT,
        .cluster_id = ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        .cluster_role = ZB_ZCL_CLUSTER_SERVER_ROLE,
        .attr_id = ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
        .value.temperature = value};

    publish_zigbee_attribute(data);
}

static void zb_airquality_sensor_publish_humidity(zb_uint16_t value)
{
    zigbee_attribute_update data = {
        .ep = AIRQUALITY_SENSOR_ENDPOINT,
        .cluster_id = ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
        .cluster_role = ZB_ZCL_CLUSTER_SERVER_ROLE,
        .attr_id = ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID,
        .value.humidity = value};

    publish_zigbee_attribute(data);
}

static void zb_airquality_sensor_publish_co2(float value)
{
    zigbee_attribute_update data = {
        .ep = AIRQUALITY_SENSOR_ENDPOINT,
        .cluster_id = ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT,
        .cluster_role = ZB_ZCL_CLUSTER_SERVER_ROLE,
        .attr_id = ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID,
        .value.co2 = value};

    publish_zigbee_attribute(data);
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

void publish_co2(struct sensor_value value, bool error)
{
    if (error)
    {
        zb_airquality_sensor_publish_co2(ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_UNKNOWN);
        return;
    }

    zb_airquality_sensor_publish_co2(sensor_value_to_double(&value));
}
