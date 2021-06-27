from micropython import const
from machine import SPI, Pin
import utime

import uasyncio as asyncio

CMD_DRIVER_OUTPUT_CONTROL = const(0x01)
CMD_DATA_ENTRY_MODE_SETTING = const(0x11)
CMD_SW_RESET = const(0x12)
CMD_MASTER_ACTIVATION = const(0x20)
CMD_DISPLAY_UPDATE_CONTROL_2 = const(0x22)
CMD_WRITE_RAM_BLACK = const(0x24)
CMD_WRITE_RAM_COLOR = const(0x26)
CMD_SET_RAM_X_ADDRESS_START_END = const(0x44)
CMD_SET_RAM_Y_ADDRESS_START_END = const(0x45)
CMD_SET_RAM_X_ADDRESS_COUNTER = const(0x4E)
CMD_SET_RAM_Y_ADDRESS_COUNTER = const(0x4F)

# Display resolution
EPD_WIDTH = const(200)
EPD_HEIGHT = const(200)


class EPaper:

    def __init__(self, spi: SPI, cs: Pin, dc: Pin, rst: Pin, busy: Pin):
        self.spi = spi
        self.cs = cs
        self.dc = dc
        self.rst = rst
        self.busy = busy
        self.cs.init(self.cs.OUT, value=1)
        self.dc.init(self.dc.OUT, value=1)
        self.rst.init(self.rst.OUT, value=1)
        self.busy.init(self.busy.IN)

        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    async def init(self):
        # Reset
        await self._reset()

        # Basic init
        self._write_spi(
            CMD_DRIVER_OUTPUT_CONTROL,
            bytes([
                (EPD_HEIGHT - 1) & 0xFF,
                ((EPD_HEIGHT - 1) >> 8) & 0xFF,
                0x00
            ])
        )

        # Data entry sequence setting
        self._write_spi(
            CMD_DATA_ENTRY_MODE_SETTING,
            bytes([0x11])
        )

        # Set RAMX Start/end position
        self._write_spi(
            CMD_SET_RAM_X_ADDRESS_START_END,
            bytes([0x00, 0x18])
        )

        # Set RAMY Start/end position
        self._write_spi(
            CMD_SET_RAM_Y_ADDRESS_START_END,
            bytes([0x00, 0x00, 0xC7, 0x00])
        )

        # Load operation LUT
        self._write_spi(
            CMD_DISPLAY_UPDATE_CONTROL_2,
            bytes([0xb1])
        )

        # Execute operation
        self._write_spi(
            CMD_MASTER_ACTIVATION
        )

        await self.wait_until_idle()

    def _write_spi(self, command: int, data=None):
        self.dc(0)
        self.cs(0)
        self.spi.write(bytes([command]))
        if data is not None:
            self.dc(1)
            self.spi.write(data)
            self.dc(0)
        self.cs(1)

    async def _reset(self):
        # Panel reset
        self.rst(0)
        await asyncio.sleep_ms(10)
        self.rst(1)
        await asyncio.sleep_ms(10)
        # Software reset
        self._write_spi(CMD_SW_RESET)

    async def wait_until_idle(self):
        while self.busy():
            await asyncio.sleep_ms(1)
        asyncio.sleep_ms(100)

    async def display_frame(self, data_bw, data_col=None):
        self._write_spi(
            CMD_SET_RAM_X_ADDRESS_COUNTER,
            bytes([0x00])
        )
        self._write_spi(
            CMD_SET_RAM_Y_ADDRESS_COUNTER,
            bytes([0x00, 0x00])
        )
        self._write_spi(
            CMD_WRITE_RAM_BLACK,
            data_bw
        )

        if data_col is not None:
            self._write_spi(
                CMD_SET_RAM_X_ADDRESS_COUNTER,
                bytes([0x00])
            )
            self._write_spi(
                CMD_SET_RAM_Y_ADDRESS_COUNTER,
                bytes([0x00, 0x00])
            )
            self._write_spi(
                CMD_WRITE_RAM_COLOR,
                data_col
            )

        # Load operation LUT
        self._write_spi(
            CMD_DISPLAY_UPDATE_CONTROL_2,
            bytes([0xF7])
        )

        # Execute operation
        self._write_spi(
            CMD_MASTER_ACTIVATION
        )

        await self.wait_until_idle()
