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

#include <Arduino.h>
#include "SAF7730.hpp"
#include "Alignment.hpp"
#include "../Common/Bandwidth.hpp"
#include "../../../Utils/Utils.hpp"
#include "../../../../ConfigSAF7730.hpp"

void
SAF7730::setup()
{
    pinMode(TUNER_SAF7730_PIN_RDS, INPUT);
    pinMode(TUNER_SAF7730_PIN_POWER, OUTPUT);
    digitalWrite(TUNER_SAF7730_PIN_POWER, LOW);
    pinMode(TUNER_SAF7730_PIN_RESET, OUTPUT);
    digitalWrite(TUNER_SAF7730_PIN_RESET, LOW);
    i2c.init();
}

bool
SAF7730::start()
{
    digitalWrite(TUNER_SAF7730_PIN_POWER, HIGH);
    delay(100);
    digitalWrite(TUNER_SAF7730_PIN_RESET, LOW);
    delay(5);
    digitalWrite(TUNER_SAF7730_PIN_RESET, HIGH);
    delay(100);

    this->readVersion();
    i2c.writeConfig(DIRANA_INIT);
    return true;
}

void
SAF7730::shutdown()
{
}

void
SAF7730::hello()
{
}

void
SAF7730::loop()
{
    if (timerQualityAvailable.process(Timer::Oneshot))
    {
        this->qualityReady = true;
    }

    if (timerQualityReset.process(Timer::Oneshot))
    {
        this->forceFastDetector = false;
    }

    if (digitalRead(TUNER_SAF7730_PIN_RDS) == LOW)
    {
        this->readRds();
    }
}

void
SAF7730::resetQuality()
{
    this->timerQualityReset.set(250);
    this->forceFastDetector = true;
}

void
SAF7730::resetRds()
{
    constexpr uint16_t mode = 0x0060; /* Fast PI mode */
    i2c.write16(DIRANA_RDS_CONTROL, mode);
}

bool
SAF7730::setMode(Mode value)
{
    if (value == MODE_FM)
    {
        i2c.writeConfig(DIRANA_FM);
    }
    else if (value == MODE_AM)
    {
        i2c.writeConfig(DIRANA_AM);
    }
    else
    {
        return false;
    }

    tef6730.tune();
    this->mode = value;
    return true;
}

bool
SAF7730::setFrequency(uint32_t  value,
                      TuneFlags flags)
{
    uint32_t newFrequency = tef6730.setFrequency(value, flags & TUNE_SCAN);

    if (newFrequency == 0)
    {
        return false;
    }

    uint8_t band = tef6730.getBand();
    tef6730.setAlignment(Alignment::get(newFrequency));
    tef6730.tune();

    const bool bandChanged = band != tef6730.getBand();
    const Timer::Interval qualityDelay = bandChanged ? 8 : 3;
    this->timerQualityAvailable.set(qualityDelay);
    this->qualityReady = false;
    this->frequency = newFrequency;

    return true;
}

bool
SAF7730::setBandwidth(uint32_t value)
{
    if (this->mode != MODE_FM && 
        this->mode != MODE_AM)
    {
        return false;
    }

    const bool adaptive = (value == 0);
    if (adaptive)
    {
        if (mode == MODE_FM)
        {
            this->filter.setAdaptive();
            this->bandwidth = 0;
            return true;
        }

        return false;
    }

    uint8_t id;
    const uint16_t *table = (this->mode == MODE_FM) ? DIRANA_BANDWIDTH_FM : DIRANA_BANDWIDTH_AM;
    uint32_t bandwidth = Bandwidth::lookup(table, value / 100, &id) * 100;
    if (bandwidth)
    {
        this->filter.setFixed(this->mode, id);
        this->bandwidth = bandwidth;
        return true;
    }

    return false;
}

bool
SAF7730::setDeemphasis(uint32_t value)
{
    switch (value)
    {
        //case 50:
        case 0:
            i2c.writeConfig(DIRANA_DEEMPHASIS_50US);
            break;

        //case 75:
        case 1:
            i2c.writeConfig(DIRANA_DEEMPHASIS_75US);
            break;

        //case 0:
        case 2:
            i2c.writeConfig(DIRANA_DEEMPHASIS_0US);
            break;

        default:
            return false;
    }

    return true;
}

bool
SAF7730::setAgc(uint32_t value)
{
    if (value <= 3)
    {
        tef6730.setAgc(value);
        tef6730.tune();
        return true;
    }

    return false;
}

bool
SAF7730::setAlignment(uint32_t value)
{
    if (value <= 0x7F)
    {
        tef6730.setAlignment(value);
        tef6730.tune();
        return true;
    }

    return false;
}

bool
SAF7730::setVolume(uint8_t value)
{
    uint16_t volume = (uint16_t)((exp(value / 100.0) - 1) / (M_E - 1) * DIRANA_MAX_VOLUME);
    i2c.write16(DIRANA_VOLUME_SCALER, volume);
    return true;
}

bool
SAF7730::setOutputMode(OutputMode value)
{
    switch (value)
    {
        case OUTPUT_MODE_STEREO:
            i2c.write16(DIRANA_FORCE_MONO, DIRANA_DSP_FALSE);
            return true;

        case OUTPUT_MODE_MONO:
            i2c.write16(DIRANA_FORCE_MONO, DIRANA_DSP_TRUE);
            return true;

        default:
            return false;
    }
}

