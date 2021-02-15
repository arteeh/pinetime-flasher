# Pinetime Flasher

A GTK app for easily flashing the Pinetime smartwatch with an ST-Link

## Project goals

This is a Linux GTK application made for developers and tinkerers to easily flash the Pinetime smartwatch using an ST-Link V2 programmer. If you own a Pinetime devkit and an ST-Link, the app can do the following for you:

- Get the latest versions of Enhanced MCUBoot, Infinitime and RIOT OS and flash it at the click of a button
- Flash any binary from a given web URL to the watch
- Flash any binary stored on your device to the watch

## How to use

Due to Flatpak not allowing access to Udev, a Udev rule from OpenOCD needs to be manually installed by the user, like this:

- Download [this file](https://sourceforge.net/p/openocd/code/ci/master/tree/contrib/60-openocd.rules?format=raw)
- With root access, copy it into the /etc/udev/rules.d/ folder
- Run `udevadm control --reload-rules`

After that, you can simply wire up your ST-Link adapter to your device and Pinetime, and press one of the flash buttons.

## How to build

This application runs on Flatpak. To build, head into the build directory and run `./flatpak` to see possible commands. If you're building for the first time, you should run `./flatpak -f`, which will clean up any build artifacts, install dependencies and then build the app. Note that this will take a while because it needs to compile libusb, OpenOCD and Libhandy manually. Run `./flatpak -q` to simply build and install the flatpak, after which you can use `./flatpak -p` to start the app.

## How to develop

Open pinetime-flasher.ui in Glade (or a text editor) to modify the UI, and to connect buttons with event handlers (under 'signals'). pinetime-flasher.c is the entire codebase. It handles all the initialization, contains the application logic and starts GTK.

## Credits

- [Lup Yuen Lee](https://lupyuen.github.io/) for creating [pinetime-updater](https://github.com/lupyuen/pinetime-updater). The functionality in my app is practically a fork of this effort.

## Who am I?

I've been casually hanging around the Pinetime community since about November 2019, talking to people in the chat. I'm also working on UI designs and mockups for the Pinetime which you can find [here](https://www.gitlab.com/arteeh/pinetimeos). On Matrix I'm @arteeh:matrix.org and you can otherwise contact me through the links on [my website](https://www.arteeh.com/).
