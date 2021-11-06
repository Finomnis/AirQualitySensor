#pragma once

#include <zboss_api.h>

#ifndef NAN
#define NAN (__builtin_nanf(""))
#define isnan(x) ((x) != (x))
#endif

#define ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT 0x040DU /*!< CO2 measurement */

/*! @brief CO2 Measurement cluster attribute identifiers
*/
enum zb_zcl_co2_measurement_attr_e
{
    /*! @brief MeasuredValue, ZCL spec 4.4.2.2.1.1 */
    ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID = 0x0000,
};

/** @brief MeasuredValue attribute unknown value */
#define ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_UNKNOWN ((float)NAN)

/** @brief Default value for MeasurementValue attribute */
#define ZB_ZCL_CO2_MEASUREMENT_VALUE_DEFAULT_VALUE ((float)NAN)

/** @brief Declare attribute list for CO2 Measurement cluster - server side
    @param attr_list - attribute list name
    @param value - pointer to variable to store MeasuredValue attribute
*/
#define ZB_ZCL_DECLARE_CO2_MEASUREMENT_ATTRIB_LIST(attr_list, value)    \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID, (value)) \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID(data_ptr) \
    {                                                                         \
        ZB_ZCL_ATTR_CO2_MEASUREMENT_VALUE_ID,                                 \
            ZB_ZCL_ATTR_TYPE_SINGLE,                                          \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,      \
            (void *)data_ptr                                                  \
    }

void zb_zcl_co2_measurement_init_server(void);
void zb_zcl_co2_measurement_init_client(void);
#define ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_co2_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_CO2_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_co2_measurement_init_client
