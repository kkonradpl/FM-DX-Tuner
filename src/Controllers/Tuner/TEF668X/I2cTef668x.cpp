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
#include "I2cTef668x.hpp"
#include "Lithio.hpp"

#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

void
I2cTef668x::read(uint8_t   module,
                 uint8_t   command,
                 uint8_t   length,
                 uint16_t *bufferOut)
{
    this->busStartWrite();
    this->busWrite(module);
    this->busWrite(command);
    constexpr uint8_t index = 1;
    this->busWrite(index);
    this->busStop();

    /* Add latency for 400 kHz I2C bus */
    constexpr unsigned int readLatency = 27;
    delayMicroseconds(readLatency);

    length *= 2;
    this->busStartRead(length);

    for (uint8_t i = 0; i < length; i++)
    {
        if ((i % 2) == 0)
        {
            bufferOut[i/2] = this->busRead() << 8;
        }
        else
        {
            bufferOut[i/2] |= this->busRead();
        }
    }

    this->busStop();
}

void
I2cTef668x::write(uint8_t module,
                  uint8_t command,
                  uint8_t length,
                  ...)
{
    va_list args;
    va_start(args, length);

    this->busStartWrite();
    this->busWrite(module);
    this->busWrite(command);
    constexpr uint8_t index = 1;
    this->busWrite(index);

    for (uint8_t i = 0; i < length; i++)
    {
        uint16_t buffer = va_arg(args, int);
        this->busWrite((uint8_t)(buffer >> 8));
        this->busWrite((uint8_t)buffer);
    }

    this->busStop();
    va_end(args);
}


void
I2cTef668x::writePatch(const uint8_t *data,
                       size_t         dataLen)
{
    constexpr size_t chunkLen = 24;

    while (dataLen)
    {
        this->busStartWrite();
        this->busWrite(MODULE_PATCH);

        size_t len = min(chunkLen, dataLen);
        for (size_t pos = 0; pos < len; pos++)
        {
            uint8_t buffer = pgm_read_byte_near(data++);
            this->busWrite(buffer);
        }
        
        this->busStop();
        dataLen -= len;
    }
}

void
I2cTef668x::writeConfig(const uint8_t *data)
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

