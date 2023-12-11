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

#include <stdint.h>
#include "Volume.hpp"


void
Volume::setVolume(uint8_t value)
{
    this->volume = value;

    if (this->muteRefs == 0)
    {
        this->driver.setVolume(value);
    }
}

uint8_t
Volume::getVolume()
{
    return this->volume;
}

void
Volume::mute()
{
    if (this->muteRefs == 0)
    {
        this->muteRefs++;
        this->driver.setVolume(0);
    }
}

void
Volume::unMute()
{
    if (this->muteRefs)
    {
        if (--this->muteRefs == 0)
        {
            this->driver.setVolume(this->getVolume());
        }
    }
}

bool
Volume::isMuted()
{
    return this->muteRefs;
}
