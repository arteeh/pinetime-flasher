::  Flash to nRF52 ROM with ST-Link on Windows

openocd\bin\openocd.exe ^
    -f scripts/nrf52/flash-init.ocd ^
    -f interface/stlink.cfg ^
    -c "transport select hla_swd" ^
    -f target/nrf52.cfg ^
    -f scripts/nrf52/flash-program.ocd
