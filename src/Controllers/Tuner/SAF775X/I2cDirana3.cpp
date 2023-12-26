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
#include <stdarg.h>
#include "I2cDirana3.hpp"
#include "Dirana3.hpp"

#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

void
I2cDirana3::read(uint8_t  command,
                 uint8_t  length,
                 uint8_t *bufferOut)
{
    this->busStartWrite();
    this->busWrite(command);
    this->busStop();

    this->busStartRead(length);
    for (uint8_t i = 0; i < length; i++)
    {
        bufferOut[i] = this->busRead();
    }
    this->busStop();
}

void
I2cDirana3::write(uint8_t command,
                  uint8_t length,
                  ...)
{
    va_list args;
    va_start(args, length);

    this->busStartWrite();
    this->busWrite(command);

    for (uint8_t i = 0; i < length; i++)
    {
        uint16_t buffer = va_arg(args, int);
        this->busWrite((uint8_t)buffer);
    }

    this->busStop();
    va_end(args);
}

void
I2cDirana3::writeConfig(const uint8_t *data)
{
    uint8_t len;
    
    while ((len = pgm_read_byte_near(data++)))
    {
        this->busStartWrite();

        while (len--)
        {
            uint8_t buffer = pgm_read_byte_near(data++);
            this->busWrite(buffer);
        }

        this->busStop();
    }
}

