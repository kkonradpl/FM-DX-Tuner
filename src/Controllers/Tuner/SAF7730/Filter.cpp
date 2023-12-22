
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

#include "Filter.hpp"
#include "Dirana.hpp"

#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

void
Filter::setAdaptive()
{
    i2c.write24(DIRANA_FIR_FM_MODE_1, (uint16_t)DIRANA_FIR_FM_ADAPTIVE_1);
    i2c.write24(DIRANA_FIR_FM_MODE_2, (uint16_t)DIRANA_FIR_FM_ADAPTIVE_2);

    for (uint8_t bank = 0; bank < DIRANA_FILTER_ADAPTIVE_COUNT; bank++)
    {
        uint8_t filter = pgm_read_byte_near(DIRANA_FILTER_ADAPTIVE + bank);
        this->setFilter(bank, filter);
    }
}

void
Filter::setFixed(Mode    mode,
                 uint8_t filter)
{
    /* Use another filter bank to avoid audio distortion
       when changing bandwidth next to stronger adjacent station */
    this->flag = !this->flag;

    /* Write the FIR filter coefficients into filter bank 14 or 15 */
    const uint8_t bank = 0x0F - this->flag;
    this->setFilter(bank, filter);

    if (mode == MODE_FM)
    {
        this->i2c.write24(DIRANA_FIR_FM_FIXED_1, bank);
        this->i2c.write24(DIRANA_FIR_FM_MODE_1, (uint16_t)DIRANA_FIR_FM_FIXED_1);
        this->i2c.write24(DIRANA_FIR_FM_FIXED_2, bank);
        this->i2c.write24(DIRANA_FIR_FM_MODE_2, (uint16_t)DIRANA_FIR_FM_FIXED_2);
    }
    else if (mode == MODE_AM)
    {
        this->i2c.write24(DIRANA_FIR_AM_FIXED_1, bank);
        this->i2c.write24(DIRANA_FIR_AM_ATT_1, 0);
        this->i2c.write24(DIRANA_FIR_AM_FIXED_2, bank);
        this->i2c.write24(DIRANA_FIR_AM_ATT_2, 0);
    }
}

void
Filter::setFilter(uint8_t bank, 
                  uint8_t filter)
{
    uint32_t address = DIRANA_FIR_MEMORY + 32 * bank;

    uint8_t i = 0;
    while (i < DIRANA_FILTER_COEFF_LENGTH)
    {
        uint32_t coeff;
        coeff = (uint32_t)pgm_read_byte_near(DIRANA_FILTER_COEFF + (filter * DIRANA_FILTER_COEFF_LENGTH) + (i++)) << 16;
        coeff |= (uint32_t)pgm_read_byte_near(DIRANA_FILTER_COEFF + (filter * DIRANA_FILTER_COEFF_LENGTH) + (i++)) << 8;
        
        this->i2c.write24(address, coeff);
        address++;
    }
}

