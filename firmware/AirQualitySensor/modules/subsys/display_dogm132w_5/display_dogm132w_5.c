#include "display_dogm132w_5.h"

#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(display_dogm132w_5);

static void display_dogm132w_5_entry_point(void *, void *, void *);

K_THREAD_DEFINE(display_dogm132w_5, CONFIG_SUBSYS_DISPLAY_DOGM132W_5_STACK_SIZE,
                display_dogm132w_5_entry_point, NULL, NULL, NULL,
                CONFIG_SUBSYS_DISPLAY_DOGM132W_5_THREAD_PRIORITY, 0, 0);

K_MUTEX_DEFINE(dogm132w_5_mutex);

static void display_dogm132w_5_entry_point(void *u1, void *u2, void *u3)
{
    while (1)
    {
        k_sleep(K_MSEC(1000));
    }
}
