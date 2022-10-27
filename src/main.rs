#![no_std]
#![no_main]

use core::time::Duration;
use esp_println::{logger::init_logger, println};

use esp32_hal::{
    prelude::*,
    clock::ClockControl,
    ledc::{channel::{self, ChannelIFace}, timer::{self, TimerIFace}, HighSpeed, LEDC},
    pac::Peripherals,
    timer::TimerGroup,
    Rtc,
    IO,
    Delay,
};
use esp_backtrace as _;

const MAX_DUTY_PCT: u8 = 100;

#[xtensa_lx_rt::entry]
fn main() -> ! {
    init_logger(log::LevelFilter::Info);

    let peripherals = Peripherals::take().unwrap();
    let mut system = peripherals.DPORT.split();
    let clocks = ClockControl::boot_defaults(system.clock_control).freeze();

    // Disable the RTC and TIMG watchdog timers
    let mut rtc = Rtc::new(peripherals.RTC_CNTL);
    let timer_group0 = TimerGroup::new(peripherals.TIMG0, &clocks);
    let mut wdt0 = timer_group0.wdt;
    let timer_group1 = TimerGroup::new(peripherals.TIMG1, &clocks);
    let mut wdt1 = timer_group1.wdt;

    let io = IO::new(peripherals.GPIO, peripherals.IO_MUX);

    rtc.rwdt.disable();
    wdt0.disable();
    wdt1.disable();

    let ledc = LEDC::new(
        peripherals.LEDC,
        &clocks,
        &mut system.peripheral_clock_control,
    );

    let mut hstimer0 = ledc.get_timer::<HighSpeed>(timer::Number::Timer0);
    hstimer0
        .configure(timer::config::Config {
            duty: timer::config::Duty::Duty8Bit,
            clock_source: timer::HSClockSource::APBClk,
            frequency: 156250u32.Hz(),
        })
        .unwrap();

    let mut channel0 = ledc.get_channel(channel::Number::Channel0, io.pins.gpio2.into_push_pull_output());


    let delay = Delay::new(&clocks);

    let mut duty_pct = 0u8;
    loop {
        delay.delay(Duration::from_millis(50).as_micros() as u32);

        duty_pct += 1;
        if duty_pct > MAX_DUTY_PCT {
            duty_pct = 1;
        }

        println!("set duty to {}", duty_pct);

        // channel0.set_duty(duty_pct).unwrap();
        channel0
            .configure(channel::config::Config {
                timer: &hstimer0,
                duty_pct,
            })
            .unwrap();
    }
}
