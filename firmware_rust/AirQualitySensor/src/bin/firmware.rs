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
    use systick_monotonic::*;

    use firmware::subsystems::sensor::Sensor;

    #[monotonic(binds = SysTick, default = true)]
    type SystickMono = Systick<1000>; // 1000 Hz / 1 ms granularity

    #[local]
    struct LocalResources {
        next_sensor_tick: <SystickMono as rtic::Monotonic>::Instant,
        sensor_subsystem: Sensor,
        watchdog: hal::watchdog::IndependedWatchdog,
    }

    #[shared]
    struct SharedResources {
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

        // Initialize & schedule sensor subsystem
        let sensor_subsystem = Sensor::new();
        let next_sensor_tick = monotonics::now();
        sensor_tick::spawn_at(next_sensor_tick).unwrap();

        // Initialize watchdog
        let mut watchdog = ctx.device.IWDG.constrain();
        watchdog.start(2000.ms());
        watchdog_wagger::spawn_at(monotonics::now()).unwrap();

        (
            SharedResources { i2c },
            LocalResources {
                next_sensor_tick,
                sensor_subsystem,
                watchdog,
            },
            init::Monotonics(Systick::new(delay.release(), rcc.clocks.sys_clk.0)),
        )
    }

    #[task(shared = [i2c], local = [next_sensor_tick, sensor_subsystem])]
    fn sensor_tick(mut ctx: sensor_tick::Context) {
        let next_sensor_tick = ctx.local.next_sensor_tick;

        let delay = ctx
            .shared
            .i2c
            .lock(|i2c| ctx.local.sensor_subsystem.tick(i2c));

        *next_sensor_tick = *next_sensor_tick + delay.millis();
        sensor_tick::spawn_at(*next_sensor_tick).unwrap();
    }

    #[task(local = [watchdog])]
    fn watchdog_wagger(ctx: watchdog_wagger::Context) {
        ctx.local.watchdog.feed();
        watchdog_wagger::spawn_at(monotonics::now() + 100.millis()).unwrap();
    }
}
