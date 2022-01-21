#![no_main]
#![no_std]

use clock_frequency_measurement as _; // global logger + panicking-behavior + memory layout
use clock_frequency_measurement::hal;
use hal::rcc::{Enable, Reset};
use hal::stm32;

fn configure_core_timer(timer_core: stm32::TIM14, rcc: &mut hal::rcc::Rcc) -> stm32::TIM14 {
    stm32::TIM14::enable(rcc);
    stm32::TIM14::reset(rcc);
    timer_core.cr1.modify(|_, w| w.cen().clear_bit());
    timer_core.cnt.reset();
    let psc = 999; // Prescaler of 1000, exactly
    let arr = 0xffff; // Count to max
    timer_core.psc.write(|w| unsafe { w.psc().bits(psc) });
    timer_core.arr.write(|w| unsafe { w.bits(arr) });
    timer_core
        .cr1
        .modify(|_, w| w.cen().set_bit().urs().set_bit());

    timer_core
}

fn configure_ds3231<I2C>(i2c: &mut I2C)
where
    I2C: embedded_hal::blocking::i2c::Write + embedded_hal::blocking::i2c::WriteRead,
{
    defmt::info!("Enabling 1Hz output of DS3231 clock chip ...");
    if let Err(_) = i2c.write(0x68, &[0x0e, 0b00011000]) {
        panic!("Unable to enable 1Hz output of DS3231!");
    }

    let mut buf = [0u8; 0x13];
    if let Err(_) = i2c.write_read(0x68, &[0], &mut buf) {
        panic!("Unable to read from DS3231!");
    };

    // for (addr, val) in buf.iter().enumerate() {
    //     defmt::println!("{:02x}h: {:08b}", addr, val);
    // }

    if (buf[0x0e] & 0b00000100) != 0 {
        panic!("DS3231 1Hz output was not enabled successfully!");
    }

    defmt::info!(
        "DS3231 Temperature: {}.{:02} C",
        buf[0x11],
        (buf[0x12] >> 6) * 25
    );
}

#[rtic::app(device = clock_frequency_measurement::hal::stm32, peripherals = true)]
mod app {
    use super::hal;
    use hal::exti::Event;
    use hal::gpio::gpiob::PB0;
    use hal::gpio::{GpioExt, Output, PushPull, SignalEdge};
    use hal::prelude::*;
    use hal::rcc::RccExt;
    use hal::stm32;
    use hal::timer::{Timer, TimerExt};

    #[local]
    struct LocalResources {
        core_time_prev_actual: u16,
        core_time_prev_expected: u16,
    }

    #[shared]
    struct SharedResources {
        timer_1hz: Timer<stm32::TIM16>,
        led: PB0<Output<PushPull>>,
        timer_core: stm32::TIM14,
        exti: stm32::EXTI,
        measured_expected: Option<u16>,
    }

    #[init]
    fn init(ctx: init::Context) -> (SharedResources, LocalResources, init::Monotonics) {
        // enable dma clock during sleep, otherwise defmt doesn't work
        ctx.device.RCC.ahbenr.modify(|_, w| w.dmaen().set_bit());

        defmt::println!("Measuring clock frequency ...");

        // Initialize GPIOs
        let mut exti = ctx.device.EXTI;
        let mut rcc = ctx.device.RCC.constrain();
        let gpioa = ctx.device.GPIOA.split(&mut rcc);
        let gpiob = ctx.device.GPIOB.split(&mut rcc);
        let led = gpiob.pb0.into_push_pull_output();
        let i2c_sda = gpioa.pa12.into_open_drain_output();
        let i2c_scl = gpioa.pa11.into_open_drain_output();
        let ds3231_int = gpioa.pa0.into_pull_up_input();

        // Initialize timers
        let mut timer_1hz = ctx.device.TIM16.timer(&mut rcc);
        timer_1hz.start(1.hz());
        timer_1hz.listen();

        // Configure TIM14 to run with clock/1000
        let timer_core = super::configure_core_timer(ctx.device.TIM14, &mut rcc);

        // Initialize I2C
        let mut i2c =
            ctx.device
                .I2C2
                .i2c(i2c_sda, i2c_scl, hal::i2c::Config::new(100.khz()), &mut rcc);

        // Initialize DS3231
        super::configure_ds3231(&mut i2c);

        // Configure DS3231 1Hz pin as external trigger
        ds3231_int.listen(SignalEdge::Falling, &mut exti);

        (
            SharedResources {
                timer_core,
                timer_1hz,
                led,
                exti,
                measured_expected: None,
            },
            LocalResources {
                core_time_prev_actual: 0,
                core_time_prev_expected: 0,
            },
            init::Monotonics(),
        )
    }

    #[task(binds = TIM16, shared = [timer_1hz, timer_core, measured_expected], local = [core_time_prev_expected])]
    fn measure_expected(mut ctx: measure_expected::Context) {
        ctx.shared.timer_1hz.lock(|timer| timer.clear_irq());
        // ctx.shared.led.lock(|led| led.toggle().unwrap());

        let core_time = ctx
            .shared
            .timer_core
            .lock(|timer| timer.cnt.read().cnt().bits());

        let (core_time_diff, _) = core_time.overflowing_sub(*ctx.local.core_time_prev_expected);
        *ctx.local.core_time_prev_expected = core_time;

        ctx.shared
            .measured_expected
            .lock(|e| *e = Some(core_time_diff));

        defmt::debug!(
            "Expected: {}.{:03} MHz",
            core_time_diff / 1000,
            core_time_diff % 1000
        );
    }

    #[task(binds = EXTI0_1, shared = [timer_1hz, led, timer_core, exti, measured_expected], local = [core_time_prev_actual])]
    fn measure_actual(mut ctx: measure_actual::Context) {
        ctx.shared.exti.lock(|exti| exti.unpend(Event::GPIO0));
        ctx.shared.led.lock(|led| led.toggle().unwrap());

        let core_time = ctx
            .shared
            .timer_core
            .lock(|timer| timer.cnt.read().cnt().bits());

        let core_time_diff = core_time
            .overflowing_sub(*ctx.local.core_time_prev_actual)
            .0;
        *ctx.local.core_time_prev_actual = core_time;

        let expected = ctx.shared.measured_expected.lock(|e| e.clone());

        match expected {
            Some(expected) => {
                let diff = ((expected - core_time_diff) as i16).abs() as u32;
                let diff_ppm = diff * 1000000 / expected as u32;
                defmt::println!(
                    "{}.{:03} MHz; expected: {}.{:03} MHz; precision: {} ppm",
                    core_time_diff / 1000,
                    core_time_diff % 1000,
                    expected / 1000,
                    expected % 1000,
                    diff_ppm
                );
            }
            None => {
                defmt::println!("{}.{:03} MHz", core_time_diff / 1000, core_time_diff % 1000);
            }
        }

        ctx.shared
            .timer_1hz
            .lock(|t| defmt::info!("Drift Indicator: {}", t.get_current()));
    }
}
