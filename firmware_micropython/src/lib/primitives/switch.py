# switch.py

# Copyright (c) 2018-2020 Peter Hinch
# Released under the MIT License (MIT) - see LICENSE file

import uasyncio as asyncio
import utime as time
from . import launch

class Switch:
    debounce_ms = 50
    def __init__(self, pin):
        self.pin = pin # Should be initialised for input with pullup
        self._open_func = False
        self._close_func = False
        self.switchstate = self.pin.value()  # Get initial state
        asyncio.create_task(self.switchcheck())  # Thread runs forever

    def open_func(self, func, args=()):
        self._open_func = func
        self._open_args = args

    def close_func(self, func, args=()):
        self._close_func = func
        self._close_args = args

    # Return current state of switch (0 = pressed)
    def __call__(self):
        return self.switchstate

    async def switchcheck(self):
        while True:
            state = self.pin.value()
            if state != self.switchstate:
                # State has changed: act on it now.
                self.switchstate = state
                if state == 0 and self._close_func:
                    launch(self._close_func, self._close_args)
                elif state == 1 and self._open_func:
                    launch(self._open_func, self._open_args)
            # Ignore further state changes until switch has settled
            await asyncio.sleep_ms(Switch.debounce_ms)
