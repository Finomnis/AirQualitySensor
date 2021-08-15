#include "LEDs.hpp"

#include "../Peripherals/pinout.hpp"
#include "../sensors/Sensors.hpp"

LEDs_t::LEDs_t()
    : blink_state{BLINK_PERIOD},
      blink_value{blink_state.get_blink_value()},
      co2_value{Sensors.get_co2_value()}
{
}

void LEDs_t::init()
{
    pinMode(PINS::LED_RED, OUTPUT);
    pinMode(PINS::LED_YELLOW, OUTPUT);
    pinMode(PINS::LED_GREEN, OUTPUT);

    update_leds();
}

void LEDs_t::update()
{
    blink_state.update();

    if (blink_value.new_value_available() ||
        co2_value.new_value_available())
    {
        update_leds();
    }
}

LEDs_t::LED_STATE LEDs_t::compute_led_state()
{
    if (!co2_value.is_valid())
        return LED_STATE_ERROR;

    const uint16_t value = co2_value.get();

    if (value >= 1450)
        return LED_STATE_WARN_STRONG;
    if (value >= 1000)
        return LED_STATE_WARN_WEAK;
    if (value >= 600)
        return LED_STATE_OK;
    if (value < 600)
        return LED_STATE_EXCELLENT;

    return LED_STATE_NONE;
}

void LEDs_t::update_leds()
{
    LED_STATE led_state = compute_led_state();

    uint8_t green = 0;
    uint8_t yellow = 0;
    uint8_t red = 0;

    if (led_state == LED_STATE_ERROR)
    {
        if (blink_value.get())
        {
            red = 1;
            yellow = 1;
            green = 1;
        }
    }
    else if (led_state == LED_STATE_WARN_STRONG)
    {
        red = 1;
    }
    else if (led_state == LED_STATE_WARN_WEAK)
    {
        yellow = 1;
    }
    else if (led_state == LED_STATE_OK)
    {
        green = 1;
    }
    else if (led_state == LED_STATE_EXCELLENT)
    {
        if (blink_value.get())
        {
            green = 1;
        }
    }
    else
    {
        red = 1;
        yellow = 1;
        green = 1;
    }

    digitalWrite(PINS::LED_GREEN, green);
    digitalWrite(PINS::LED_YELLOW, yellow);
    digitalWrite(PINS::LED_RED, red);
}

LEDs_t LEDs{};
