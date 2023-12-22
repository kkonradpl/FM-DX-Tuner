
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

#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include "Alignment.hpp"
#include "../../../../ConfigSAF7730.hpp"

uint8_t
Alignment::get(uint32_t frequency)
{
    uint8_t value = 0;
    uint32_t threshold;
    while ((threshold = pgm_read_dword_near(TUNER_SAF7730_ALIGNMENT + value)))
    {
        if (frequency >= threshold)
        {
            break;
        }

        value++;
    }

    value += TUNER_SAF7730_ALIGNMENT_START;
    return (threshold == 0 ? value + 1 : value);
}
