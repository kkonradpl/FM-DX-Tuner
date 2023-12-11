/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2023  Konrad Kosmatka 
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef FMDX_TUNER_VOLUME_H
#define FMDX_TUNER_VOLUME_H

#include "TunerDriver.hpp"

class Volume
{
public:
    Volume(TunerDriver &_driver) : driver(_driver) {};

    void setVolume(uint8_t value);
    uint8_t getVolume(void);

    void mute(void);
    void unMute(void);
    bool isMuted(void);

private:
    TunerDriver &driver;
    uint8_t volume = 100;
    uint8_t muteRefs = 0;
};

#endif
