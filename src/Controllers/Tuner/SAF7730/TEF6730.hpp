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

#ifndef FMDX_TUNER_SAF7730_TEF6730_H
#define FMDX_TUNER_SAF7730_TEF6730_H

#include "I2cDirana.hpp"

class TEF6730
{
public:
    TEF6730(I2cDirana &_i2c);

    void setAlignment(uint8_t value);
    void setRfBoost(bool state);
    void setIfBoost(bool state);
    void setAgc(uint8_t value);
    uint32_t setFrequency(uint32_t freq, bool consistent);

    uint8_t getAlignment() const;
    bool getRfBoost() const;
    bool getIfBoost() const;
    uint8_t getAgc() const;
    uint32_t getFrequency() const;
    uint8_t getBand() const;

    void tune();
    
private:
    static uint32_t getFrequency(const uint8_t *regs);

    enum : uint8_t
    {
        REG_MSA = 0,
        REG_CONTROL = 1,
        REG_PLLM = 2,
        REG_PLLL = 3,
        REG_DAA = 4,
        REG_AGC = 5,
        REG_BAND = 6,
        REG_COUNT
    };

    I2cDirana &i2c;
    uint8_t regUsed[REG_COUNT];
    uint8_t regTemp[REG_COUNT];
};

#endif
