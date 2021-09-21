#pragma once

#include <drivers/sensor.h>

void initialize_zigbee_device();

void publish_temperature(struct sensor_value value);
void publish_humidity(struct sensor_value value);
