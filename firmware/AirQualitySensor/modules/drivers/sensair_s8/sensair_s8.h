#pragma once

#include <device.h>
#include <drivers/sensor.h>

/** @brief Information collected from the sensor on each fetch. */
struct sensair_s8_sample_type
{
    /** Equivalent carbon dioxide in parts-per-million volume (ppmv). */
    uint16_t co2;

    /** Sensor status at completion of most recent fetch. */
    uint8_t status;

    /**
	 * Sensor error flags at completion of most recent fetch.
	 *
	 * Note that errors are cleared when read.
	 */
    uint8_t error;
};

struct sensair_s8_data
{
    struct sensair_s8_sample_type sample;
    int modbus_iface;
};
