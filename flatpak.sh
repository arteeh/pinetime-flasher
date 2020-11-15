#!/bin/bash

flatpak run org.flatpak.Builder builddir com.arteeh.Flasher.yml --force-clean
flatpak run org.flatpak.Builder builddir com.arteeh.Flasher.yml --force-clean --user --install
