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
#include "Timer.hpp"

bool
Timer::process(Mode mode)
{
    const Timer::Interval now = millis();

    if (!this->enabled ||
        (now - this->timer) < this->interval)
    {
        return false;
    }

    if (mode == Mode::Oneshot)
    {
        this->enabled = false;
    }
    else
    {
        this->timer = now;
    }

    return true;
}

void
Timer::set(Timer::Interval value)
{
    const Timer::Interval now = millis();
    this->timer = now;
    this->interval = value;
    this->enabled = true;
}

void
Timer::unset()
{
    this->enabled = false;
}
