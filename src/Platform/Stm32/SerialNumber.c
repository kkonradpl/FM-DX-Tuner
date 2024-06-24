/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2024  Konrad Kosmatka
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

#ifdef ARDUINO_ARCH_STM32
#include <stm32f0xx.h>
#include "SerialNumber.h"


static void
convert(uint8_t   value,
        uint16_t *out)
{
    out[0] = '0' + (value >> 4);
    out[1] = '0' + (value & 0xF);

    const uint8_t diff = 'A' - '9' - 1;
    if (out[0] > '9')
        out[0] += diff;

    if (out[1] > '9')
        out[1] += diff;
}

size_t
Platform_Stm32_SerialNumber_Read(uint16_t *out)
{
    const size_t length = 3;

    for (size_t i = 0; i < length; i++)
    {
        uint32_t value = ((uint32_t*)UID_BASE)[i];
        convert((value & 0xFF000000) >> 24, out + i * 8);
        convert((value & 0xFF0000) >> 16, out + i * 8 + 2);
        convert((value & 0xFF00) >> 8, out + i * 8 + 4);
        convert((value & 0xFF), out + i * 8 + 6);
    }

    return length * sizeof(uint32_t) * 2;
}

#endif
