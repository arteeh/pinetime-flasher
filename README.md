# Pinetime Companion

A GTK companion app for the Pinetime, for use on Pinephones and Linux desktops.

## Project goals

This is a Linux GTK application targeted towards smartphones, containing everything you need to interact with your PineTime smartwatch. For users, it will contain the following functionality:

- Syncing the time and date on the watch
- Sending phone notifications to the watch
- Receiving heartbeat and step counter data from the watch and visualising it
- Updating the watch firmware over Bluetooth
- Customizing the bootloader image
- Installing and removing apps on the watch (This is a long-term goal)

For developers who own a PineTime devkit and an ST-Link, the app can do the following:

- Get the latest Enhanced MCUBoot bootloader and flash it to the watch with the click of a button
- Get the latest InfiniTime firmware and flash it to the watch
- Flash a binary from a given web URL to the watch
- Flash a binary stored on your device to the watch
- (Optional) Automatically get other popular firmware like Wasp OS and RIOT OS and flash it as well

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

To build a flatpak, run flatpak.sh in the project root. Note: the flatpak builder currently grabs the app from this git repository, so your local changes will not be included in the flatpak you build. Another note: Flashing currently doesn't work in the flatpak because the sandbox doesn't have access to udev. This can be worked around by asking the user to install the udev rule themselves.

## How to develop

Install Glade, and make sure it recognizes libhandy widgets. If Glade was installed as a flatpak, you need org.gnome.Sdk version 3.38 as well.

Open companion.ui in Glade to create and modify the UI, and to connect buttons with event handlers (under 'signals'). Also read [this](glade-sucks.md).

companion.c is the main file with the main function. It handles all the initialization and starts GTK.

updates.c contains all functionality related to the 'Updates' tab.

advanced.c contains all functionality related to the 'Advanced' tab.

## Credits

- [Lup Yuen Lee](https://lupyuen.github.io/) for creating [pinetime-updater](https://github.com/lupyuen/pinetime-updater). The 'Advanced' functionality is practically a fork of this effort.

## Who am I?

I've been casually hanging around the PineTime community since about November 2019, talking to people in the chat. I'm also working on UI designs and mockups for the PineTime which you can find [here](https://www.gitlab.com/arteeh/pinetimeos). On Matrix I'm @arteeh:matrix.org and you can otherwise contact me through the links on [my website](https://www.arteeh.com/).
