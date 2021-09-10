#pragma once

#include <zboss_api.h>

/**
 * @brief Sets and publishes a zigbee attribute value
 *
 * @param ep - endpoint number
 * @param cluster_id - cluster ID (see @ref zb_zcl_cluster_id_t)
 * @param cluster_role - role (see @ref zcl_cluster_role)
 * @param attr_id - ID of the attribute being set
 * @param value - pointer to the value
 *
 * @return ZB_ZCL_STATUS_SUCCESS if value was written \n
 *         ZB_ZCL_STATUS_READ_ONLY if attribute is read only and check_access is false
 *         ZB_ZCL_STATUS_UNSUP_ATTRIB if attribute is not supported by cluster
 *         ZB_ZCL_STATUS_INVALID_VALUE if current value of attribute is invalid
 * @note given endpoint with given cluster ID should exist
 */
zb_zcl_status_t publish_zigbee_attribute(zb_uint8_t ep, zb_uint16_t cluster_id,
                                         zb_uint8_t cluster_role, zb_uint16_t attr_id,
                                         void *value);
