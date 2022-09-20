#![no_main]
#![no_std]

use defmt_rtt as _;
use dht_sensor::DhtReading;
// global logger
use hal::prelude::*;
use nrf52840_hal as hal; // memory layout
use panic_probe as _;

use display_interface_spi::SPIInterface;
use embedded_graphics::{pixelcolor::BinaryColor, prelude::*};
use embedded_hal::blocking::delay::DelayMs;
use hal::gpio::{Level, OpenDrain, OpenDrainConfig, Output, Pin};
use st7565::{displays::DOGM132W5, ST7565};
use u8g2_fonts::{
    fonts,
    types::{FontColor, VerticalPosition},
    FontRenderer,
};

// same panicking *behavior* as `panic-probe` but doesn't print a panic message
// this prevents the panic message being printed *twice* when `defmt::panic` is invoked
#[defmt::panic_handler]
fn panic() -> ! {
    cortex_m::asm::udf()
}

/// Terminates the application and makes `probe-run` exit with exit-code = 0
pub fn exit() -> ! {
    loop {
        cortex_m::asm::bkpt();
    }
}

struct OpenDrainIOPin {
    pin: Pin<Output<OpenDrain>>,
}
impl OpenDrainIOPin {
    fn new(pin: Pin<Output<OpenDrain>>) -> Self {
        let this = Self { pin };
        let pin_cnf = &this.p().pin_cnf[this.pin.pin() as usize];
        let prev = pin_cnf.read().bits();
        defmt::println!("{:b}", prev);
        this.p().pin_cnf[this.pin.pin() as usize].write(|w| {
            unsafe {
                w.bits(prev);
            }
            w.input().connect();
            w
        });
        defmt::println!("{:b}", pin_cnf.read().bits());
        this
    }

    fn p(&self) -> &'static hal::pac::p0::RegisterBlock {
        match self.pin.port() {
            hal::gpio::Port::Port0 => unsafe { &*hal::pac::P0::ptr() },
            hal::gpio::Port::Port1 => unsafe { &*hal::pac::P1::ptr() },
        }
    }
}

impl OutputPin for OpenDrainIOPin {
    type Error = <Pin<Output<OpenDrain>> as OutputPin>::Error;

    fn set_low(&mut self) -> Result<(), Self::Error> {
        self.pin.set_low()
    }

    fn set_high(&mut self) -> Result<(), Self::Error> {
        self.pin.set_high()
    }
}

impl InputPin for OpenDrainIOPin {
    type Error = <Pin<Output<OpenDrain>> as OutputPin>::Error;

    fn is_high(&self) -> Result<bool, Self::Error> {
        Ok(self.p().in_.read().bits() & (1 << self.pin.pin()) != 0)
    }

    fn is_low(&self) -> Result<bool, Self::Error> {
        self.is_high().map(|h| !h)
    }
}

impl StatefulOutputPin for OpenDrainIOPin {
    fn is_set_high(&self) -> Result<bool, Self::Error> {
        self.pin.is_set_high()
    }

    fn is_set_low(&self) -> Result<bool, Self::Error> {
        self.pin.is_set_low()
    }
}

#[cortex_m_rt::entry]
fn main() -> ! {
    let peripherals = hal::pac::Peripherals::take().unwrap();
    let port0 = hal::gpio::p0::Parts::new(peripherals.P0);
    let port1 = hal::gpio::p1::Parts::new(peripherals.P1);

    // Create timer
    let mut timer = hal::timer::Timer::new(peripherals.TIMER0);

    // Get DOGM132W-5 pins
    let mut disp_rst = port0.p0_12.into_push_pull_output(Level::High);
    let disp_cs = port1.p1_09.into_push_pull_output(Level::High).degrade();
    let disp_a0 = port0.p0_23.into_push_pull_output(Level::High).degrade();
    let disp_scl = port0.p0_21.into_push_pull_output(Level::High).degrade();
    let disp_si = port0.p0_19.into_push_pull_output(Level::Low).degrade();

    // Get DHT22 pin
    let pin_dht22 = port0
        .p0_22
        .into_open_drain_output(OpenDrainConfig::Standard0Disconnect1, Level::High)
        //.into_push_pull_output(Level::Low)
        .degrade();

    let mut dht22 = OpenDrainIOPin::new(pin_dht22);

    // Create DOGM132W-5 spi bus
    let disp_spi = SPIInterface::new(
        hal::Spim::new(
            peripherals.SPIM0,
            hal::spim::Pins {
                sck: disp_scl,
                mosi: Some(disp_si),
                miso: None,
            },
            hal::spim::Frequency::M8,
            hal::spim::MODE_3,
            0,
        ),
        disp_a0,
        disp_cs,
    );

    // Create DOGM132W-5 display driver
    let mut disp = ST7565::new(disp_spi, DOGM132W5).into_graphics_mode();
    disp.reset(&mut disp_rst, &mut timer).unwrap();
    disp.flush().unwrap();
    disp.set_display_on(true).unwrap();

    // Wait for DHT22 initialization
    timer.delay_ms(500u16);

    let font = FontRenderer::new::<fonts::u8g2_font_timB10_tf>().with_line_height(14);

    loop {
        // Read sensor value
        let value = dht_sensor::dht22::Reading::read(&mut timer, &mut dht22).unwrap();
        defmt::info!("{:?} C", value.temperature);
        defmt::info!("{:?} %", value.relative_humidity);

        // Draw content
        font.render(
            format_args!("{:?} C\n{:?} %", value.temperature, value.relative_humidity),
            Point::new(30, 2),
            VerticalPosition::Top,
            FontColor::Transparent(BinaryColor::On),
            &mut disp,
        )
        .unwrap();

        // Send content to display
        disp.flush().unwrap();

        // Wait for new DHT22 value
        timer.delay_ms(2000u16);

        // Draw content
        font.render(
            format_args!("{:?} C\n{:?} %", value.temperature, value.relative_humidity),
            Point::new(30, 2),
            VerticalPosition::Top,
            FontColor::Transparent(BinaryColor::Off),
            &mut disp,
        )
        .unwrap();
    }
}
