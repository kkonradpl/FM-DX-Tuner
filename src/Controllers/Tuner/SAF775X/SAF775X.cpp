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
#include "SAF775X.hpp"
#include "Dirana3.hpp"
#include "../Common/Bandwidth.hpp"
#include "../../../../ConfigSAF775X.hpp"

void
SAF775X::setup()
{
    pinMode(TUNER_SAF775X_PIN_RESET, INPUT);
}

bool
SAF775X::start()
{
    digitalWrite(TUNER_SAF775X_PIN_RESET, LOW);
    pinMode(TUNER_SAF775X_PIN_RESET, OUTPUT);
    delay(50);
    digitalWrite(TUNER_SAF775X_PIN_RESET, HIGH);
    pinMode(TUNER_SAF775X_PIN_RESET, INPUT);
    delay(100);

    i2c.writeConfig(Dirana3::BOOT);
    delay(50);

    i2c.write(Dirana3::Write_Start, 0);
    delay(300);

    i2c.writeConfig(Dirana3::INIT);
    delay(100);

    i2c.writeConfig(Dirana3::FM);
    delay(100);

    this->mode = MODE_FM;
    this->timerQuality.set(this->qualityInterval);
    this->timerRds.set(this->rdsInterval);

    return true;
}

void
SAF775X::shutdown()
{
    i2c.write(Dirana3::Write_Radio_Tune, 1, 0x00);
    this->timerQuality.unset();
    this->timerRds.unset();
}

void
SAF775X::hello()
{
}

void
SAF775X::loop()
{
    if (this->timerQuality.process(Timer::Continous))
    {
        this->readQuality();
    }

    if (mode == MODE_FM &&
        this->timerRds.process(Timer::Continous))
    {
        this->readRds();
    }
}

void
SAF775X::resetQuality()
{
    this->rssi.reset();
    this->cci.reset();
}

void
SAF775X::resetRds()
{
    i2c.write(Dirana3::Write_Radio_RDS, 1, 0x81);
}

bool
SAF775X::setMode(Mode value)
{
    return false;
}

bool
SAF775X::setFrequency(uint32_t  value,
                      TuneFlags flags)
{
    uint8_t tunerMode;
    uint16_t tunerFrequency;

    if (value > 0 &&
        value <= 27400)
    {
        tunerFrequency = value;
        tunerMode = (1 << 4);

        if (value < 521)
        {
             tunerMode |= B001;
        }
        else if (value >= 522 &&
                 value <= 2299)
        {
            tunerMode |= B010;
        }
        else if (value >= 2300)
        {
            tunerMode |= B011;
        }

        this->mode = MODE_AM;
    }
    else if (value >= 60070 &&
             value <= 108500)
    {
        tunerFrequency = value / 10;
        tunerMode = (this->mode != MODE_FM) ? (1 << 4) : (B100 << 4);
        this->mode = MODE_FM;
    }
    else
    {
        return false;
    }

    i2c.write(Dirana3::Write_Radio_Tune, 3,
              tunerMode,
              (tunerFrequency >> 8),
              (uint8_t)tunerFrequency);

    this->frequency = value;
    return true;
}

bool
SAF775X::setBandwidth(uint32_t value)
{
    if (this->mode != MODE_FM &&
        this->mode != MODE_AM)
    {
        return false;
    }

    if (value == 0)
    {
        i2c.write(Dirana3::Write_Radio_Bandwidth, 1, 0);
        this->bandwidth = 0;
        return true;
    }
    else
    {
        uint8_t id;
        const uint16_t *table = (this->mode == MODE_FM) ? Dirana3::BANDWIDTH_FM : Dirana3::BANDWIDTH_AM;
        uint16_t bw = Bandwidth::lookup(table, value / 100, &id);

        if (bw)
        {
            i2c.write(Dirana3::Write_Radio_Bandwidth, 1, id + 1);
            this->bandwidth = (uint32_t)bw * 100;
            return true;
        }
    }

    return false;
}

bool
SAF775X::setDeemphasis(uint32_t value)
{
    return false;
}

bool
SAF775X::setAgc(uint32_t value)
{
    return false;
}

bool
SAF775X::setAlignment(uint32_t value)
{
    return false;
}

