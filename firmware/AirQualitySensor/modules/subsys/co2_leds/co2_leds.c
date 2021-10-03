#include "co2_leds.h"

#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(co2_leds);

static void co2_leds_entry_point(void *, void *, void *);

K_THREAD_DEFINE(co2_leds, CONFIG_SUBSYS_CO2_LEDS_STACK_SIZE,
                co2_leds_entry_point, NULL, NULL, NULL,
                CONFIG_SUBSYS_CO2_LEDS_THREAD_PRIORITY, 0, 0);

static enum LED_STATE { LED_STATE_INIT,
                        LED_STATE_ERROR,
                        LED_STATE_EXCELLENT,
                        LED_STATE_OK,
                        LED_STATE_WARN_WEAK,
                        LED_STATE_WARN_STRONG,
} led_state = LED_STATE_INIT;

static bool blink_state = false;

K_MUTEX_DEFINE(co2_leds_mutex);

static void update_leds()
{
    bool red_on = false;
    bool yellow_on = false;
    bool green_on = false;
    switch (led_state)
    {
    case LED_STATE_INIT:
        red_on = true;
        yellow_on = true;
        green_on = true;
        break;
    case LED_STATE_ERROR:
        red_on = blink_state;
        yellow_on = blink_state;
        green_on = blink_state;
        break;
    case LED_STATE_EXCELLENT:
        green_on = blink_state;
        break;
    case LED_STATE_OK:
        green_on = true;
        break;
    case LED_STATE_WARN_WEAK:
        yellow_on = true;
        break;
    case LED_STATE_WARN_STRONG:
        red_on = true;
        break;
    }

    LOG_INF("LEDs: %s %s %s", red_on ? "R" : " ", yellow_on ? "Y" : " ", green_on ? "G" : " ");
}

void co2_leds_set_co2_level(struct sensor_value value)
{
    // TODO: LOCK
    uint16_t co2_value = value.val2;

    enum LED_STATE new_led_state;
    if (co2_value < 0)
    {
        new_led_state = LED_STATE_ERROR;
    }
    else if (co2_value <= CONFIG_SUBSYS_CO2_LEDS_LEVEL_EXCELLENT)
    {
        new_led_state = LED_STATE_EXCELLENT;
    }
    else if (co2_value >= CONFIG_SUBSYS_CO2_LEDS_LEVEL_WARN_STRONG)
    {
        new_led_state = LED_STATE_WARN_STRONG;
    }
    else if (co2_value >= CONFIG_SUBSYS_CO2_LEDS_LEVEL_WARN_WEAK)
    {
        new_led_state = LED_STATE_WARN_WEAK;
    }
    else
    {
        new_led_state = LED_STATE_OK;
    }

    k_mutex_lock(&co2_leds_mutex, K_FOREVER);
    if (led_state != new_led_state)
    {
        led_state = new_led_state;
        update_leds();
    }
    k_mutex_unlock(&co2_leds_mutex);
}

static void co2_leds_entry_point(void *u1, void *u2, void *u3)
{
    while (1)
    {
        k_sleep(K_MSEC(CONFIG_SUBSYS_CO2_LEDS_BLINK_PERIOD_MS));

        k_mutex_lock(&co2_leds_mutex, K_FOREVER);
        blink_state = !blink_state;
        update_leds();
        k_mutex_unlock(&co2_leds_mutex);
    }
}
