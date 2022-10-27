usb_port="/dev/ttyACM0"
cargo build && cargo espflash "$usb_port" && cargo espmonitor "$usb_port"
