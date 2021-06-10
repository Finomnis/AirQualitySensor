
from homie.device import HomieDevice

import uasyncio as asyncio

import settings


async def main():
    # Initialize the Homie device
    device = HomieDevice(settings)

    # Run
    await device.run()


if __name__ == "__main__":
    asyncio.run(main())
