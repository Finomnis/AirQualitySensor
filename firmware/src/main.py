from homie.device import HomieDevice
import uasyncio as asyncio

import settings
import pins

from modules.DHT22 import DHT22
from modules.SensairS8 import SensairS8


async def main():
    # Initialize the Homie device
    device = HomieDevice(settings)

    sensor_dht22 = DHT22(id="dht22", name="DHT22", pin=pins.DHT22, interval=2)
    sensor_s8lp = SensairS8(id="s8lp", name="S8LP", interval=4)

    device.add_node(sensor_dht22)
    device.add_node(sensor_s8lp)

    # Run
    await asyncio.gather(
        device.run(),
        sensor_dht22.run(),
        sensor_s8lp.run(),
    )


if __name__ == "__main__":
    asyncio.run(main())
