
from homie.device import HomieDevice
import uasyncio as asyncio

import settings
import pins

from sensors.DHT22 import DHT22


async def main():
    # Initialize the Homie device
    device = HomieDevice(settings)

    sensor_dht22 = DHT22(id="am2302", name="AM2302", pin=pins.DHT22, interval=2)

    device.add_node(sensor_dht22)

    # Run
    await asyncio.gather(
        device.run(),
        sensor_dht22.run(),
    )


if __name__ == "__main__":
    asyncio.run(main())
