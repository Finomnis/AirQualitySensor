mod data;

use embedded_hal::blocking::i2c::{Read, Write, WriteRead};

#[derive(Debug, defmt::Format)]
pub enum SCD4xError {
    I2cNack,
    I2cOverrun,
    I2cPECError,
    I2cBusError,
    I2cArbitrationLost,
    ChecksumError,
    ByteCountError,
}

fn from_i2c_error(e: crate::hal::i2c::Error) -> SCD4xError {
    match e {
        stm32g0xx_hal::i2c::Error::Overrun => SCD4xError::I2cOverrun,
        stm32g0xx_hal::i2c::Error::Nack => SCD4xError::I2cNack,
        stm32g0xx_hal::i2c::Error::PECError => SCD4xError::I2cPECError,
        stm32g0xx_hal::i2c::Error::BusError => SCD4xError::I2cBusError,
        stm32g0xx_hal::i2c::Error::ArbitrationLost => SCD4xError::I2cArbitrationLost,
    }
}

const I2C_ADDR: u8 = 0x62;

pub struct SCD4xController<'a, I2C> {
    i2c: &'a mut I2C,
}

impl<'a, I2C> SCD4xController<'a, I2C>
where
    I2C: Write<Error = stm32g0xx_hal::i2c::Error>
        + Read<Error = stm32g0xx_hal::i2c::Error>
        + WriteRead<Error = stm32g0xx_hal::i2c::Error>,
{
    pub fn new(i2c: &'a mut I2C) -> Self {
        Self { i2c }
    }

    pub fn get_serial_number(self) -> Result<u64, SCD4xError> {
        let mut response = [0u8; 9];
        self.i2c
            .write_read(I2C_ADDR, &[0x36, 0x82], &mut response)
            .map_err(from_i2c_error)?;

        let mut serial = 0;
        for num in data::convert_response_to_words(&response) {
            serial = serial << 16 | num? as u64;
        }

        Ok(serial)
    }
}
