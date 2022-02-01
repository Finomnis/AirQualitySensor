use embedded_hal::PwmPin;

const GALVO_TICK_PERIOD_MS: u64 = 10;

pub const GALVO_CALIBRATION: (f32, f32, f32) = (0.94, 0.80, 0.84);

pub struct Galvos<PWM0, PWM1, PWM2> {
    pwm: ((PWM0, bool, f32), (PWM1, bool, f32), (PWM2, bool, f32)),
    position: (f32, f32, f32),
    speed: (f32, f32, f32),
    desired_position: (f32, f32, f32),
    tick_id: u64,
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
        };

        s.update_pwm();

        (s.pwm.0).0.enable();
        (s.pwm.1).0.enable();
        (s.pwm.2).0.enable();

        s
    }

    const P: f32 = 0.0015;
    const I: f32 = 0.05;
    fn perform_control_loop_update(position: &mut f32, speed: &mut f32, desired_position: f32) {
        let acceleration = Self::P * (desired_position - *position) - Self::I * *speed;
        *speed += acceleration;
        *position += *speed;
        *position = position.clamp(0.0, 1.0);
    }

    /// Performs an update tick on the galvos
    ///
    /// # Return
    ///
    /// After how much time the next tick should be performed, in millis
    ///
    pub fn tick(&mut self) -> u64 {
        self.tick_id += 1;

        Self::perform_control_loop_update(
            &mut self.position.0,
            &mut self.speed.0,
            self.desired_position.0,
        );
        Self::perform_control_loop_update(
            &mut self.position.1,
            &mut self.speed.1,
            self.desired_position.1,
        );
        Self::perform_control_loop_update(
            &mut self.position.2,
            &mut self.speed.2,
            self.desired_position.2,
        );

        self.update_pwm();
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
}
