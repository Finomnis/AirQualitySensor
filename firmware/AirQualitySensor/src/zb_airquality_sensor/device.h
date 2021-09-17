#pragma once

#include <zboss_api.h>

extern zb_af_device_ctx_t device_ctx;
void publish_temperature(zb_int16_t value);
void publish_humidity(zb_uint16_t value);
