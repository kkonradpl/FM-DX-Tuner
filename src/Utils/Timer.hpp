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

#ifndef FMDX_TUNER_TIMER_H
#define FMDX_TUNER_TIMER_H

#include <Arduino.h>

class Timer
{
public:
    typedef unsigned long Interval;

    enum Mode : bool
    {
        Oneshot = false,
        Continous = true
    };

    bool process(Mode mode);
    void set(Interval value);
    void unset();

private:
    Interval timer = 0;
    Interval interval;
    bool enabled = false;
};

#endif