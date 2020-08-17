# Pinetime Companion

A GTK companion app for the Pinetime, for use on Pinephones and Linux desktops. My goal for this app is to replace
[pinetime-updater](https://github.com/lupyuen/pinetime-updater) for:
- Flashing the device using an ST-Link or Raspberry Pi

and to replace NRF Connect for:
- Updating firmware over Bluetooth
- Syncing time and date on the watch
- Sending notifications to the watch (in the background)

...making this app a one-stop-shop for linking your PineTime with the PinePhone.

## How to build

You need to install the GTK3 library to build this project. On Debian the package you need is called "libgtk-3-dev".

Clone the repo, and run `make app`. Then run the app with `./app`. Clean with `make clean`

## How to contribute

The app is written in C, and the UI is created using [Glade](https://glade.gnome.org/). Glade is a tool for building GTK interfaces. You can open `app.glade` with Glade and tinker with the UI, then you can work in `app.c` to work on event handling and other functionality.
