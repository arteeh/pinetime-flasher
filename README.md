# Pinetime Companion

A GTK companion app for the Pinetime, for use on Pinephones and Linux desktops. My goal for this app is to replace
[pinetime-updater](https://github.com/lupyuen/pinetime-updater) for:
- Flashing the device using an ST-Link or Raspberry Pi

and to replace NRF Connect/Gadgetbridge/Amazfish for:
- Updating firmware over Bluetooth
- Syncing time and date on the watch
- Sending notifications to the watch (in the background)

...making this app a one-stop-shop for interacting with your PineTime, on your PinePhone.

## How to build

To build, you need to have the following packages installed: 

Debian:

`clang` (or `gcc`, but change the makefile)
`make`
`libgtk-3-dev`
`libhandy-1-dev`
`libcurl4-openssl-dev`

Fedora:

`clang` (or `gcc`, but change the makefile)
`make`
`gtk3-devel`
`libhandy1-devel`
`libcurl-devel`

Build by running `make` in the project directory. Start the app by running `make run`.

## Who am I?

I've been casually hanging around the PineTime community since about November 2019, talking to people in the chat. I'm also working on UI designs and mockups for the PineTime which you can find [here](https://www.gitlab.com/arteeh/pinetimeos). On Matrix I'm @arteeh:matrix.org and you can otherwise contact me through the links on [my website](https://www.arteeh.com/).
