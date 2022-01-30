const I2C_ADDR: u8 = 0x62;

use super::{Measurement, SCD4xError};

pub struct SCD4xController<'a, I2C> {
    i2c: &'a mut I2C,
}

impl<'a, I2C> SCD4xController<'a, I2C>
where
    I2C: crate::modules::i2c::I2cConnection,
{
    pub fn new(i2c: &'a mut I2C) -> Self {
        Self { i2c }
    }

    pub fn get_serial_number(&mut self) -> Result<u64, SCD4xError> {
        let mut response = [0u8; 9];
        self.i2c
            .write_read(I2C_ADDR, &[0x36, 0x82], &mut response)
            .map_err(SCD4xError::from_i2c_error)?;

        let mut serial = 0;
        for num in super::data::response_to_array::<3>(&response)? {
            serial = serial << 16 | num as u64;
        }

        Ok(serial)
    }

    pub fn start_self_test(&mut self) -> Result<(), SCD4xError> {
        self.i2c
            .write(I2C_ADDR, &[0x36, 0x39])
            .map_err(SCD4xError::from_i2c_error)
    }

    pub fn finish_self_test(&mut self) -> Result<(), SCD4xError> {
        let mut response = [0u8; 3];
        self.i2c
            .read(I2C_ADDR, &mut response)
            .map_err(SCD4xError::from_i2c_error)?;

        let result = super::data::response_to_array::<1>(&response)?[0];
        if 0 != result {
            return Err(SCD4xError::SelfTestFailed);
        }

        Ok(())
    }

    pub fn start_periodic_measurement(&mut self) -> Result<(), SCD4xError> {
        self.i2c
            .write(I2C_ADDR, &[0x21, 0xb1])
            .map_err(SCD4xError::from_i2c_error)
    }

    pub fn stop_periodic_measurement(&mut self) -> Result<(), SCD4xError> {
        self.i2c
            .write(I2C_ADDR, &[0x3f, 0x86])
            .map_err(SCD4xError::from_i2c_error)
    }

    pub fn get_data_ready_status(&mut self) -> Result<bool, SCD4xError> {
        let mut response = [0u8; 3];
        self.i2c
            .write_read(I2C_ADDR, &[0xe4, 0xb8], &mut response)
            .map_err(SCD4xError::from_i2c_error)?;

        let data = super::data::response_to_array::<1>(&response)?[0];

        let ready = data & 0x07ff != 0;

        Ok(ready)
    }

    pub fn read_measurement(&mut self) -> Result<Measurement, SCD4xError> {
        let mut response = [0u8; 9];
        self.i2c
            .write_read(I2C_ADDR, &[0xec, 0x05], &mut response)
            .map_err(SCD4xError::from_i2c_error)?;

        let data = super::data::response_to_array::<3>(&response)?;

        defmt::debug!("data: {:?}", data);

        Ok(Measurement { raw_data: data })
    }
}
