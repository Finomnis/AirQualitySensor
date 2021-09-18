#pragma once

#include <zboss_api.h>

extern zb_af_device_ctx_t airquality_sensor_device_ctx;
void zb_airquality_sensor_publish_temperature(zb_int16_t value);
void zb_airquality_sensor_publish_humidity(zb_uint16_t value);
void zb_airquality_sensor_publish_co2(float value);
