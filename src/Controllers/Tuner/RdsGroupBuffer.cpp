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
#include "RdsGroupBuffer.hpp"

static constexpr float RDS_DATA_RATE = 1187.5; /* bps */
static constexpr uint8_t RDS_GROUP_SIZE = 104; /* b */
static constexpr uint8_t RDS_BLOCK_DURATION = (RDS_GROUP_SIZE / RDS_DATA_RATE * 1000 / 4) + 1;


RdsGroupBuffer::RdsGroupBuffer()
{
    this->clear();
}

void
RdsGroupBuffer::set(Block    blockId,
                    uint16_t value,
                    uint8_t  error)
{
    if ((millis() - this->lastClear) < RDS_BLOCK_DURATION)
    {
        /* Ignore block from previous sync */
        return;
    }

    for (Block i = BLOCK_A; i < blockId; i = (Block)(i + 1))
    {
        if ((millis() - lastBlockTime[i]) > (blockId + 1 - i) * RDS_BLOCK_DURATION)
        {
            /* Invalidate previous blocks, if timeout is reached */
            this->invalidate(i);
        }
    }

    this->rdsData[blockId] = value;
    this->rdsError &= ~(B11 << (6 - 2 * blockId));
    this->rdsError |= error << (6 - 2 * blockId);

    for (Block i = (Block)(blockId + 1); i <= BLOCK_D; i = (Block)(i + 1))
    {
        /* Invalidate next blocks */
        this->invalidate(i);
    }

    this->lastBlock = blockId;
    this->lastBlockTime[blockId] = millis();
}

bool
RdsGroupBuffer::get(uint16_t **outData,
                    uint8_t   *outStatus)
{
    if (lastBlock == BLOCK_NONE)
    {
        return false;
    }

    /* If RDS_BLOCK_D was the last block - the data is available.
     * If no RDS_BLOCK_D was received, check the last received block and
     * if timeout was reached, also flush available data. */

    if ((lastBlock == BLOCK_D) ||
        ((millis() - lastBlockTime[lastBlock]) > (BLOCK_D - lastBlock + 1) * RDS_BLOCK_DURATION))
    {
        lastBlock = BLOCK_NONE;
        *outData = rdsData;
        *outStatus = rdsError;
        return true;
    }

    return false;
}

void
RdsGroupBuffer::clear()
{
    this->lastBlock = BLOCK_NONE;
    this->lastClear = millis();
}

void
RdsGroupBuffer::invalidate(Block blockId)
{
    this->rdsData[blockId] = 0;
    this->rdsError |= (B11 << (6 - 2 * blockId));
}
