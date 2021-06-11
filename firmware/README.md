# General

## Software Versions of Development Process

-   esp32-micropython 1.15.0
-   microhomie 3.0.2

## Protocols

-   Sensair S8:
    -   UART/Modbus
    -   https://rmtplusstoragesenseair.blob.core.windows.net/docs/Dev/publicerat/TDE2067.pdf
    -   Power Rail: 5V, but doesn't require level shifter
    -   UART: 3.3V
-   DHT22:
    -   1-wire DHT bus
    -   Power Rail: 3.3V / 5V (preferred 3.3V for ESP32)
    -   Bus: must match power rail (preferred 3.3V ESP32)
-   Display:
    -   SPI
    -   to be determined

# Software Setup

## MicroPython

MicroPython is the base firmware on the ESP32. It has to be flashed to the ESP32 once.

After that, the device can be programmed by uploading scripts to it.

To install MicroPython on the ESP32:

1. Download firmware from https://micropython.org/download/esp32/
2. Clear flash:

    - `esptool --chip esp32 --port <COM-Port> erase_flash`

3. Upload firmware:

    - `esptool --chip esp32 --port <COM-Port> --baud 115200 write_flash -z 0x1000 esp32-20210418-v1.15.bin`

## micropy

micropy-cli is a command line tool that assists with MicroPython script development.

It is responsible to provide stubs, install packages and other convenience features for
VSCode.

Most of the information is taken from https://lemariva.com/blog/2019/08/micropython-vsc-ide-intellisense.

Do the following steps for initial setup:

1. Install micropy-cli

    - `pip install micropy-cli`

2. Install micropy-stubs

    - `micropy stubs search esp32`
    - `micropy stubs add esp32-micropython-1.15.0`

3. Update dependencies
    - `micropy install`

It is important in the entire process of development that the `firmware` folder is the root folder opened in VSCode.
