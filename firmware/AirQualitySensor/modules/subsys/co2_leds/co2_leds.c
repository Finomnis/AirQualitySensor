#include "co2_leds.h"

#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(co2_leds);

static void co2_leds_entry_point(void *, void *, void *);

K_THREAD_DEFINE(co2_leds, CONFIG_SUBSYS_CO2_LEDS_STACK_SIZE,
                co2_leds_entry_point, NULL, NULL, NULL,
                CONFIG_SUBSYS_CO2_LEDS_THREAD_PRIORITY, 0, 0);

void co2_leds_set_co2_level(struct sensor_value value)
{
    LOG_INF("New CO2: %d ppm", value.val2);
}

static void co2_leds_entry_point(void *u1, void *u2, void *u3)
{
    while (1)
    {
        k_sleep(K_MSEC(1000));
    }
}
