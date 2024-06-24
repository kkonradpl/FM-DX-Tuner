/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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
#include "../Comm.hpp"
#include "Utils.hpp"

void
Utils::serialHex8(uint8_t value)
{
    Comm.print(value >> 4, HEX);
    Comm.print(value & 0xF, HEX);
}

void
Utils::serialHex16(uint16_t value)
{
    Utils::serialHex8((uint8_t)(value >> 8));
    Utils::serialHex8((uint8_t)value);
}

void
Utils::serialHexPi(uint16_t value,
                   uint8_t  error)
{
    serialHex16(value);
    
    while (error--)
    {
        Comm.print('?');
    }
}

void
Utils::serialDecimal(int16_t value,
                     uint8_t precision)
{
    int32_t multiplier = 1;
    for (uint8_t i = 0; i < precision; i++)
    {
        multiplier *= 10;
    }

    int16_t integer = value / multiplier;
    int16_t fraction = value % multiplier;
    
    if (fraction < 0)
    {
        fraction = -fraction;
        if (integer == 0)
        {
            Comm.print('-');
        }
    }

    Comm.print(integer, DEC);

    if (fraction)
    {
        Comm.print('.');

        /* Count leading zeros */
        uint8_t i = precision;
        int16_t tmp = fraction;
        while (i > 0 && tmp)
        {
            i--;
            tmp /= 10;
        }

        /* Print leading zeros */
        while (i--)
        {
            Comm.print('0');
        }

        /* Remove all trailing zeros */
        while (fraction % 10 == 0)
        {
            fraction /= 10;
        }

        /* Print rest of the fractional part */
        Comm.print(fraction, DEC);
    }
}
