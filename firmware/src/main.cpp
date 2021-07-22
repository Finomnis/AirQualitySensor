#include <Arduino.h>

#include "peripherals/pinout.hpp"

#include "display/Display.hpp"
#include "leds/LEDs.hpp"
#include "homie/HomieDevice.hpp"
#include "sensors/Sensors.hpp"

void setup()
{
    Peripherals::setup();

    LEDs.init();
    Display.init();

    Sensors.init();

    HomieDevice.init();
}

void loop()
{
    Sensors.update();

    LEDs.update();
    Display.update();

    HomieDevice.update();
}
