//--------------------------------------------------------------------------------------------------
// Public Definitions
//--------------------------------------------------------------------------------------------------

pub mod common;
pub mod gpio;
pub mod mailbox;
pub mod uart;

use crate::{
    bcm::{common::map, gpio::Gpio, mailbox::Mailbox as my_mailbox, uart::Uart},
    println,
};

//--------------------------------------------------------------------------------------------------
// Global instances
// -------------------------------------------------------------------------------------------------

pub static UART: Uart = unsafe { Uart::new(map::mmio::UART_START) };
pub static GPIO: Gpio = unsafe { Gpio::new(map::mmio::GPIO_START) };
pub static MAILBOX: my_mailbox = unsafe { my_mailbox::new(map::mmio::MAILBOX_START) };

//--------------------------------------------------------------------------------------------------
// Public Code
// -------------------------------------------------------------------------------------------------

pub fn hardware_init() {
    GPIO.init();
    UART.init();
    let (val, _) = MAILBOX.get(mailbox::MailboxTag::GetBoardRevision);
    println!("Board revision(loading): {:x}", val);
}