bool
SAF7730::setCustom(const char *name,
                   const char *value)
{
    /* TODO: New protocol */
    if (strlen(value) == 2)
    {
        tef6730.setRfBoost(value[0] == '1');
        tef6730.setIfBoost(value[1] == '1');
        tef6730.tune();
        /* TODO: reset quality */
        return true;
    }

    return false;
}

bool
SAF7730::getQuality()
{
    return this->qualityReady;
}

int16_t
SAF7730::getQualityRssi(QualityMode mode)
{
    uint32_t addr;

    if (this->mode == MODE_FM)
    {
        const bool fast = this->forceFastDetector || (mode == TunerDriver::QUALITY_FAST);
        addr = (fast ? DIRANA_FM_LEVEL_FAST : DIRANA_FM_LEVEL);
    }
    else
    {
        addr = DIRANA_AM_LEVEL;
    }

    uint32_t buffer = i2c.read24(addr);
    float value = (buffer & 0xFF0000) >> 16;
    value += (buffer & 0xFFFF) / 65536.0;

    if (this->mode == MODE_FM)
    {
        value = value * 0.797 + 3.5 + TUNER_SAF7730_FM_LEVEL_OFFSET;
    }
    else
    {
        value = value + TUNER_SAF7730_AM_LEVEL_OFFSET;
    }

    this->lastRssi = (int16_t)(value * 100);
    return this->lastRssi;
}

int16_t
SAF7730::getQualityCci(QualityMode mode)
{
    if (this->mode != MODE_FM)
    {
        return -1;
    }

    /*  Unfortunately, the multipath detector slightly changes
     *  its output depending on a current signal level. To
     *  overcome this and provide a relative output that
     *  always starts at 0%, a linear correction of usable
     *  detector segment range has been estimated.
     *  For 83 dBf signal, the detector range is ~13-51% FS
     *  For 29 dBf signal, the detector range is ~ 8-46% FS
     *  Full scale is 0x000000-0x7FFFFF. */

    uint32_t multipathMin = 7723 * (uint32_t)(this->lastRssi / 100) + 450000;
    uint32_t multipathMax = multipathMin + 3187670; /* 38% of FS */
    uint32_t output = i2c.read24(DIRANA_FM_MULTIPATH);

    output = constrain(output, multipathMin, multipathMax);
    output = map(output, multipathMin, multipathMax, 0, 100);
    return output;
}

int16_t
SAF7730::getQualityAci(QualityMode mode)
{
    uint32_t output;

    if (this->mode != MODE_FM ||
        this->bandwidth != 0)
    {
        this->lastAciRaw = -1;
        return -1;
    }

    output = i2c.read24(DIRANA_ULTRASONIC_NOISE);
    if (this->lastAciRaw != -1)
    {
        output += this->lastAciRaw;
        output /= 2;
    }

    this->lastAciRaw = output;
    static constexpr uint32_t usnMin = 251658;  /* 0.03 of FS */
    static constexpr uint32_t usnMax = 3355442; /* 0.40 of FS */
    output = constrain(output, usnMin, usnMax);
    output = map(output, usnMin, usnMax, 0, 100);
    return output;
}

int16_t
SAF7730::getQualityModulation(QualityMode mode)
{
    /* Unimplemented */
    return -1;
}

int16_t
SAF7730::getQualityOffset(QualityMode mode)
{
    /* Unimplemented */
    return -1;
}

int16_t
SAF7730::getQualityBandwidth(QualityMode mode)
{
    /* Unimplemented */
    return -1;
}

bool
SAF7730::getQualityStereo(QualityMode mode)
{
    if (this->mode == MODE_FM)
    {
        const uint16_t pilot = i2c.read16(DIRANA_PILOT);
        return (pilot == DIRANA_DSP_TRUE);
    }

    return false;
}

const char*
SAF7730::getName()
{
    return this->tuner;
}

void
SAF7730::readVersion()
{
    const uint16_t id = i2c.read32(DIRANA_IDENTIFICATION);
    const uint8_t hwVersion = (uint8_t)((id >> 12) & 0x0F);
    const uint8_t swVersion = (uint8_t)(id & 0x7F);

    const uint16_t version = ((uint16_t)hwVersion * 100) + swVersion;
    snprintf(this->tuner, sizeof(this->tuner),
             "SAF7730/V%d", version);
}

void
SAF7730::readRds()
{
    uint8_t status = i2c.read16(DIRANA_RDS_STATUS);
    uint16_t buffer = i2c.read16(DIRANA_RDS_DATA);
    uint8_t error = status & B11;

    switch (status & B11111100)
    {
        case 0x80: /* Block A */
            this->rdsBuffer.set(RdsGroupBuffer::BLOCK_A, buffer, error);
        case 0x00: /* Fast PI mode */
            this->piBuffer.add(buffer, status != 0x80);
            break;

        case 0x84: /* Block B */
            this->rdsBuffer.set(RdsGroupBuffer::BLOCK_B, buffer, error);
            break;

        case 0x88: /* Block C */
        case 0x90: /* Block C' */
            this->rdsBuffer.set(RdsGroupBuffer::BLOCK_C, buffer, error);
            /* Some RDS encoders do not use C' for PI code, do not use rdsPiBuffer */
            break;

        case 0x8C: /* Block D */
            this->rdsBuffer.set(RdsGroupBuffer::BLOCK_D, buffer, error);
            break;
    }
}
