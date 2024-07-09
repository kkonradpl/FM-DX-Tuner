/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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

#ifndef FMDX_TUNER_TEF668X_H
#define FMDX_TUNER_TEF668X_H

#include "../TunerDriver.hpp"
#include "../AvgData.hpp"
#include "../../../Utils/Timer.hpp"
#include "../../../../Config.hpp"
#include "Lithio.hpp"
#include "I2cTef668x.hpp"

class TEF668X : public TunerDriver
{
public:
    void setup() override;
    bool start() override;
    void shutdown() override;
    void hello() override;
    void loop() override;

    void resetQuality() override;
    void resetRds() override;

    bool setMode(Mode value) override;
    bool setFrequency(uint32_t value, TuneFlags flags) override;
    bool setBandwidth(uint32_t value) override;
    bool setDeemphasis(uint32_t value) override;
    bool setAgc(uint32_t value) override;
    bool setAlignment(uint32_t value) override;
    bool setVolume(uint8_t value) override;
    bool setOutputMode(OutputMode value) override;
    bool setCustom(const char *name, const char *value) override;

    bool getQuality() override;
    int16_t getQualityRssi(QualityMode mode) override;
    int16_t getQualityCci(QualityMode mode) override;
    int16_t getQualityAci(QualityMode mode) override;
    int16_t getQualityModulation(QualityMode mode) override;
    int16_t getQualityOffset(QualityMode mode) override;
    int16_t getQualityBandwidth(QualityMode mode) override;
    bool getQualityStereo(QualityMode mode) override;
    
    const char* getName() override;

private:
    void setFrequencyFM(uint32_t value, TuneFlags flags);
    void setFrequencyAM(uint32_t value, TuneFlags flags);

    State readState();
    void readVersion();
    void readQuality();
    void readRds();
    bool readStereo();
    void patch();

    I2cTef668x i2c{LITHIO_I2C};
    char tuner[16];
    uint16_t version = 0;
    bool fmsi = false;
    uint8_t qualityDelay;

    static constexpr uint32_t minHfFreq = 144;
    static constexpr uint32_t maxHfFreq = 27000;
    uint32_t minVhfFreq = 65000;
    static constexpr uint32_t maxVhfFreq = 108000;

    static constexpr Timer::Interval rdsInterval = 87 / 2;
    static constexpr Timer::Interval qualityInterval = (TUNER_I2C_CLOCK >= 200000L) ? 2 : 4;
    AvgData<int16_t, int32_t, uint8_t, 100 / qualityInterval> rssi;
    AvgData<uint16_t, uint32_t, uint8_t, 300 / qualityInterval> cci;

    Timer timerQuality;
#if TUNER_TEF668X_RDS_DAVN == false
    Timer timerRds;
#endif
    Timer timerMute;
    Timer timerUnmute;
};

#endif
