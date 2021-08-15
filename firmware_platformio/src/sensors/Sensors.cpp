#include "Sensors.hpp"

#include "DHT22/DHT22.hpp"
#include "S8LP/S8LP.hpp"
#include "SensorError.hpp"

void Sensors_t::init()
{
    SensorDHT22.init();
    SensorS8LP.init();
}

void Sensors_t::update()
{
    SensorDHT22.update();
    SensorS8LP.update();
    SensorError.update();
}

Value<float> const &Sensors_t::get_temperature_value()
{
    return SensorDHT22.get_temperature_value();
}

Value<float> const &Sensors_t::get_humidity_value()
{
    return SensorDHT22.get_humidity_value();
}

Value<uint16_t> const &Sensors_t::get_co2_value()
{
    return SensorS8LP.get_co2_value();
}

Sensors_t Sensors{};
