use embedded_hal::PwmPin;

use crate::modules::scd4x::Measurement;

const GALVO_TICK_PERIOD_MS: u64 = 20;
const GALVO_TUNING_P: f32 = 25.0;
const GALVO_TUNING_I: f32 = 7.0;

pub const GALVO_CALIBRATION: (f32, f32, f32) = (0.94, 0.79, 0.84);

pub struct Galvos<PWM0, PWM1, PWM2> {
    pwm: ((PWM0, bool, f32), (PWM1, bool, f32), (PWM2, bool, f32)),
    position: (f32, f32, f32),
    speed: (f32, f32, f32),
    desired_position: (f32, f32, f32),
    tick_id: u64,
    t_millis: u64,
}

impl<PWM0, PWM1, PWM2> Galvos<PWM0, PWM1, PWM2>
where
    PWM0: PwmPin<Duty = u16>,
    PWM1: PwmPin<Duty = u16>,
    PWM2: PwmPin<Duty = u16>,
{
    pub fn new(pwm: ((PWM0, bool, f32), (PWM1, bool, f32), (PWM2, bool, f32))) -> Self {
        let mut s = Self {
            pwm,
            position: (0.0, 0.0, 0.0),
            speed: (0.0, 0.0, 0.0),
            desired_position: (1.0, 1.0, 1.0),
            tick_id: 0,
            t_millis: 0,
        };

        s.update_pwm();

        (s.pwm.0).0.enable();
        (s.pwm.1).0.enable();
        (s.pwm.2).0.enable();

        s
    }

    fn perform_control_loop_update(
        delta_millis: u64,
        position: &mut f32,
        speed: &mut f32,
        desired_position: f32,
    ) {
        let delta_secs: f32 = delta_millis as f32 / 1000.0;

        let acceleration =
            GALVO_TUNING_P * (desired_position - *position) - GALVO_TUNING_I * *speed;

        *speed += acceleration * delta_secs;
        *position += *speed * delta_secs;
        *position = position.clamp(0.0, 1.0);
    }

    /// Performs an update tick on the galvos
    ///
    /// # Return
    ///
    /// After how much time the next tick should be performed, in millis
    ///
    pub fn tick(&mut self) -> u64 {
        let delta = GALVO_TICK_PERIOD_MS;
        self.t_millis += delta;

        let desired_position = if self.t_millis < 500 {
            (1.0, 0.0, 0.0)
        } else if self.t_millis < 1000 {
            (1.0, 1.0, 0.0)
        } else if self.t_millis < 3000 {
            (1.0, 1.0, 1.0)
        } else {
            self.desired_position
        };

        Self::perform_control_loop_update(
            delta,
            &mut self.position.0,
            &mut self.speed.0,
            desired_position.0,
        );
        Self::perform_control_loop_update(
            delta,
            &mut self.position.1,
            &mut self.speed.1,
            desired_position.1,
        );
        Self::perform_control_loop_update(
            delta,
            &mut self.position.2,
            &mut self.speed.2,
            desired_position.2,
        );

        self.update_pwm();

        self.tick_id += 1;
        GALVO_TICK_PERIOD_MS
    }

    fn update_pwm(&mut self) {
        fn update(
            (pwm, inverted, calibration): &mut (impl PwmPin<Duty = u16>, bool, f32),
            position: f32,
        ) {
            let max_duty = pwm.get_max_duty();
            let desired_duty = max_duty as f32 * position * *calibration;
            let desired_pwm_value = (desired_duty as u16).clamp(0, max_duty);
            pwm.set_duty(if *inverted {
                max_duty - desired_pwm_value
            } else {
                desired_pwm_value
            });
        }

        update(&mut self.pwm.0, self.position.0);
        update(&mut self.pwm.1, self.position.1);
        update(&mut self.pwm.2, self.position.2);
    }

    pub fn update_values(&mut self, values: &Option<Measurement>) {
        const TEMP_MIN: f32 = 15.0;
        const TEMP_MAX: f32 = 30.0;
        const CO2_MIN: f32 = 500.0;
        const CO2_MAX: f32 = 2000.0;
        const HUMID_MIN: f32 = 30.0;
        const HUMID_MAX: f32 = 70.0;

        let lerp_clamp = |value: f32, min: f32, max: f32| -> f32 {
            ((value - min) / (max - min)).clamp(0.0, 1.0)
        };

        self.desired_position = match values {
            None => (1.0, 1.0, 1.0),
            Some(measurements) => (
                lerp_clamp(measurements.get_temperature(), TEMP_MIN, TEMP_MAX),
                lerp_clamp(measurements.get_co2() as f32, CO2_MIN, CO2_MAX),
                lerp_clamp(measurements.get_humidity(), HUMID_MIN, HUMID_MAX),
            ),
        };
    }
}
