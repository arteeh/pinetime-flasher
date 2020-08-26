# Pinetime Companion

A GTK companion app for the Pinetime, for use on Pinephones and Linux desktops. My goal for this app is to replace
[pinetime-updater](https://github.com/lupyuen/pinetime-updater) for:
- Flashing the device using an ST-Link or Raspberry Pi

and to replace NRF Connect/Gadgetbridge/Amazfish for:
- Updating firmware over Bluetooth
- Syncing time and date on the watch
- Sending notifications to the watch (in the background)

...making this app a one-stop-shop for linking your PineTime with the PinePhone.

## How to build

I build the project using GNOME Builder, which in turn decided to use Meson. I recommend you clone the project using Builder and working in there.
You need to have libhandy (`libhandy1-devel` on Fedora) installed to build this project.

## Who am I?

I've been casually hanging around the PineTime community since about November 2019, talking to people in the chat. I'm also working on UI designs and mockups for the PineTime which you can find [here](https://www.gitlab.com/arteeh/pinetimeos). I'm arteeh#1186 on Discord and you can otherwise contact me through the links on [my website](https://www.arteeh.com/).
