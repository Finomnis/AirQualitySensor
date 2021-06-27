from homie.node import HomieNode
from homie.property import HomieProperty
from homie.constants import FLOAT

import uasyncio as asyncio
from machine import UART


class SensairS8(HomieNode):
    def __init__(self, id: str, name: str, interval: float):
        super().__init__(id=id, name=name, type="s8lp")

        uart = UART(2, 9600)
        self.sensor_writer = asyncio.StreamWriter(uart, {})
        self.sensor_reader = asyncio.StreamReader(uart)

        self.interval = interval

        self.property_co2 = HomieProperty(
            id="co2",
            name="CO2",
            datatype=FLOAT,
            format="0:10000",
            unit="ppm",
        )
        self.add_property(self.property_co2)

    @staticmethod
    def _crc(data: bytes):
        crc: int = 0xffff

        for next_data in data:
            crc ^= next_data
            for _ in range(8):
                if ((crc & 0x0001) != 0):
                    crc >>= 1
                    crc ^= 0xA001
                else:
                    crc >>= 1

        result_high = (crc >> 8) & 255
        result_low = crc & 255

        return bytes([result_low, result_high])

    async def run(self):
        async def update():
            await self.sensor_writer.awrite(b'\xfe\x04\x00\x00\x00\x04\xe5\xc6')
            response = await self.sensor_reader.readexactly(13)

            if len(response) != 13 or response[:3] != b'\xfe\x04\x08':
                raise RuntimeError("Invalid packet received!")

            expected_crc = self._crc(response[:-2])
            if expected_crc != response[-2:]:
                raise RuntimeError("Invalid CRC!")

            co2_data = int.from_bytes(response[9:11], 'big')
            error_codes = int.from_bytes(response[3:5], 'big')
            if error_codes:
                raise RuntimeError("Sensor reports error: {}".format(error_codes))

            self.property_co2.data = str(co2_data)

        while True:
            try:
                await asyncio.wait_for_ms(update(), 500)
            except Exception as e:
                print("Error in S8LP: ", e)
            await asyncio.sleep(self.interval)
