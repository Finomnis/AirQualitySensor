#pragma once

#include <zboss_api.h>

#define ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT 0x040DU /*!< CO2 measurement */

/*! @brief CO2 Measurement cluster attribute identifiers
*/
enum zb_zcl_co2_measurement_attr_e
{
    /*! @brief MeasuredValue, ZCL spec 4.4.2.2.1.1 */
    ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID = 0x0000,
    /*! @brief MinMeasuredValue, ZCL spec 4.4.2.2.1.2 */
    ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_ID = 0x0001,
    /*! @brief MaxMeasuredValue, ZCL spec 4.4.2.2.1.3 */
    ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_ID = 0x0002,
#ifndef ZB_DISABLE_CO2_MEASUREMENT_TOLERANCE_ID
    /*! @brief Tolerance, ZCL spec 4.4.2.2.1.4 */
    ZB_ZCL_ATTR_CO2_MEASUREMENT_TOLERANCE_ID = 0x0003,
#else
    ZB_ZCL_ATTR_CO2_MEASUREMENT_TOLERANCE_ID = 0xF003,
#endif
};

/** @brief MeasuredValue attribute unknown value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_UNKNOWN ((zb_uint16_t)0xFFFF)

/** @brief MinMeasuredValue attribute minimum value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_MIN_VALUE 0

/** @brief MinMeasuredValue attribute maximum value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_MAX_VALUE ((zb_uint16_t)0xffef)

/** @brief MinMeasuredValue attribute undefined value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_UNDEFINED ((zb_uint16_t)0xFFFF)

/** @brief MaxMeasuredValue attribute minimum value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_MIN_VALUE ((zb_uint16_t)0x0001)

/** @brief MaxMeasuredValue attribute maximum value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_MAX_VALUE ((zb_uint16_t)0xfff0)

/** @brief MaxMeasuredValue attribute value not defined */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_UNDEFINED ((zb_uint16_t)0xFFFF)

/** @brief Default value for MeasurementValue attribute */
#define ZB_ZCL_CO2_MEASUREMENT_VALUE_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Default value for MeasurementMinValue attribute */
#define ZB_ZCL_CO2_MEASUREMENT_MIN_VALUE_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Default value for MeasurementMaxValue attribute */
#define ZB_ZCL_CO2_MEASUREMENT_MAX_VALUE_DEFAULT_VALUE ((zb_uint16_t)0xFFFF)

/** @brief Declare attribute list for CO2 Measurement cluster - server side
    @param attr_list - attribute list name
    @param value - pointer to variable to store MeasuredValue attribute
    @param min_value - pointer to variable to store MinMeasuredValue attribute
    @param max_value - pointer to variable to store MAxMeasuredValue attribute
*/
#define ZB_ZCL_DECLARE_CO2_MEASUREMENT_ATTRIB_LIST(attr_list,                   \
                                                   value, min_value, max_value) \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                 \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID, (value))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_ID, (min_value)) \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_ID, (max_value)) \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID(data_ptr) \
    {                                                                         \
        ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID,                                 \
            ZB_ZCL_ATTR_TYPE_U16,                                             \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,      \
            (void *)data_ptr                                                  \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_ID(data_ptr) \
    {                                                                             \
        ZB_ZCL_ATTR_CO2_MEASUREMENT_MIN_VALUE_ID,                                 \
            ZB_ZCL_ATTR_TYPE_U16,                                                 \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                         \
            (void *)data_ptr                                                      \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_ID(data_ptr) \
    {                                                                             \
        ZB_ZCL_ATTR_CO2_MEASUREMENT_MAX_VALUE_ID,                                 \
            ZB_ZCL_ATTR_TYPE_U16,                                                 \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                         \
            (void *)data_ptr                                                      \
    }

void zb_zcl_co2_measurement_init_server(void);
void zb_zcl_co2_measurement_init_client(void);
#define ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_co2_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_co2_measurement_init_client
