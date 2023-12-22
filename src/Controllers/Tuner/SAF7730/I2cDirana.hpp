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

#ifndef FMDX_TUNER_I2C_DIRANA_H
#define FMDX_TUNER_I2C_DIRANA_H

#include "../../../I2c/I2cDriver.hpp"

class I2cDirana : public I2cDriver
{
public:
    I2cDirana(uint8_t _dspAddr, uint8_t _tunerAddr) : I2cDriver(_dspAddr), tunerAddr(_tunerAddr) {};
    
    uint16_t read16(uint32_t addr);
    uint32_t read24(uint32_t addr);
    void write16(uint32_t addr, uint16_t data);
    void write24(uint32_t addr, uint32_t data);
    void writeConfig(const uint8_t *data);
    void writeTuner(uint32_t addr, const uint8_t *data, uint8_t dataLen);

private:
    uint8_t tunerAddr;
};

#endif
