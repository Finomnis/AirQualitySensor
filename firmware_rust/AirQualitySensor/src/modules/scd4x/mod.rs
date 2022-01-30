mod controller;
mod data;
mod measurement;
mod tasks;

pub use controller::SCD4xController;
pub use tasks::SCD4xTasks;

#[derive(Clone)]
pub struct Measurement {
    raw_data: [u16; 3],
}

#[derive(Debug, defmt::Format)]
pub enum SCD4xError {
    I2cNack,
    I2cOverrun,
    I2cPECError,
    I2cBusError,
    I2cArbitrationLost,
    ChecksumError,
    ByteCountError,
    SelfTestFailed,
}

impl SCD4xError {
    fn from_i2c_error(e: crate::hal::i2c::Error) -> SCD4xError {
        match e {
            stm32g0xx_hal::i2c::Error::Overrun => SCD4xError::I2cOverrun,
            stm32g0xx_hal::i2c::Error::Nack => SCD4xError::I2cNack,
            stm32g0xx_hal::i2c::Error::PECError => SCD4xError::I2cPECError,
            stm32g0xx_hal::i2c::Error::BusError => SCD4xError::I2cBusError,
            stm32g0xx_hal::i2c::Error::ArbitrationLost => SCD4xError::I2cArbitrationLost,
        }
    }
}
