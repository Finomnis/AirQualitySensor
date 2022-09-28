#![no_main]
#![no_std]

use defmt_rtt as _;
use dht_sensor::DhtReading;
// global logger
use nrf52840_hal as hal; // memory layout
use panic_probe as _;

use display_interface_spi::SPIInterface;
use embedded_graphics::{pixelcolor::BinaryColor, prelude::*};
use embedded_hal::blocking::delay::DelayMs;
use hal::gpio::{Level, OpenDrainConfig};
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
    let mut dht22 = port0
        .p0_22
        .into_open_drain_input_output(OpenDrainConfig::Standard0Disconnect1, Level::High)
        .degrade();

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

    let font = FontRenderer::new::<fonts::u8g2_font_timB10_tf>().with_line_height(14);

    font.render(
        format_args!("Starting ..."),
        Point::new(30, 2),
        VerticalPosition::Top,
        FontColor::Transparent(BinaryColor::On),
        &mut disp,
    )
    .unwrap();
    disp.flush().unwrap();

    // Wait for DHT22 initialization
    timer.delay_ms(2500u16);

    font.render(
        format_args!("Starting ..."),
        Point::new(30, 2),
        VerticalPosition::Top,
        FontColor::Transparent(BinaryColor::Off),
        &mut disp,
    )
    .unwrap();

    loop {
        // Read sensor value
        match dht_sensor::dht22::Reading::read(&mut timer, &mut dht22) {
            Ok(value) => {
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
            Err(e) => {
                font.render(
                    format_args!("Error:\n{:?}", e),
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

                font.render(
                    format_args!("Error:\n{:?}", e),
                    Point::new(30, 2),
                    VerticalPosition::Top,
                    FontColor::Transparent(BinaryColor::Off),
                    &mut disp,
                )
                .unwrap();
            }
        };
    }
}
