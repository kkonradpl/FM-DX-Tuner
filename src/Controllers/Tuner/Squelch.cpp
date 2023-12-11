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
#include "Squelch.hpp"


Squelch::Squelch(Volume  &_volume,
                 uint8_t  _timeout)
    : volume(_volume)
{
    this->timeout = _timeout;
    this->mode = SQUELCH_NONE;
    this->threshold = 0;
    this->countdown = this->timeout;
}

void
Squelch::process(int8_t value)
{
    bool exceeded;

    if (this->mode == SQUELCH_CCI)
    {
        exceeded = value <= this->threshold;
    }
    else
    {
        exceeded = value >= this->threshold;
    }

    if (exceeded)
    {
        if (this->isMuted())
        {
            this->volume.unMute();
        }

        this->countdown = this->timeout;
    }
    else if (this->countdown)
    {
        if (--this->countdown == 0)
        {
            this->volume.mute();
        }
    }
}

void
Squelch::set(SquelchMode _mode,
             int8_t      _threshold)
{
    this->mode = _mode;
    this->threshold = _threshold;

    if (this->mode == SQUELCH_NONE)
    {
        if (this->isMuted())
        {
            this->volume.unMute();
        }

        this->countdown = this->timeout;
    }
}

SquelchMode
Squelch::getMode(void)
{
    return this->mode;
}

int8_t
Squelch::getThreshold(void)
{
    return this->threshold;
}

bool
Squelch::isMuted(void)
{
    return (this->countdown == 0);
}
