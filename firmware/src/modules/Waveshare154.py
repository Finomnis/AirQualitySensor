import uasyncio as asyncio
from machine import SPI, Pin

import pins
import time

from drivers.waveshare.epaper1in54_async import EPaper
import framebuf


class Waveshare154:
    def __init__(self):
        self.interval = 5.0

        self.eink = EPaper(
            SPI(pins.EINK_SPI, baudrate=2000000),
            Pin(pins.EINK_CS),
            Pin(pins.EINK_DC),
            Pin(pins.EINK_RST),
            Pin(pins.EINK_BUSY)
        )

    async def init(self):
        t_start = time.time_ns()
        await self.eink.init()
        duration = time.time_ns() - t_start
        print("Init duration: {} ms".format(duration/1000000.0))

        # async def demo(black, color):
        #     print("setting to black({}) color({})".format(black, color))
        #     await self.eink.display_frame(bytes([black])*5000, bytes([color])*5000)
        #     print("FINISHED.")
        #     await asyncio.sleep_ms(1000)

        # await demo(0x00, 0x00)
        # await demo(0xff, 0xff)
        # await demo(0xff, 0x00)
        # await demo(0x00, 0xff)

    async def run(self):
        buf = bytearray(self.eink.width * self.eink.height // 8)
        fb = framebuf.FrameBuffer(buf, self.eink.width, self.eink.height, framebuf.MONO_HLSB)
        black = 0
        white = 1

        async def update():
            print("Display!")
            fb.fill(white)
            fb.text('Hello World {}'.format(time.time()), 30, 0, black)
            fb.pixel(30, 10, black)
            fb.hline(30, 30, 10, black)
            fb.vline(30, 50, 10, black)
            fb.line(30, 70, 40, 80, black)
            fb.rect(30, 90, 10, 10, black)
            fb.fill_rect(30, 110, 10, 10, black)
            await self.eink.display_frame(buf)

        async def wait_for_interval():
            await asyncio.sleep(self.interval)

        while True:
            await asyncio.gather(
                update(),
                wait_for_interval()
            )
