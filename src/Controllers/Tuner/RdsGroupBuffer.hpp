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

#ifndef FMDX_TUNER_RDSGROUPBUFFER_H
#define FMDX_TUNER_RDSGROUPBUFFER_H

class RdsGroupBuffer
{
public:
    enum Block : uint8_t
    {
        BLOCK_A = 0,
        BLOCK_B = 1,
        BLOCK_C = 2,
        BLOCK_D = 3,
        BLOCK_NONE
    };

    RdsGroupBuffer();
    void set(Block blockId, uint16_t value, uint8_t error);
    bool get(uint16_t**, uint8_t*);
    void clear();
  
private:
    uint16_t rdsData[4];
    uint8_t rdsError;
    uint8_t lastBlock;
    uint32_t lastBlockTime[4];
    uint32_t lastClear;
    
    void invalidate(Block blockId);
};

#endif
