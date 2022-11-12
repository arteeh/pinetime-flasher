# Pinetime Flasher

A GTK app for easily flashing the Pinetime smartwatch with an ST-Link.

## Project goals

This is a Linux app made for developers and tinkerers to easily flash the Pinetime smartwatch using an ST-Link V2 programmer. If you own a Pinetime devkit and an ST-Link, the app can do the following for you:

- Get the latest versions of the community bootloader and InfiniTime and flash it with the click of a button
- Flash any other binary from a given web URL to the watch
- Flash any other binary stored on your device to the watch

## How to use

Due to Flatpak not allowing access to Udev, a Udev rule from OpenOCD needs to be manually installed by the user, like this:

- Download [this file](https://sourceforge.net/p/openocd/code/ci/master/tree/contrib/60-openocd.rules?format=raw)
- With root access, copy it into the /etc/udev/rules.d/ folder
- Run `udevadm control --reload-rules`

After that, you can simply wire up your ST-Link adapter to your device and Pinetime, and press one of the flash buttons.