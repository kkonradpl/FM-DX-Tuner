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
#include "I2cAvr.hpp"
#include "../Utils/Utils.hpp"

bool
I2cAvr::busStartWrite()
{
    return bus.start(this->address | I2C_WRITE);
}

bool
I2cAvr::busStartRead(uint8_t length)
{
    this->readLength = length;
    return bus.start(this->address | I2C_READ);
}

bool
I2cAvr::busWrite(uint8_t data)
{
    return bus.write(data);
}

uint8_t
I2cAvr::busRead()
{
    this->readLength--;
    return bus.read(this->readLength == 0);
}

bool
I2cAvr::busRestartWrite(uint8_t address)
{
    return bus.restart(address | I2C_WRITE);
}

bool
I2cAvr::busRestartRead(uint8_t address,
                       uint8_t length)
{
    this->readLength = length;
    return bus.restart(address | I2C_READ);
}

void
I2cAvr::busStop(void)
{
    bus.stop();
}
