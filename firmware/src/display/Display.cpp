#include "Display.hpp"

#include "../peripherals/pinout.hpp"
#include "../utils/TextHelper/U8g2.hpp"

#include "sensors/Sensors.hpp"

void Display_t::init()
{
    display.begin();
}

void Display_t::update()
{
    static bool done = false;

    if (done)
        return;

    done = true;

    display.clearBuffer();

    display.setFont(u8g2_font_ncenB14_tr);
    TextHelper::drawText(display, "Hello World!",
                         display.getWidth() / 2,
                         display.getHeight() / 2,
                         TextHelper::H_CENTER,
                         TextHelper::V_CENTER);

    display.sendBuffer();
}

Display_t::Display_t()
    : display{Peripherals::display},
      temperature_value{Sensors.get_temperature_value()},
      humidity_value{Sensors.get_humidity_value()},
      co2_value{Sensors.get_co2_value()}
{
}

Display_t Display{};
