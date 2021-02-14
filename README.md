# Pinetime Flasher

A GTK app for easily flashing the PineTime smartwatch with an ST-Link


## Project goals

This is a Linux GTK application made for developers and tinkerers to easily flash the PineTime smartwatch using an ST-Link programmer. If you own a PineTime devkit and an ST-Link, the app can do the following for you:

- Get the latest versions of Enhanced MCUBoot, Infinitime, wasp-bootloader, and RIOT OS and flash it at the click of a button
- Flash any binary from a given web URL to the watch
- Flash any binary stored on your device to the watch

## How to build

To build, you need to have the following packages installed: 

Arch:

`gcc`
`make`
`pkgconf`
`gtk4`
`libhandy`
`libcurl-compat`

Build by running `make` in the project directory. Start the app by running `make run`.

To build a flatpak, run flatpak.sh in the project root. Note: the flatpak builder currently grabs the app from this git repository, so your local changes will not be included in the flatpak you build. Another note: Flashing currently doesn't work in the flatpak because the sandbox doesn't have access to udev. This can be worked around by asking the user to install the udev rule themselves.

## How to develop

Install Glade, and make sure it recognizes libhandy widgets. If Glade was installed as a flatpak, you need org.gnome.Sdk version 3.38 as well.

Open flasher.ui in Glade to create and modify the UI, and to connect buttons with event handlers (under 'signals'). Also read [this](glade-sucks.md).

flasher.c is the main file with the main function. It handles all the initialization and starts GTK.

## Credits

- [Lup Yuen Lee](https://lupyuen.github.io/) for creating [pinetime-updater](https://github.com/lupyuen/pinetime-updater). The functionality in my app is practically a fork of this effort.

## Who am I?

I've been casually hanging around the PineTime community since about November 2019, talking to people in the chat. I'm also working on UI designs and mockups for the PineTime which you can find [here](https://www.gitlab.com/arteeh/pinetimeos). On Matrix I'm @arteeh:matrix.org and you can otherwise contact me through the links on [my website](https://www.arteeh.com/).
