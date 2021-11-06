#pragma once

#include <drivers/sensor.h>

typedef void (*senseair_s8_value_cb)(struct sensor_value value, bool error);

void senseair_s8_register_co2_handler(senseair_s8_value_cb cb);
