#pragma once

#include <drivers/sensor.h>

void publish_temperature(struct sensor_value value);
void publish_humidity(struct sensor_value value);

void publish_pending_attributes();
