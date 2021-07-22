#include "Display.hpp"

#include "../peripherals/pinout.hpp"
#include "../utils/TextHelper/U8g2.hpp"

#include "sensors/Sensors.hpp"

void Display_t::init()
{
    display.begin();
    redraw();
}

void Display_t::update()
{
    if (temperature_value.new_value_available() ||
        humidity_value.new_value_available() ||
        co2_value.new_value_available())
    {
        redraw();
    }
}

void Display_t::redraw()
{
    temperature_value.clear();
    humidity_value.clear();
    co2_value.clear();

    display.clearBuffer();

    char textBuffer[128];

    if (co2_value.is_valid())
    {
        snprintf(textBuffer, sizeof(textBuffer), "%d", co2_value.get());
    }
    else
    {
        snprintf(textBuffer, sizeof(textBuffer), "--");
    }

    //display.setFont(u8g2_font_fub25_tf);
    //display.setFont(u8g2_font_logisoso28_tn);
    display.setFont(u8g2_font_luBIS19_tn);
    TextHelper::drawText(display, textBuffer,
                         display.getWidth() - 1,
                         display.getHeight() / 2,
                         TextHelper::H_RIGHT,
                         TextHelper::V_CENTER);

    if (temperature_value.is_valid())
    {
        snprintf(textBuffer, sizeof(textBuffer), "%.0f \260C", temperature_value.get());
    }
    else
    {
        snprintf(textBuffer, sizeof(textBuffer), "-- \260C");
    }

    //display.setFont(u8g2_font_fub25_tf);
    //display.setFont(u8g2_font_logisoso28_tn);
    display.setFont(u8g2_font_helvR14_tf);
    TextHelper::drawText(display, textBuffer,
                         0,
                         display.getHeight() / 4,
                         TextHelper::H_LEFT,
                         TextHelper::V_CENTER);

    if (humidity_value.is_valid())
    {
        snprintf(textBuffer, sizeof(textBuffer), "%.0f %%", humidity_value.get());
    }
    else
    {
        snprintf(textBuffer, sizeof(textBuffer), "-- %%");
    }

    //display.setFont(u8g2_font_fub25_tf);
    //display.setFont(u8g2_font_logisoso28_tn);
    display.setFont(u8g2_font_helvR14_tf);
    TextHelper::drawText(display, textBuffer,
                         0,
                         (3 * display.getHeight()) / 4,
                         TextHelper::H_LEFT,
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
