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

#include <Arduino.h>
#include "Squelch.hpp"

Squelch::State
Squelch::getState(void)
{
    return (this->countdown == 0 ? Mute : Unmute);
}

Squelch::State
Squelch::update(int16_t value)
{
    bool exceeded = (value >= this->threshold);

    if (exceeded)
    {
        if (this->countdown == 0)
        {
            this->countdown = this->timeout;
            return Unmute;
        }

        this->countdown = this->timeout;
        return None;
    }

    if (this->countdown)
    {
        if (--this->countdown == 0)
        {
            return Mute;
        }
    }

    return None;
}


void
Squelch::setThreshold(uint8_t _threshold)
{
    this->threshold = _threshold;
}

uint8_t
Squelch::getThreshold()
{
    return this->threshold;
}
