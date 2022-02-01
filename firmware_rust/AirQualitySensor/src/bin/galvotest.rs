#![no_main]
#![no_std]

use firmware as _; // global logger + panicking-behavior + memory layout
use firmware::hal;
//use hal::stm32;

#[rtic::app(device = firmware::hal::stm32, peripherals = true, dispatchers = [USART1, USART2])]
mod app {
    use super::hal;
    use hal::prelude::*;
    use hal::stm32;
    use hal::timer::pwm::PwmPin;
    use hal::timer::Channel1;
    use systick_monotonic::*;

    #[monotonic(binds = SysTick, default = true)]
    type SystickMono = Systick<1000>; // 1000 Hz / 1 ms granularity

    #[local]
    struct LocalResources {
        pwm_time: u32,
        pwm_co2: PwmPin<stm32::TIM14, Channel1>,
        pwm_temp: PwmPin<stm32::TIM16, Channel1>,
        pwm_humid: PwmPin<stm32::TIM1, Channel1>,
        next_pwm_update: <SystickMono as rtic::Monotonic>::Instant,
    }

    #[shared]
    struct SharedResources {}

    #[init]
    fn init(ctx: init::Context) -> (SharedResources, LocalResources, init::Monotonics) {
        // enable dma clock during sleep, otherwise defmt doesn't work
        ctx.device.RCC.ahbenr.modify(|_, w| w.dmaen().set_bit());

        // Initialize GPIOs
        let mut rcc = ctx.device.RCC.constrain();
        let gpioa = ctx.device.GPIOA.split(&mut rcc);

        let pwm1 = ctx.device.TIM14.pwm(1.khz(), &mut rcc);
        let pwm2 = ctx.device.TIM16.pwm(1.khz(), &mut rcc);
        let pwm3 = ctx.device.TIM1.pwm(1.khz(), &mut rcc);

        //let mut humid = gpioa.pa7.into_push_pull_output();
        //humid.set_high();

        let mut pwm_temp = pwm2.bind_pin(gpioa.pa6);
        pwm_temp.set_duty(0); //pwm_temp.get_max_duty() / 2);
        pwm_temp.enable();
        let mut pwm_co2 = pwm1.bind_pin(gpioa.pa4);
        pwm_co2.set_duty(0); //pwm_co2.get_max_duty() / 4);
        pwm_co2.enable();
        let mut pwm_humid = pwm3.bind_pin(gpioa.pa7);
        pwm_humid.set_duty(pwm_humid.get_max_duty()); //pwm_humid.get_max_duty() / 4);
        pwm_humid.enable();

        // Schedule PWM updates
        let next_pwm_update = monotonics::now() + 10.millis();
        update_pwm::spawn_at(next_pwm_update).unwrap();

        (
            SharedResources {},
            LocalResources {
                pwm_time: 0,
                pwm_temp,
                pwm_co2,
                pwm_humid,
                next_pwm_update,
            },
            init::Monotonics(Systick::new(ctx.core.SYST, rcc.clocks.sys_clk.0)),
        )
    }

    #[task(local = [pwm_time, pwm_co2, pwm_temp, pwm_humid, next_pwm_update])]
    fn update_pwm(ctx: update_pwm::Context) {
        let update_pwm::LocalResources {
            pwm_time,
            pwm_temp,
            pwm_co2,
            pwm_humid,
            next_pwm_update,
        } = ctx.local;

        *pwm_time += 1;
        if *pwm_time > 1200 {
            *pwm_time = 0;
        }

        // Compute max duty for galvos. According to math, this should be 84% duty cycle.
        let max_duty_temp = pwm_temp.get_max_duty() as u64 * 94 / 100;
        let max_duty_co2 = pwm_co2.get_max_duty() as u64 * 80 / 100;
        let max_duty_humid = pwm_humid.get_max_duty() as u64 * 84 / 100;

        let compute_duty = |max_duty| match *pwm_time {
            x if x < 100 => 0,
            x if x < 200 => max_duty * (x as u64 - 100) / 300,
            x if x < 300 => max_duty * 100 / 300,
            x if x < 400 => max_duty * (x as u64 - 200) / 300,
            x if x < 500 => max_duty * 200 / 300,
            x if x < 600 => max_duty * (x as u64 - 300) / 300,
            x if x < 700 => max_duty,
            x if x < 800 => max_duty * (1000 - x as u64) / 300,
            x if x < 900 => max_duty * 200 / 300,
            x if x < 1000 => max_duty * (1100 - x as u64) / 300,
            x if x < 1100 => max_duty * 100 / 300,
            x => max_duty * (1200 - x as u64) / 300,
        } as u16;

        pwm_temp.set_duty(compute_duty(max_duty_temp));
        pwm_co2.set_duty(compute_duty(max_duty_co2));
        pwm_humid.set_duty(pwm_humid.get_max_duty() - compute_duty(max_duty_humid));

        // defmt::info!(
        //     "duty: {} - {} - {}",
        //     duty,
        //     monotonics::now().ticks(),
        //     next_pwm_update.ticks()
        // );

        *next_pwm_update = *next_pwm_update + 10.millis();
        update_pwm::spawn_at(*next_pwm_update).unwrap();
    }
}
