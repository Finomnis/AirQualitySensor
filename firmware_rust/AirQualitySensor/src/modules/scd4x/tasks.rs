use super::SCD4xError;

use crate::modules::i2c::I2cConnection;
pub struct SCD4xTasks {}

use super::{controller::SCD4xController, Measurement};

impl SCD4xTasks {
    pub fn try_get_sensor_data(
        controller: &mut SCD4xController<impl I2cConnection>,
    ) -> Result<Measurement, SCD4xError> {
        match controller.read_measurement() {
            Ok(a) => Ok(a),
            Err(e) => {
                controller.start_periodic_measurement()?;
                Err(e)
            }
        }
    }
}
