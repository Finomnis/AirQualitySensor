#pragma once

#include <drivers/sensor.h>

typedef void (*senseair_s8_value_cb)(struct sensor_value value);

void senseair_s8_register_co2_handler(senseair_s8_value_cb cb);

static const struct sensor_value SENSEAIR_S8_ERROR_VALUE = {.val1 = 0, .val2 = -1};
