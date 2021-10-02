#pragma once

#include <device.h>
#include <drivers/sensor.h>

struct sensair_s8_data
{
    uint16_t sample;
    int modbus_iface;
    uint8_t modbus_sensor_address;
};
