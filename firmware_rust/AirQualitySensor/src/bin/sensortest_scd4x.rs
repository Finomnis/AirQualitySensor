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

    use firmware::modules::scd4x::SCD4xController;

    #[monotonic(binds = SysTick, default = true)]
    type SystickMono = Systick<1000>; // 1000 Hz / 1 ms granularity

    #[local]
    struct LocalResources {
        next_sensor_tick: <SystickMono as rtic::Monotonic>::Instant,
    }

    #[shared]
    struct SharedResources {
        self_test_finished: bool,
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

        defmt::println!("SCD4x sensor test");

        // Initialize GPIOs
        let mut rcc = ctx.device.RCC.constrain();
        let gpioa = ctx.device.GPIOA.split(&mut rcc);
        let i2c_sda = gpioa.pa12.into_open_drain_output();
        let i2c_scl = gpioa.pa11.into_open_drain_output();

        // Initialize I2C
        let mut i2c =
            ctx.device
                .I2C2
                .i2c(i2c_sda, i2c_scl, hal::i2c::Config::new(100.khz()), &mut rcc);

        // Check sensor
        let mut controller = SCD4xController::new(&mut i2c);
        match controller.get_serial_number() {
            Ok(serial) => defmt::info!("Sensor serial: {}", serial),
            Err(e) => defmt::warn!("Unable to query serial number: {:?}", e),
        };
        match controller.start_self_test() {
            Ok(()) => defmt::info!("Self test started ..."),
            Err(e) => defmt::warn!("Unable to start self test: {:?}", e),
        };
        finish_self_test::spawn_at(monotonics::now() + 15.secs()).unwrap();

        // Schedule PWM updates
        let next_sensor_tick = monotonics::now() + 10.millis();
        sensor_tick::spawn_at(next_sensor_tick).unwrap();

        (
            SharedResources {
                i2c,
                self_test_finished: false,
            },
            LocalResources { next_sensor_tick },
            init::Monotonics(Systick::new(ctx.core.SYST, rcc.clocks.sys_clk.0)),
        )
    }

    #[task(shared = [i2c, self_test_finished])]
    fn finish_self_test(mut ctx: finish_self_test::Context) {
        ctx.shared.i2c.lock(|i2c| {
            let mut controller = SCD4xController::new(i2c);
            match controller.finish_self_test() {
                Ok(()) => defmt::info!("Self test succeeded."),
                Err(e) => defmt::warn!("Self test: {:?}", e),
            }
        });
        ctx.shared.self_test_finished.lock(|x| *x = true);
    }

    #[task(local = [next_sensor_tick], shared = [i2c, self_test_finished])]
    fn sensor_tick(ctx: sensor_tick::Context) {
        let sensor_tick::LocalResources { next_sensor_tick } = ctx.local;
        let sensor_tick::SharedResources {
            i2c: _,
            mut self_test_finished,
        } = ctx.shared;

        if !self_test_finished.lock(|x| *x) {
            return;
        }

        //i2c.lock(|i2c| {});

        *next_sensor_tick = *next_sensor_tick + 5.secs();
        sensor_tick::spawn_at(*next_sensor_tick).unwrap();
    }
}
