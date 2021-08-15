#include "Startup.hpp"

#include "../utils/TimeHelpers.hpp"
#include "../sensors/SensorError.hpp"

Startup::Startup(uint32_t duration)
    : ValueWatcher<bool>{value},
      value{true},
      startup_end{millis() + duration},
      sensor_error_value{SensorError.get_value()}
{
}

void Startup::update()
{
    Serial.println(sensor_error_value.get());
    if (value.get() && (!sensor_error_value.get() || event_is_over(startup_end)))
    {
        value.update(false);
    }
}
