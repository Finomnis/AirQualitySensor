use crate::modules::{
    i2c::I2cConnection,
    scd4x::{Measurement, SCD4xController, SCD4xError},
};

pub struct SensorError {
    msg: &'static str,
    nested: SCD4xError,
}

trait SensorErrorSource<T> {
    fn error_message(self, msg: &'static str) -> Result<T, SensorError>;
}

impl<T> SensorErrorSource<T> for Result<T, SCD4xError> {
    fn error_message(self, msg: &'static str) -> Result<T, SensorError> {
        self.map_err(|e| SensorError { msg, nested: e })
    }
}

pub struct Sensor {
    value: Option<Measurement>,
    needs_init: bool,
}

impl Sensor {
    pub fn new() -> Self {
        Self {
            value: None,
            needs_init: true,
        }
    }

    pub fn get_value(&self) -> &Option<Measurement> {
        &self.value
    }

    /// Performs an update tick on the sensor
    ///
    /// # Return
    ///
    /// After how much time the next tick should be performed, in millis
    ///
    pub fn tick(&mut self, i2c: &mut impl I2cConnection) -> (u64, bool) {
        let mut controller = SCD4xController::new(i2c);

        match self.update(&mut controller) {
            Ok(Some(value)) => {
                self.value = Some(value);
                defmt::info!("New value: {}", self.value);
                (100, true)
            }
            Ok(None) => (100, false),
            Err(e) => {
                defmt::warn!(
                    "Error while trying to read new sensor value: {}: {}",
                    e.msg,
                    e.nested
                );
                self.value = None;

                defmt::info!("Stopping periodic measurement ...");
                controller.stop_periodic_measurement().ok();
                self.needs_init = true;

                (1000, true)
            }
        }
    }

    pub fn update(
        &mut self,
        controller: &mut SCD4xController<impl I2cConnection>,
    ) -> Result<Option<Measurement>, SensorError> {
        if self.needs_init {
            defmt::info!("Starting periodic measurement ...");
            controller
                .start_periodic_measurement()
                .error_message("Unable to start periodic measurements")?;
            self.needs_init = false;
        }

        let data_ready = controller
            .get_data_ready_status()
            .error_message("Unable to ready data ready state")?;

        let result = if data_ready {
            defmt::debug!("Reading sensor value ...");
            let measurement = controller
                .read_measurement()
                .error_message("Reading measurement failed")?;

            Some(measurement)
        } else {
            None
        };

        Ok(result)
    }
}
