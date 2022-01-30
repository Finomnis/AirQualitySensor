use embedded_hal::blocking::i2c::{Read, Write, WriteRead};

use crate::hal::{self, gpio, stm32};

pub trait I2cConnection:
    Write<Error = stm32g0xx_hal::i2c::Error>
    + Read<Error = stm32g0xx_hal::i2c::Error>
    + WriteRead<Error = stm32g0xx_hal::i2c::Error>
{
}

impl I2cConnection
    for hal::i2c::I2c<
        stm32::I2C2,
        gpio::gpioa::PA12<gpio::Output<gpio::OpenDrain>>,
        gpio::gpioa::PA11<gpio::Output<gpio::OpenDrain>>,
    >
{
}
