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
#include "TEF6730.hpp"

TEF6730::TEF6730(I2cDirana &_i2c) : i2c(_i2c)
{
    this->regTemp[REG_MSA] = 0x80;
    this->regTemp[REG_CONTROL] = 0x00;
    this->regTemp[REG_PLLM] = 0x00;
    this->regTemp[REG_PLLL] = 0x00;
    this->regTemp[REG_DAA] = 0x00;
    this->regTemp[REG_AGC] = 0xC8;
    this->regTemp[REG_BAND] = 0x00;
}
        
void
TEF6730::setAlignment(uint8_t value)
{
    this->regTemp[REG_DAA] = value & 0x7F;
}

void
TEF6730::setRfBoost(bool state)
{
    if (state)
    {
        this->regTemp[REG_CONTROL] |= B10000000;
    }
    else
    {
        this->regTemp[REG_CONTROL] &= B01111111;
    }
}

void
TEF6730::setIfBoost(bool state)
{
    if (state)
    {
        this->regTemp[REG_CONTROL] |= B00010000;
    }
    else
    {
        this->regTemp[REG_CONTROL] &= B11101111;
    }
}

void
TEF6730::setAgc(uint8_t value)
{
    this->regTemp[REG_AGC] &= B11110011;
    this->regTemp[REG_AGC] |= ((value & B11) << 2);
}

uint32_t
TEF6730::setFrequency(uint32_t freq,
                      bool     consistent)
{
    uint16_t PLL;
    
    if ((freq >= 55000) && (freq <= 150000)) /* FM BAND */
    {
        if (freq % 50 || freq > 108000 || consistent)
        {
            PLL = ((freq + 10700) * 2) / 10;
            this->regTemp[REG_BAND] = B00110001; /* 5 kHz step, fref=10 kHz */
        }
        else
        {
            /* Use 50 kHz step where possible to get clean audio output */
            PLL = ((freq + 10700) * 2) / 100;
            this->regTemp[REG_BAND] = B00100001; /* 50 kHz step, fref=100 kHz */
        }
    }
    else if ((freq >= 100) && (freq <= 1900)) /* LW & MW BAND */
    {
        PLL = ((freq + 10700) * 20) / 20;
        this->regTemp[REG_BAND] = B11101101;
    }
    else if ((freq >= 1901) && (freq <= 5899)) /* AM SW 120m to 60m */
    {
        PLL = ((freq + 10700) * 16) / 10;
        this->regTemp[REG_BAND] = B11010001;
    }
    else if ((freq >= 5900) && (freq <= 13870)) /* AM SW 49m to 22m */
    {
        PLL = ((freq + 10700) * 10) / 10;
        this->regTemp[REG_BAND] = B10110001;
    }
    else if ((freq >= 13871) && (freq <= 19020)) /* AM SW 25m to 15m */
    {
        PLL = ((freq + 10700) * 8) / 10;
        this->regTemp[REG_BAND] = B10010001;
    }
    else if ((freq >= 19021) && (freq <= 30000)) /* AM SW 16m to 11m */
    {
        PLL = ((freq + 10700) * 6) / 10;
        this->regTemp[REG_BAND] = B01110001;
    }
    else
    {
        return 0;
    }
    
    this->regTemp[REG_PLLM] = PLL >> 8;
    this->regTemp[REG_PLLL] = PLL & 0xFF;
    return TEF6730::getFrequency(this->regTemp);
}

uint8_t
TEF6730::getAlignment() const
{
    return this->regUsed[REG_DAA] & 0x7F;
}

bool
TEF6730::getRfBoost() const
{
    return this->regUsed[REG_CONTROL] & B10000000;
}

bool
TEF6730::getIfBoost() const
{
    return this->regUsed[REG_CONTROL] & B00010000;
}

uint8_t
TEF6730::getAgc() const
{
    return (this->regUsed[REG_AGC] & B00001100) >> 2;
}

uint32_t
TEF6730::getFrequency() const
{
    return this->getFrequency(this->regUsed);
}

uint8_t
TEF6730::getBand() const
{
    return this->regUsed[REG_BAND];
}

void
TEF6730::tune()
{
    static constexpr uint32_t addr = 0x00FFFF;
    this->i2c.writeTuner(addr, this->regTemp, REG_COUNT);

    for (uint8_t i = 0; i < REG_COUNT; i++)
    {
        this->regUsed[i] = this->regTemp[i];
    }
}

uint32_t
TEF6730::getFrequency(const uint8_t *regs)
{
    uint32_t PLL;
    PLL = ((uint16_t)regs[REG_PLLM]) << 8;
    PLL |= regs[REG_PLLL];

    if (regs[REG_BAND] == B00100001)
    {
        return 100 * PLL / 2 - 10700;
    }
    else if (regs[REG_BAND] == B00110001)
    {
        return 10 * PLL / 2 - 10700;
    }
    else if (regs[REG_BAND] == B11101101)
    {
        return 20 * PLL / 20 - 10700;
    }
    else if (regs[REG_BAND] == B11010001)
    {
        return 10 * PLL / 16 - 10700;
    }
    else if (regs[REG_BAND] == B10110001)
    {
        return 10 * PLL / 10 - 10700;
    }
    else if (regs[REG_BAND] == B10010001)
    {
        return 10 * PLL / 8 - 10700;
    }
    else if (regs[REG_BAND] == B01110001)
    {
        return 10 * PLL / 6 - 10700;
    }

    return 0;
}
