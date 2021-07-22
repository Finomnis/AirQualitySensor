from machine import Pin
import uasyncio as asyncio

import pins

LED_STATE_EXCELLENT = 0
LED_STATE_OK = 1
LED_STATE_WARN_WEAK = 2
LED_STATE_WARN_STRONG = 3
LED_STATE_ERROR = 4
LED_STATE_NONE = 5


class LEDs:
    def __init__(self):
        self.led_red = Pin(pins.LED_RED, mode=Pin.OUT, value=0)
        self.led_yellow = Pin(pins.LED_YELLOW, mode=Pin.OUT, value=0)
        self.led_green = Pin(pins.LED_GREEN, mode=Pin.OUT, value=0)

        self.blink_state_on = True

        self.led_state = LED_STATE_NONE

        self._update_leds()

    async def on_co2_changed(self, value):
        if value is None:
            self.led_state = LED_STATE_ERROR
        elif value >= 1450:
            self.led_state = LED_STATE_WARN_STRONG
        elif value >= 1000:
            self.led_state = LED_STATE_WARN_WEAK
        elif value >= 600:
            self.led_state = LED_STATE_OK
        elif value < 600:
            self.led_state = LED_STATE_EXCELLENT
        else:
            self.led_state = LED_STATE_NONE

        self._update_leds()

    async def run(self):
        while True:
            await asyncio.sleep_ms(600)
            self.blink_state_on = not self.blink_state_on
            self._update_leds()

    def _update_leds(self):
        green = 0
        yellow = 0
        red = 0

        if self.led_state == LED_STATE_ERROR:
            if self.blink_state_on:
                red = 1
                yellow = 1
                green = 1
        elif self.led_state == LED_STATE_WARN_STRONG:
            red = 1
        elif self.led_state == LED_STATE_WARN_WEAK:
            yellow = 1
        elif self.led_state == LED_STATE_OK:
            green = 1
        elif self.led_state == LED_STATE_EXCELLENT:
            if self.blink_state_on:
                green = 1
        else:
            red = 1
            yellow = 1
            green = 1

        self.led_green.value(green)
        self.led_yellow.value(yellow)
        self.led_red.value(red)
