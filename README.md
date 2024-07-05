FM-DX Tuner
=======

Multi-plaftorm microcontroller firmware for NXP radio tuners

Copyright (C) 2023-2024  Konrad Kosmatka

https://fmdx.pl/tuner/

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

# Work in progress

Firmware is currently compatible with XDR-GTK software and FM-DX
Webserver, but the protocol is going to be changed in the future.

# Microcontroller support

- AVR (ATmega328P)
- ESP32 (PE5PVB board)
- STM32 (STM32F072 crystal-less)

# Tuner support

- TEF668X driver:
    - TEF6686, TEF6687 (V102 and V205)
    - TEF6688, TEF6689 (untested)
    - TEA685X (not recommended)
- SAF7730 driver:
    - XDR-F1HD (extended modification)
- SAF775X driver (highly experimental):
    - Mini SAF775X Radio (from Aliexpress)

# Configuration

Check the `Config.hpp` file and the corresponding tuner configurations.

The directory `presets` contains predefined configuration files.

# TEF668X configuration

Make sure to set correct `TUNER_TEF668X_REF_CLOCK` frequency. Wrong
frequency value will not stop tuner from working, but the reception
will be corrupted.

The PE5PVB board contains a hardware jumper for selection of the clock
frequency (keep the default zero value of `TUNER_TEF668X_REF_CLOCK`).

# Arduino AVR boards

TEF668X tuners require 3.3V logic level on the I2C bus, but the AVR
based Arduino boards are 5V, which is out of specification for the
TEF668X ICs. Because of that, the `TUNER_AVR_PULLUP` is set by default
to false.

Due to the nature of the I2C bus, it is OK to use a microcontroller
with 5V logic on a 3.3V I2C bus (during communication, the bus lines
are only being pulled down to the ground). Some of the Chinese tuner
modules (especially the ones available currently on Aliexpress)
already contain pull-up resistors, so the tuner will work out of the
box with the default configuration.

If your tuner module does not have the 3.3V I2C pull-ups (SDA/A4 and
SCL/A5 is almost 0V), then you need to solder 10 kΩ resistors: one
between 3.3V and SDA (A4), and another one between 3.3V and SCL (A5).