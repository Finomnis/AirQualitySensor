#include <Arduino.h>

#include "peripherals/pinout.hpp"

#include "display/Display.hpp"
#include "homie/HomieDevice.hpp"
#include "sensors/Sensors.hpp"

void setup()
{
    Peripherals::setup();

    Display.init();

    Sensors.init();

    HomieDevice.init();
}

void loop()
{
    Sensors.update();

    HomieDevice.update();
}
