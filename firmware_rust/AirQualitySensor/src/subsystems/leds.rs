use crate::hal::prelude::OutputPin;

use crate::modules::scd4x::Measurement;

const LED_BLINK_HALF_INTERVAL_MS: u64 = 600;

const CO2_LEDS_LEVEL_EXCELLENT: u16 = 600;
const CO2_LEDS_LEVEL_WARN_WEAK: u16 = 1125;
const CO2_LEDS_LEVEL_WARN_STRONG: u16 = 1875;
const CO2_LEDS_LEVEL_WARN_CRITICAL: u16 = 2500;

pub struct LEDs<PinR, PinY, PinG> {
    blink_value: bool,
    led_red: PinR,
    led_yellow: PinY,
    led_green: PinG,
}

impl<PinR, PinY, PinG> LEDs<PinR, PinY, PinG>
where
    PinR: OutputPin,
    PinY: OutputPin,
    PinG: OutputPin,
{
    pub fn new(led_red: PinR, led_yellow: PinY, led_green: PinG) -> Self {
        Self {
            blink_value: false,
            led_red,
            led_yellow,
            led_green,
        }
    }

    /// Performs an update tick
    ///
    /// # Return
    ///
    /// After how much time the next tick should be performed, in millis
    ///
    pub fn tick(&mut self, value: &Option<Measurement>) -> u64 {
        self.blink_value = !self.blink_value;

        self.update_leds(value);

        LED_BLINK_HALF_INTERVAL_MS
    }

    pub fn update_leds(&mut self, value: &Option<Measurement>) {
        let (red, yellow, green) = {
            match value {
                Some(measurement) => match measurement.get_co2() {
                    co2 if co2 > CO2_LEDS_LEVEL_WARN_CRITICAL => (self.blink_value, false, false),
                    co2 if co2 > CO2_LEDS_LEVEL_WARN_STRONG => (true, false, false),
                    co2 if co2 > CO2_LEDS_LEVEL_WARN_WEAK => (false, true, false),
                    co2 if co2 < CO2_LEDS_LEVEL_EXCELLENT => (false, false, self.blink_value),
                    _ => (false, false, true),
                },
                None => (self.blink_value, self.blink_value, self.blink_value),
            }
        };

        match red {
            true => self.led_red.set_high().ok(),
            false => self.led_red.set_low().ok(),
        };
        match yellow {
            true => self.led_yellow.set_high().ok(),
            false => self.led_yellow.set_low().ok(),
        };
        match green {
            true => self.led_green.set_high().ok(),
            false => self.led_green.set_low().ok(),
        };
    }
}
