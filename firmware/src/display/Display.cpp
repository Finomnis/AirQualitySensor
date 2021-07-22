#include "Display.hpp"

#include "../peripherals/pinout.hpp"
#include "../utils/TextHelper/U8g2.hpp"
#include "../utils/TimeHelpers.hpp"

#include "sensors/Sensors.hpp"

Display_t::Display_t()
    : display{Peripherals::display},
      startup{STARTUP_DISPLAY_TIME},
      temperature_value{Sensors.get_temperature_value()},
      humidity_value{Sensors.get_humidity_value()},
      co2_value{Sensors.get_co2_value()}
{
}

void Display_t::init()
{
    display.begin();
    redraw();
}

void Display_t::update()
{
    startup.update();

    if (temperature_value.new_value_available() ||
        humidity_value.new_value_available() ||
        co2_value.new_value_available() ||
        startup.new_value_available())
    {
        redraw();
    }
}

void Display_t::render_startup_screen()
{
    display.clearBuffer();
    display.setFont(u8g2_font_luBIS14_te);
    TextHelper::drawText(display, "Starting...",
                         display.getWidth() / 2,
                         display.getHeight() / 2,
                         TextHelper::H_CENTER,
                         TextHelper::V_CENTER);

    display.sendBuffer();
}

void Display_t::redraw()
{
    temperature_value.clear();
    humidity_value.clear();
    co2_value.clear();

    if (startup.get())
    {
        render_startup_screen();
        return;
    }

    display.clearBuffer();

    char textBuffer[128];

    // Draw CO2
    display.setFont(u8g2_font_luBIS19_tn);
    if (co2_value.is_valid())
    {
        snprintf(textBuffer, sizeof(textBuffer), "%d", co2_value.get());
    }
    else
    {
        snprintf(textBuffer, sizeof(textBuffer), "---");
    }
    TextHelper::drawText(display, textBuffer,
                         display.getWidth() - 1,
                         display.getHeight() / 2,
                         TextHelper::H_RIGHT,
                         TextHelper::V_CENTER);

    // Draw Temperature
    display.setFont(u8g2_font_helvB14_tf);
    if (temperature_value.is_valid())
    {
        snprintf(textBuffer, sizeof(textBuffer), "%.0f \260C", temperature_value.get());
    }
    else
    {
        snprintf(textBuffer, sizeof(textBuffer), "-- \260C");
    }
    TextHelper::drawText(display, textBuffer,
                         0,
                         display.getHeight() / 4,
                         TextHelper::H_LEFT,
                         TextHelper::V_CENTER);

    // Draw Humidity
    if (humidity_value.is_valid())
    {
        snprintf(textBuffer, sizeof(textBuffer), "%.0f %%", humidity_value.get());
    }
    else
    {
        snprintf(textBuffer, sizeof(textBuffer), "-- %%");
    }
    TextHelper::drawText(display, textBuffer,
                         0,
                         (3 * display.getHeight()) / 4,
                         TextHelper::H_LEFT,
                         TextHelper::V_CENTER);

    display.sendBuffer();
}

Display_t Display{};
