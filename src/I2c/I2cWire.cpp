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
#include <Wire.h>
#include "I2cWire.hpp"
#include "../Utils/Utils.hpp"

I2cWire::I2cWire(uint8_t _address) : address(_address >> 1)
{
    Wire.begin();
    Wire.setClock(400000UL);
}

bool
I2cWire::busStartWrite()
{
    Wire.beginTransmission(this->address);
    return true;
}

bool
I2cWire::busStartRead(uint8_t length)
{
    Wire.requestFrom(address, length, true);
    return true;
}

bool
I2cWire::busWrite(uint8_t data)
{
    return Wire.write(data) == 1;
}

uint8_t
I2cWire::busRead()
{
    return Wire.read();
}

bool
I2cWire::busRestartWrite(uint8_t address)
{
    Wire.endTransmission(false);
    Wire.beginTransmission(address);
    return true;
}

bool
I2cWire::busRestartRead(uint8_t address,
                        uint8_t length)
{
    Wire.endTransmission(false);
    Wire.requestFrom(address, length, true);
    return true;
}

void
I2cWire::busStop(void)
{
    Wire.endTransmission();
}
