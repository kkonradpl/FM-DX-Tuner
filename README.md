FM-DX Tuner
=======

Microcontroller fimware for radio tuners dedicated for FM-DXing

Copyright (C) 2023  Konrad Kosmatka

https://fmdx.pl/tuner/

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

# Work in progress

Warning: Firmware is currently compatible with master branch of
XDR-GTK software, but the protocol is going to be changed.

Only TEF668X driver is available. The SAF7730 driver for
XDR-F1HD (and others) will be ported soon.

# Tested configurations

- TEF6687/V205 + Arduino Nano clone
- TEF6687/V205 + ESP32 board by PE5PVB

# Configuration

Set the TUNER_TEF668X_REF_CLOCK frequency before FW upload.
