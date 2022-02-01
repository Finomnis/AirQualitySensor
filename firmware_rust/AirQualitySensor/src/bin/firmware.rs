#![no_main]
#![no_std]

use firmware as _; // global logger + panicking-behavior + memory layout
use firmware::hal;
//use hal::stm32;

#[rtic::app(device = firmware::hal::stm32, peripherals = true, dispatchers = [USART1, USART2])]
mod app {
    use super::hal;
    use hal::gpio;
    use hal::prelude::*;
    use hal::stm32;
    use hal::timer;
    use systick_monotonic::*;

    use firmware::subsystems::{Galvos, LEDs, Sensor, GALVO_CALIBRATION};

    #[monotonic(binds = SysTick, default = true)]
    type SystickMono = Systick<1000>; // 1000 Hz / 1 ms granularity

    #[local]
    struct LocalResources {
        next_sensor_tick: <SystickMono as rtic::Monotonic>::Instant,
        next_led_tick: <SystickMono as rtic::Monotonic>::Instant,
        next_galvo_tick: <SystickMono as rtic::Monotonic>::Instant,
        watchdog: hal::watchdog::IndependedWatchdog,
    }

    #[shared]
    struct SharedResources {
        sensor_subsystem: Sensor,
        galvo_subsystem: Galvos<
            timer::pwm::PwmPin<stm32::TIM16, timer::Channel1>,
            timer::pwm::PwmPin<stm32::TIM14, timer::Channel1>,
            timer::pwm::PwmPin<stm32::TIM1, timer::Channel1>,
        >,
        led_subsystem: LEDs<
            gpio::gpioa::PA0<gpio::Output<gpio::PushPull>>,
            gpio::gpioa::PA1<gpio::Output<gpio::PushPull>>,
            gpio::gpioa::PA2<gpio::Output<gpio::PushPull>>,
        >,
        i2c: hal::i2c::I2c<
            stm32::I2C2,
            gpio::gpioa::PA12<gpio::Output<gpio::OpenDrain>>,
            gpio::gpioa::PA11<gpio::Output<gpio::OpenDrain>>,
        >,
    }

    #[init]
    fn init(ctx: init::Context) -> (SharedResources, LocalResources, init::Monotonics) {
        // enable dma clock during sleep, otherwise defmt doesn't work
        ctx.device.RCC.ahbenr.modify(|_, w| w.dmaen().set_bit());

        defmt::println!("Finomnis' AirQualitySensor - Galvo Version");

        // Initialize GPIOs
        let mut rcc = ctx.device.RCC.constrain();
        let gpioa = ctx.device.GPIOA.split(&mut rcc);
        let i2c_sda = gpioa.pa12.into_open_drain_output();
        let i2c_scl = gpioa.pa11.into_open_drain_output();
        let delay = ctx.core.SYST.delay(&mut rcc);

        // Initialize I2C
        let i2c = ctx
            .device
            .I2C2
            .i2c(i2c_sda, i2c_scl, hal::i2c::Config::new(100.khz()), &mut rcc);

        let now = monotonics::now();

        // Initialize & schedule sensor subsystem
        let sensor_subsystem = Sensor::new();
        let next_sensor_tick = now;
        sensor_tick::spawn_at(next_sensor_tick).unwrap();

        // Initialize & schedule leds subsystem
        let led_subsystem = LEDs::new(
            gpioa.pa0.into_push_pull_output(),
            gpioa.pa1.into_push_pull_output(),
            gpioa.pa2.into_push_pull_output(),
        );
        let next_led_tick = now + 1.millis();
        led_tick::spawn_at(next_led_tick).unwrap();

        // Initialize & schedule galvo subsystem
        let pwm1 = ctx.device.TIM16.pwm(1.khz(), &mut rcc).bind_pin(gpioa.pa6);
        let pwm2 = ctx.device.TIM14.pwm(1.khz(), &mut rcc).bind_pin(gpioa.pa4);
        let pwm3 = ctx.device.TIM1.pwm(1.khz(), &mut rcc).bind_pin(gpioa.pa7);
        let galvo_subsystem = Galvos::new((
            (pwm1, false, GALVO_CALIBRATION.0),
            (pwm2, false, GALVO_CALIBRATION.1),
            (pwm3, true, GALVO_CALIBRATION.2),
        ));
        let next_galvo_tick = now + 2.millis();
        galvo_tick::spawn_at(next_galvo_tick).unwrap();

        // Initialize watchdog
        let mut watchdog = ctx.device.IWDG.constrain();
        watchdog.start(2000.ms());
        watchdog_wagger::spawn_at(monotonics::now()).unwrap();

        (
            SharedResources {
                i2c,
                sensor_subsystem,
                led_subsystem,
                galvo_subsystem,
            },
            LocalResources {
                next_sensor_tick,
                next_led_tick,
                next_galvo_tick,
                watchdog,
            },
            init::Monotonics(Systick::new(delay.release(), rcc.clocks.sys_clk.0)),
        )
    }

    #[task(shared = [i2c, sensor_subsystem, led_subsystem, galvo_subsystem], local = [next_sensor_tick])]
    fn sensor_tick(mut ctx: sensor_tick::Context) {
        let next_sensor_tick = ctx.local.next_sensor_tick;

        let delay = ctx.shared.i2c.lock(|i2c| {
            ctx.shared.sensor_subsystem.lock(|sensor_subsystem| {
                let (delay, changed) = sensor_subsystem.tick(i2c);
                if changed {
                    ctx.shared.led_subsystem.lock(|led_subsystem| {
                        led_subsystem.update_leds(sensor_subsystem.get_value())
                    });
                    ctx.shared.galvo_subsystem.lock(|galvo_subsystem| {
                        galvo_subsystem.update_values(sensor_subsystem.get_value())
                    });
                }
                delay
            })
        });

        *next_sensor_tick = *next_sensor_tick + delay.millis();
        sensor_tick::spawn_at(*next_sensor_tick).unwrap();
    }

    #[task(shared = [sensor_subsystem, led_subsystem], local = [next_led_tick])]
    fn led_tick(mut ctx: led_tick::Context) {
        let next_led_tick = ctx.local.next_led_tick;

        let sensor_value = ctx.shared.sensor_subsystem.lock(|s| s.get_value().clone());

        let delay = ctx.shared.led_subsystem.lock(|s| s.tick(&sensor_value));

        *next_led_tick = *next_led_tick + delay.millis();
        led_tick::spawn_at(*next_led_tick).unwrap();
    }

    #[task(shared = [galvo_subsystem], local = [next_galvo_tick])]
    fn galvo_tick(mut ctx: galvo_tick::Context) {
        let next_galvo_tick = ctx.local.next_galvo_tick;

        let delay = ctx.shared.galvo_subsystem.lock(|s| s.tick());

        *next_galvo_tick = *next_galvo_tick + delay.millis();
        galvo_tick::spawn_at(*next_galvo_tick).unwrap();
    }

    #[task(local = [watchdog])]
    fn watchdog_wagger(ctx: watchdog_wagger::Context) {
        ctx.local.watchdog.feed();
        watchdog_wagger::spawn_at(monotonics::now() + 100.millis()).unwrap();
    }
}
