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

#ifndef FMDX_RADIO_I2C_AVR_H
#define FMDX_RADIO_I2C_AVR_H
#ifdef ARDUINO_ARCH_AVR

#include "I2cMaster/I2cMaster.h"

class I2cAvr
{
public:
    I2cAvr(uint8_t _address) : address(_address) {};
    
    bool busStartWrite(void);
    bool busStartRead(uint8_t length);

    bool busWrite(uint8_t data);
    uint8_t busRead(void);

    bool busRestartWrite(uint8_t address);
    bool busRestartRead(uint8_t address, uint8_t length);

    void busStop(void);

private:
    const uint8_t address;
    TwiMaster bus{true};
    uint8_t readLength;
};

#endif
#endif
