#pragma once

#include <drivers/sensor.h>

typedef void (*dht22_value_cb)(struct sensor_value value);

void dht22_register_temperature_handler(dht22_value_cb cb);
void dht22_register_humidity_handler(dht22_value_cb cb);

static const struct sensor_value DHT22_ERROR_VALUE = {.val1 = 0, .val2 = -1};
