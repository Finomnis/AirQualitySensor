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
        let mut delay = ctx.core.SYST.delay(&mut rcc);

        // Initialize I2C
        let mut i2c =
            ctx.device
                .I2C2
                .i2c(i2c_sda, i2c_scl, hal::i2c::Config::new(100.khz()), &mut rcc);

        // Get sensor controller
        let mut controller = SCD4xController::new(&mut i2c);

        // Stop sensor periodic measuring. Many methods cannot be called while the sensor is active.
        match controller.stop_periodic_measurement() {
            Ok(()) => defmt::info!("Stopped periodic measurement."),
            Err(e) => defmt::warn!("Unable to stop periodic measurement: {:?}", e),
        }
        delay.delay(500.ms());

        // Query serial number, as sanity check
        match controller.get_serial_number() {
            Ok(serial) => defmt::info!("Sensor serial: {}", serial),
            Err(e) => defmt::warn!("Unable to query serial number: {:?}", e),
        };

        // Perform self test
        // match controller.start_self_test() {
        //     Ok(()) => defmt::info!("Self test started ..."),
        //     Err(e) => defmt::warn!("Unable to start self test: {:?}", e),
        // };
        // delay.delay(10.seconds());
        // match controller.finish_self_test() {
        //     Ok(()) => defmt::info!("Self test succeeded."),
        //     Err(e) => defmt::warn!("Self test: {:?}", e),
        // }

        // Start periodic measurements
        match controller.start_periodic_measurement() {
            Ok(()) => defmt::info!("Periodic measurement started."),
            Err(e) => defmt::warn!("Starting periodic measurement: {:?}", e),
        }

        // Schedule PWM updates
        let next_sensor_tick = monotonics::now();
        sensor_tick::spawn_at(next_sensor_tick).unwrap();

        (
            SharedResources { i2c },
            LocalResources { next_sensor_tick },
            init::Monotonics(Systick::new(delay.release(), rcc.clocks.sys_clk.0)),
        )
    }

    #[task(shared = [i2c], local = [next_sensor_tick])]
    fn sensor_tick(mut ctx: sensor_tick::Context) {
        let next_sensor_tick = ctx.local.next_sensor_tick;

        ctx.shared.i2c.lock(|i2c| {
            let mut controller = SCD4xController::new(i2c);

            let data_ready = match controller.get_data_ready_status() {
                Ok(ready) => ready,
                Err(e) => {
                    defmt::warn!("Unable to ready data ready state: {:?}", e);
                    false
                }
            };

            if data_ready {
                defmt::debug!("Reading sensor value ...");
                match controller.read_measurement() {
                    Ok(()) => defmt::info!("Measurement: {}", 0),
                    Err(e) => defmt::warn!("Reading measurement failed: {:?}", e),
                }
            }
        });

        *next_sensor_tick = *next_sensor_tick + 100.millis();
        sensor_tick::spawn_at(*next_sensor_tick).unwrap();
    }
}
