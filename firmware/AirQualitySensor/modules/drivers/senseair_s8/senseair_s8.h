#pragma once

#include <device.h>
#include <drivers/sensor.h>

struct senseair_s8_data
{
    bool error;
    int16_t sample;
    int modbus_iface;
    uint8_t modbus_sensor_address;
};
