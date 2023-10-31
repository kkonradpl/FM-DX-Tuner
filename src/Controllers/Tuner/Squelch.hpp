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

#ifndef FMDX_TUNER_SQUELCH_H
#define FMDX_TUNER_SQUELCH_H

#include "../../Protocol.h"

class Squelch
{
public:
    Squelch(uint8_t _timeout);

    enum Action : uint8_t
    {
        None,
        Mute,
        Unmute
    };

    Action update(int8_t value);
    void set(SquelchMode _mode, int8_t _threshold);
    SquelchMode getMode(void);
    int8_t getThreshold(void);
    bool isMuted(void);

private:
    SquelchMode mode;
    int8_t threshold;
    uint8_t countdown;
    uint8_t timeout;
};

#endif
