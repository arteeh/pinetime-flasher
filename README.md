# Pinetime Companion

[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

A GTK companion app for the Pinetime, for use on Pinephones and Linux desktops. My goal for this app is to replace
[pinetime-updater](https://github.com/lupyuen/pinetime-updater) for:
- Flashing the device using an ST-Link or Raspberry Pi

and to replace NRF Connect/Gadgetbridge/Amazfish for:
- Updating firmware over Bluetooth
- Syncing time and date on the watch
- Sending notifications to the watch (in the background)

...making this app a one-stop-shop for linking your PineTime with the PinePhone.

## How to build

You need to install the GTK3 library to build this project. On Debian the package you need is called `libgtk-3-dev`.

Clone the repo, and run `make app`. Then run the app with `./app`. Clean with `make clean`

## How to contribute

The app is written in C, and the UI is created using [Glade](https://glade.gnome.org/). Glade is a tool for building GTK interfaces. You can open `app.glade` with Glade and tinker with the UI, then you can work in `app.c` to work on event handling and other functionality.

## Who am I?

I've been casually hanging around the PineTime community since about November 2019, talking to people in the chat. I'm also working on UI designs and mockups for the PineTime which you can find [here](https://www.gitlab.com/arteeh/pinetimeos). I'm arteeh#1186 on Discord and you can otherwise contact me through the links on [my website](https://www.arteeh.com/).
