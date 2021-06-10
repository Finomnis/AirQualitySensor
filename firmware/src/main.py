import uasyncio


async def main():
    while True:
        print("Blub.")
        await uasyncio.sleep(1.0)


if __name__ == "__main__":
    uasyncio.run(main())
