const I2C_ADDR: u8 = 0x62;

use super::{Measurement, SCD4xError};

pub struct SCD4xController<'a, I2C> {
    i2c: &'a mut I2C,
}

mod scd4x_calls {
    use super::super::data::crc;
    use super::SCD4xError;
    use crate::modules::i2c::I2cConnection;

    const I2C_ADDR: u8 = 0x62;

    macro_rules! SCD4X_CALL {
        (@impl $name: ident, $code: literal, $arguments: literal, $responses: literal) => {
            fn implementation<I2C: I2cConnection>(
                i2c: &mut I2C,
                args: &[u16; $arguments],
            ) -> Result<[u16; $responses], SCD4xError> {
                let code_bytes: [u8; 2] = ($code as u16).to_be_bytes();

                let mut call_data = [0u8; $arguments * 3 + 2];
                call_data[0] = code_bytes[0];
                call_data[1] = code_bytes[1];

                for (pos, arg) in args.iter().enumerate() {
                    let arg_bytes = arg.to_be_bytes();
                    call_data[3 * pos + 2] = arg_bytes[0];
                    call_data[3 * pos + 3] = arg_bytes[1];
                    call_data[3 * pos + 4] = crc(&arg_bytes);
                }

                let mut response_data = [0u8; $responses * 3];

                defmt::debug!("Sending data: {:?}", call_data);
                if ($responses > 0) {
                    i2c.write_read(I2C_ADDR, &call_data, &mut response_data)
                        .map_err(SCD4xError::from_i2c_error)?;
                    defmt::debug!("Received response data: {:?}", response_data);
                } else {
                    i2c.write(I2C_ADDR, &call_data)
                        .map_err(SCD4xError::from_i2c_error)?;
                }

                let mut responses = [0u16; $responses];

                for (pos, response) in
                    super::super::data::response_to_array::<$responses>(&response_data)?
                        .iter()
                        .enumerate()
                {
                    responses[pos] = *response;
                }

                Ok(responses)
            }
        };
        ($name: ident, $code: literal, 0, 0) => {
            pub fn $name<I2C: I2cConnection>(
                i2c: &mut I2C,
            ) -> Result<(), SCD4xError> {
                SCD4X_CALL!(@impl $name, $code, 0, 0);
                implementation(i2c, &[]).map(|_: [u16; 0]| ())
            }
        };
        ($name: ident, $code: literal, 0, $responses: literal) => {
            pub fn $name<I2C: I2cConnection>(
                i2c: &mut I2C,
            ) -> Result<[u16; $responses], SCD4xError> {
                SCD4X_CALL!(@impl $name, $code, 0, $responses);
                implementation(i2c, &[])
            }
        };
        ($name: ident, $code: literal, $arguments: literal, 0) => {
            pub fn $name<I2C: I2cConnection>(
                i2c: &mut I2C,
                args: &[u16; $arguments],
            ) -> Result<(), SCD4xError> {
                SCD4X_CALL!(@impl $name, $code, $arguments, 0);
                implementation(i2c, args).map(|_: [u16; 0]| ())
            }
        };
        ($name: ident, $code: literal, $arguments: literal, $responses: literal) => {
            pub fn $name<I2C: I2cConnection>(
                i2c: &mut I2C,
                args: &[u16; $arguments],
            ) -> Result<[u16; $responses], SCD4xError> {
                SCD4X_CALL!(@impl $name, $code, $arguments, $responses);
                implementation(i2c, args)
            }
        };
    }

    // Basic Commands
    SCD4X_CALL!(start_periodic_measurement, 0x21b1, 0, 0);
    SCD4X_CALL!(read_measurement, 0xec05, 0, 3);
    SCD4X_CALL!(stop_periodic_measurement, 0x3f86, 0, 0);

    // On-Chip Output Signal Compensation
    SCD4X_CALL!(set_temperature_offset, 0x241d, 1, 0);
    SCD4X_CALL!(get_temperature_offset, 0x2318, 0, 1);
    SCD4X_CALL!(set_sensor_altitude, 0x2427, 1, 0);
    SCD4X_CALL!(get_sensor_altitude, 0x2322, 0, 1);

    // Low Power operation
    SCD4X_CALL!(get_data_ready_status, 0xe4b8, 0, 1);

    // Advanced Features
    SCD4X_CALL!(get_serial_number, 0x3682, 0, 3);
    SCD4X_CALL!(perform_self_test, 0x3639, 0, 0);
}

impl<'a, I2C> SCD4xController<'a, I2C>
where
    I2C: crate::modules::i2c::I2cConnection,
{
    pub fn new(i2c: &'a mut I2C) -> Self {
        Self { i2c }
    }

    pub fn get_serial_number_old(&mut self) -> Result<u64, SCD4xError> {
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

    pub fn get_serial_number(&mut self) -> Result<u64, SCD4xError> {
        scd4x_calls::get_serial_number(self.i2c).map(|data| {
            let mut serial = 0;
            for num in data {
                serial = serial << 16 | num as u64;
            }
            serial
        })
    }

    pub fn start_self_test(&mut self) -> Result<(), SCD4xError> {
        scd4x_calls::perform_self_test(self.i2c)
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
        scd4x_calls::start_periodic_measurement(self.i2c)
    }

    pub fn stop_periodic_measurement(&mut self) -> Result<(), SCD4xError> {
        scd4x_calls::stop_periodic_measurement(self.i2c)
    }

    pub fn get_data_ready_status(&mut self) -> Result<bool, SCD4xError> {
        scd4x_calls::get_data_ready_status(self.i2c).map(|data| {
            let ready = data[0] & 0x07ff != 0;
            ready
        })
    }

    pub fn read_measurement(&mut self) -> Result<Measurement, SCD4xError> {
        scd4x_calls::read_measurement(self.i2c).map(|data| {
            defmt::debug!("data: {:?}", data);
            Measurement { raw_data: data }
        })
    }

    pub fn get_temperature_offset(&mut self) -> Result<f32, SCD4xError> {
        scd4x_calls::get_temperature_offset(self.i2c).map(|data| {
            let temperature = (data[0] as f32 * 175.0) / ((1 << 16) as f32);
            temperature
        })
    }

    pub fn set_temperature_offset(&mut self, temperature: f32) -> Result<(), SCD4xError> {
        let temperature_data = (temperature * ((1 << 16) as f32) / 175.0) as u16;
        scd4x_calls::set_temperature_offset(self.i2c, &[temperature_data])
    }

    pub fn get_sensor_altitude(&mut self) -> Result<u16, SCD4xError> {
        scd4x_calls::get_sensor_altitude(self.i2c).map(|data| data[0])
    }

    pub fn set_sensor_altitude(&mut self, altitude: u16) -> Result<(), SCD4xError> {
        scd4x_calls::set_sensor_altitude(self.i2c, &[altitude])
    }
}
