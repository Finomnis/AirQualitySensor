#include "SensorError.hpp"

#include "Sensors.hpp"

SensorError_t::SensorError_t()
    : temperature_sensor{Sensors.get_temperature_value()},
      humidity_sensor{Sensors.get_humidity_value()},
      co2_sensor{Sensors.get_co2_value()},
      value{temperature_sensor.is_valid() &&
            humidity_sensor.is_valid() &&
            co2_sensor.is_valid()}
{
}

void SensorError_t::update()
{
    bool has_error = !(temperature_sensor.is_valid() && humidity_sensor.is_valid() && co2_sensor.is_valid());

    if (has_error != value.get())
    {
        value.update(has_error);
    }
}

SensorError_t SensorError{};
