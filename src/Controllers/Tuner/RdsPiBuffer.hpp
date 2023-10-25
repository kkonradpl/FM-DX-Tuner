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

#ifndef FMDX_RADIO_RDSPIBUFFER_H
#define FMDX_RADIO_RDSPIBUFFER_H

class RdsPiBuffer
{
public:
    enum State : uint8_t
    {
        STATE_CORRECT     = 0,
        STATE_VERY_LIKELY = 1,
        STATE_LIKELY      = 2,
        STATE_UNLIKELY    = 3,
        STATE_INVALID     = 4
    };

    RdsPiBuffer();
    void add(uint16_t value, bool error);
    State get(uint16_t &valueOut);
    void clear();

private:
    /* BUFF_SIZE must be a multiple of 8 */
    static constexpr uint8_t BUFF_SIZE = 64;
    uint16_t buff[BUFF_SIZE];
    uint8_t errorBuff[BUFF_SIZE/8];
    uint8_t fill;
    uint8_t pos;
    State lastState;

    State getState(uint16_t value);
};

#endif