bool
SAF775X::setVolume(uint8_t value)
{
    uint16_t volume = map(value, 0, 100, 0, 0x7FF);
    
    i2c.write(Dirana3::Write_Audio, 6,
              0x40, 0x84,
              volume >> 8,
              (uint8_t)volume,
              volume >> 8,
              (uint8_t)volume);

    return true;
}

bool
SAF775X::setOutputMode(OutputMode value)
{
    return false;
}

bool
SAF775X::setCustom(const char *name,
                   const char *value)
{
    uint8_t ceq = (value[0] == '1');
    uint8_t ims = (value[1] == '1');

    i2c.write(Dirana3::Write_Radio_Options, 1, (ims << 7) | (ceq << 5));
    return true;
}

bool
SAF775X::getQuality()
{
    return this->rssi.isAvailable();
}

int16_t
SAF775X::getQualityRssi(QualityMode mode)
{
    if (!this->rssi.isAvailable())
    {
        return 0;
    }

    const bool fast = (mode == TunerDriver::QUALITY_FAST);
    const int16_t value = fast ? this->rssi.getRaw() : this->rssi.getAvg();

    /* [dBf] = [dBµV] - 10 * log10(75) + 30
       [dBf] = [dBµV] + 11.25
       TODO: Use dBµV as main unit */
    constexpr int16_t dbfOffset = 1125;

    return value + dbfOffset;
}

int16_t
SAF775X::getQualityCci(QualityMode mode)
{
    if (!this->cci.isAvailable())
    {
        return -1;
    }

    constexpr int16_t wamMin = 8;
    constexpr int16_t wamMax = 92;
    constexpr int16_t cciMin = 0;
    constexpr int16_t cciMax = 100;

    const bool fast = (mode == TunerDriver::QUALITY_FAST);
    const int16_t value = fast ? this->cci.getRaw() : this->cci.getAvg();
    return map(constrain(value, wamMin, wamMax), wamMin, wamMax, cciMin, cciMax);
}

int16_t
SAF775X::getQualityAci(QualityMode mode)
{
    return -1;
}

int16_t
SAF775X::getQualityModulation(QualityMode mode)
{
    return -1;
}

int16_t
SAF775X::getQualityOffset(QualityMode mode)
{
    return -1;
}

int16_t
SAF775X::getQualityBandwidth(QualityMode mode)
{
    return -1;
}

bool
SAF775X::getQualityStereo(QualityMode mode)
{
    return (this->mode == MODE_FM && this->pilot);
}

const char*
SAF775X::getName()
{
    return this->tuner;
}

void
SAF775X::readQuality()
{
    uint8_t status;
    i2c.read(Dirana3::Read_Radio_Quality_Status, 1, &status);

    const uint16_t timestamp = (status & 0xE0) >> 5;
    if (timestamp >= B010)
    {
        Dirana3::QualityData data;
        i2c.read(Dirana3::Read_Radio_Quality_Data,
                sizeof(Dirana3::QualityData) / sizeof(uint8_t),
                (uint8_t*)&data);

        this->rssi.add(map(data.level, 0x00, 0xD7, -800, 9950));
        this->cci.add(data.coChannel);
        this->pilot = status & B00001000;
    }
}

void
SAF775X::readRds()
{
    uint8_t status;
    i2c.read(Dirana3::Read_Radio_RDS_Status, 1, &status);

    if ((status & B10000000) == 0)
    {
        /* Data available flag == 0 */
        return;
    }

    if ((status & B00100000))
    {
        /* Ignore first PI data, will be repeated in continuous operation */
        return;
    }

    Dirana3::RdsData data; 
    i2c.read(Dirana3::Read_Radio_RDS_Data,
             sizeof(Dirana3::RdsData) / sizeof(uint8_t),
             (uint8_t*)&data);

    uint16_t blockA = ((uint16_t)data.blockA_H << 8) | data.blockA_L;
    uint16_t blockB = ((uint16_t)data.blockB_H << 8) | data.blockB_L;
    uint16_t blockC = ((uint16_t)data.blockC_H << 8) | data.blockC_L;
    uint16_t blockD = ((uint16_t)data.blockD_H << 8) | data.blockD_L;

    this->piBuffer.add(blockA, data.errors & B11000000);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_A, blockA, (data.errors & B11000000) >> 6);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_B, blockB, (data.errors & B110000) >> 4);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_C, blockC, (data.errors & B1100) >> 2);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_D, blockD, (data.errors & B11));
}
