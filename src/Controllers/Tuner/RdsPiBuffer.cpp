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
#include "RdsPiBuffer.hpp"

RdsPiBuffer::RdsPiBuffer()
{
    this->clear();
}

void
RdsPiBuffer::add(uint16_t value,
                 bool     error)
{
    this->pos = (this->pos + 1) % BUFF_SIZE;
    this->buff[this->pos] = value;
    
    const uint8_t errorPos = this->pos / 8;
    const uint8_t errorBitPos = this->pos % 8;
    
    if (error)
    {
        this->errorBuff[errorPos] |= (1 << errorBitPos);
    }
    else
    {
        this->errorBuff[errorPos] &= ~(1 << errorBitPos);
    }
        
    if (this->fill < BUFF_SIZE)
    {
        this->fill++;
    }

    this->lastState = this->getState(value);
}

RdsPiBuffer::State
RdsPiBuffer::get(uint16_t &valueOut)
{
    if (this->lastState == STATE_INVALID)
    {
        return STATE_INVALID;
    }

    State ret = this->lastState;
    this->lastState = STATE_INVALID;
    valueOut = this->buff[this->pos];

    return ret;
}

void
RdsPiBuffer::clear()
{
    this->fill = 0;
    this->pos = (uint8_t)-1;
    this->lastState = STATE_INVALID;
}

RdsPiBuffer::State
RdsPiBuffer::getState(uint16_t value)
{
    uint8_t count = 0;
    uint8_t correctCount = 0;

    for (uint8_t i = 0; i < this->fill; i++)
    {
        if (this->buff[i] == value)
        {
            count++;
            if ((this->errorBuff[i / 8] & (1 << (i % 8))) == 0)
            {
                correctCount++;
            }
        }
    }

    if (correctCount >= 2)
    {
        return STATE_CORRECT;
    }
    else if (count >= 2 && correctCount)
    {
        return STATE_VERY_LIKELY;
    }
    else if (count >= 3)
    {
        return STATE_LIKELY;
    }
    else if (count == 2 || correctCount)
    {
        return STATE_UNLIKELY;
    }
    
    return STATE_INVALID;
}