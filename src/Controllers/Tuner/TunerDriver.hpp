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

#ifndef FMDX_TUNER_TUNERDRIVER_H
#define FMDX_TUNER_TUNERDRIVER_H

#include "../../Protocol.h"
#include "RdsGroupBuffer.hpp"
#include "RdsPiBuffer.hpp"

class TunerDriver
{
public:
    enum TuneFlags : uint8_t
    {
        TUNE_DEFAULT = 0,
        TUNE_SCAN = 1 << 0
    };

    enum QualityMode : uint8_t
    {
        QUALITY_DEFAULT = 0,
        QUALITY_FAST = 1
    };

    virtual void setup() = 0;
    virtual bool start() = 0;
    virtual void shutdown() = 0;
    virtual void hello() = 0;
    virtual void loop() = 0;

    virtual void resetQuality() = 0;
    virtual void resetRds() = 0;

    virtual bool setMode(Mode value) = 0;
    virtual bool setFrequency(uint32_t value, TuneFlags flags) = 0;
    virtual bool setBandwidth(uint32_t value) = 0;
    virtual bool setDeemphasis(uint32_t value) = 0;
    virtual bool setAgc(uint32_t value) = 0;
    virtual bool setAlignment(uint32_t value) = 0;
    virtual bool setVolume(uint8_t value) = 0;
    virtual bool setOutputMode(OutputMode value) = 0;
    virtual bool setCustom(const char *name, const char *value) = 0;

    virtual int16_t getQualityRssi(QualityMode mode) = 0;
    virtual int16_t getQualityCci(QualityMode mode) = 0;
    virtual int16_t getQualityAci(QualityMode mode) = 0;
    virtual int16_t getQualityModulation(QualityMode mode) = 0;
    virtual int16_t getQualityOffset(QualityMode mode) = 0;
    virtual int16_t getQualityBandwidth(QualityMode mode) = 0;
    virtual bool getQualityStereo(QualityMode mode) = 0;
    
    Mode getMode();
    uint32_t getFrequency();
    uint32_t getStep();
    uint32_t getBandwidth();

    RdsGroupBuffer rdsBuffer;
    RdsPiBuffer piBuffer;

protected:
    Mode mode = MODE_NONE;
    uint32_t frequency = 0;
    uint32_t step = 0;
    uint32_t bandwidth = 0;
};

#endif
