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
Timer::check()
{
    const Timer::Interval now = millis();
    if ((now - this->timer) < this->interval)
    {
        return false;
    }

    this->timer = now;
    return true;
}

Timer::Interval
Timer::getInterval(void)
{
    return this->interval;
}

void
Timer::setInterval(Timer::Interval _interval)
{
    this->interval = _interval;
}