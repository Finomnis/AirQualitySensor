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
        pwm_ch1: PwmPin<stm32::TIM14, Channel1>,
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

        let pwm = ctx.device.TIM14.pwm(1.khz(), &mut rcc);

        let mut pwm_ch1 = pwm.bind_pin(gpioa.pa4);
        pwm_ch1.set_duty(0);
        pwm_ch1.enable();

        // Schedule PWM updates
        let next_pwm_update = monotonics::now() + 10.millis();
        update_pwm::spawn_at(next_pwm_update).unwrap();

        (
            SharedResources {},
            LocalResources {
                pwm_time: 0,
                pwm_ch1,
                next_pwm_update,
            },
            init::Monotonics(Systick::new(ctx.core.SYST, rcc.clocks.sys_clk.0)),
        )
    }

    #[task(local = [pwm_time, pwm_ch1, next_pwm_update])]
    fn update_pwm(ctx: update_pwm::Context) {
        let update_pwm::LocalResources {
            pwm_time,
            pwm_ch1,
            next_pwm_update,
        } = ctx.local;

        *pwm_time += 1;
        if *pwm_time > 1200 {
            *pwm_time = 0;
        }

        // Compute max duty for galvos. According to math, this should be 84% duty cycle.
        let max_duty = pwm_ch1.get_max_duty() as u64 * 86 / 100;

        let duty = match *pwm_time {
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

        pwm_ch1.set_duty(duty);

        defmt::info!(
            "duty: {} - {} - {}",
            duty,
            monotonics::now().ticks(),
            next_pwm_update.ticks()
        );

        *next_pwm_update = *next_pwm_update + 10.millis();
        update_pwm::spawn_at(*next_pwm_update).unwrap();
    }
}
