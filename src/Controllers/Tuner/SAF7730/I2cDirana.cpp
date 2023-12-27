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
#include "I2cDirana.hpp"

#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif


#define ADDR1(a) ((a) >> 16)
#define ADDR2(a) ((a) >> 8)
#define ADDR3(a) ((a) & 0xFF)


uint16_t
I2cDirana::read16(uint32_t addr)
{
    uint16_t buffer;

    this->busStartWrite();
    this->busWrite(ADDR1(addr));
    this->busWrite(ADDR2(addr));
    this->busWrite(ADDR3(addr));
    this->busRestartRead(this->address, 2);
    buffer = ((uint16_t)this->busRead() << 8);
    buffer |= this->busRead();
    this->busStop();

    return buffer;
}

uint32_t
I2cDirana::read24(uint32_t addr)
{
    uint32_t buffer;

    this->busStartWrite();
    this->busWrite(ADDR1(addr));
    this->busWrite(ADDR2(addr));
    this->busWrite(ADDR3(addr));
    this->busRestartRead(this->address, 3);
    buffer = (uint32_t)this->busRead() << 16;
    buffer |= (uint16_t)this->busRead() << 8;
    buffer |= this->busRead();
    this->busStop();

    return buffer;
}

uint32_t
I2cDirana::read32(uint32_t addr)
{
    uint32_t buffer;

    this->busStartWrite();
    this->busWrite(ADDR1(addr));
    this->busWrite(ADDR2(addr));
    this->busWrite(ADDR3(addr));
    this->busRestartRead(this->address, 4);
    buffer = (uint32_t)this->busRead() << 24;
    buffer |= (uint32_t)this->busRead() << 16;
    buffer |= (uint16_t)this->busRead() << 8;
    buffer |= this->busRead();
    this->busStop();

    return buffer;
}

void
I2cDirana::write16(uint32_t addr,
                   uint16_t data)
{
    this->busStartWrite();
    this->busWrite(ADDR1(addr));
    this->busWrite(ADDR2(addr));
    this->busWrite(ADDR3(addr));
    this->busWrite((uint8_t)(data >> 8));
    this->busWrite((uint8_t)data);
    this->busStop();
}

void
I2cDirana::write24(uint32_t addr,
                   uint32_t data)
{
    this->busStartWrite();
    this->busWrite(ADDR1(addr));
    this->busWrite(ADDR2(addr));
    this->busWrite(ADDR3(addr));
    this->busWrite((uint8_t)(data >> 16));
    this->busWrite((uint8_t)(data >> 8));
    this->busWrite((uint8_t)data);
    this->busStop();
}

void
I2cDirana::writeConfig(const uint8_t *data)
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

void
I2cDirana::writeTuner(uint32_t       addr,
                      const uint8_t *data,
                      uint8_t        dataLen)
{
    this->busStartWrite();
    this->busWrite(ADDR1(addr));
    this->busWrite(ADDR2(addr));
    this->busWrite(ADDR3(addr));
    
    this->busRestartWrite(this->tunerAddr);
    for (uint8_t i = 0; i < dataLen; i++)
    {
        this->busWrite(data[i]);
    }
    
    this->busStop();
}
