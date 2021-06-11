from homie.node import HomieNode
from homie.property import HomieProperty
from homie.constants import FLOAT
from machine import Pin

import dht
import uasyncio as asyncio


class DHT22(HomieNode):
    def __init__(self, id: str, name: str, pin: int, interval: float):
        super().__init__(id=id, name=name, type="dht22")

        self.dht22 = dht.DHT22(Pin(pin, Pin.IN))

        self.interval = interval

        self.property_temperature = HomieProperty(
            id="temperature",
            name="Temperature",
            datatype=FLOAT,
            format="-40:80",
            unit="°C",
        )
        self.add_property(self.property_temperature)

        self.property_humidity = HomieProperty(
            id="humidity",
            name="Humidity",
            datatype=FLOAT,
            format="0:100",
            unit="%",
        )
        self.add_property(self.property_humidity)

    async def run(self):
        while True:
            try:
                self.dht22.measure()

                temperature: float = self.dht22.temperature()
                humidity: float = self.dht22.humidity()
                self.property_temperature.data = str(temperature)
                self.property_humidity.data = str(humidity)

            except Exception as e:
                print("Error in DHT22: ", e)

            await asyncio.sleep(self.interval)
