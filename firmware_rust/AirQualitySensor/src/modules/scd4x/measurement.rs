use super::Measurement;

impl Measurement {
    pub fn get_co2(&self) -> u16 {
        self.raw_data[0]
    }

    pub fn get_temperature(&self) -> f32 {
        -45.0 + (175.0 * self.raw_data[1] as f32) / (1 << 16) as f32
    }

    pub fn get_humidity(&self) -> f32 {
        (100.0 * self.raw_data[2] as f32) / (1 << 16) as f32
    }
}

impl defmt::Format for Measurement {
    fn format(&self, f: defmt::Formatter) {
        // format the bitfields of the register as struct fields

        defmt::write!(
            f,
            "CO2: {}   Temperature: {}   Humidity: {}",
            self.get_co2(),
            self.get_temperature(),
            self.get_humidity()
        )
    }
}
