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

#ifndef FMDX_RADIO_UTILS_H
#define FMDX_RADIO_UTILS_H

class Utils
{
public:
    static void serialHex8(uint8_t value);
    static void serialHex16(uint16_t value);
    static void serialHexPi(uint16_t value, uint8_t error);
    static void serialDecimal(int16_t value, uint8_t precision);
};

#endif
