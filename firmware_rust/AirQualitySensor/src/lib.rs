#![no_main]
#![no_std]

pub mod modules;
pub mod subsystems;

use defmt_rtt as _; // global logger

// TODO(5) adjust HAL import
pub use stm32g0xx_hal as hal;

use panic_probe as _;

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
