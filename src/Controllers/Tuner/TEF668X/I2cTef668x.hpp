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

#ifndef FMDX_TUNER_I2C_TEF668X_H
#define FMDX_TUNER_I2C_TEF668X_H

#include "../../../I2c/I2cDriver.hpp"

class I2cTef668x : public I2cDriver
{
public:
    I2cTef668x(uint8_t address) : I2cDriver(address) {};
    
    void read(uint8_t module, uint8_t command, uint8_t length, uint16_t *bufferOut);
    void write(uint8_t module, uint8_t command, uint8_t length, ...);
    void writePatch(const uint8_t *data, size_t dataLen);
    void writeConfig(const uint8_t *data);
};

#endif
