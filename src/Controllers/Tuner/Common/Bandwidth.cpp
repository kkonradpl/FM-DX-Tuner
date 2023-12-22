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
#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif
#include "Bandwidth.hpp"

uint32_t
Bandwidth::lookup(const uint16_t *table,
                  uint16_t        reqValue,
                  uint8_t        *idOut)
{
    uint16_t minDiff = UINT16_MAX;
    uint16_t output = 0;

    uint8_t i = 0;
    while (uint16_t value = pgm_read_word_near(table + i))
    {
        uint16_t diff = abs((int16_t)(reqValue - value));

        if (diff < minDiff)
        {
            minDiff = diff;
            output = value;
            if (idOut)
            {
                *idOut = i;
            }
        }

        i++;
    }

    return output;
}
