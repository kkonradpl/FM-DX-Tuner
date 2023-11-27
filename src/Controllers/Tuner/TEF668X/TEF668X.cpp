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
#include "TEF668X.hpp"
#include "Lithio.hpp"
#include "Lithio_V101_p119.hpp"
#include "Lithio_V102_p224.hpp"
#include "Lithio_V205_p512.hpp"
#include "../../../Utils/Utils.hpp"
#include "../../../../Config.hpp"

void
TEF668X::setup()
{
}

bool
TEF668X::start()
{
    delay(5);

    if (this->readState() == STATE_BOOT)
    {
        i2c.writeConfig(LITHIO_START);
        delay(50);
    }

    this->readVersion();
    Serial.println(this->tuner);

    i2c.writeConfig(LITHIO_RESET);
    delay(5);

    this->patch();

    i2c.writeConfig(LITHIO_START);
    delay(50);

    if (this->readState() == STATE_IDLE)
    {
#if TUNER_TEF668X_REF_CLOCK == 0
        const bool clockSelect = digitalRead(TUNER_TEF668X_PIN_REF_CLOCK);
        const uint32_t refClock = clockSelect ? 4000000 : 9216000;
#else
        const uint32_t refClock = TUNER_TEF668X_REF_CLOCK;
#endif
        i2c.write(MODULE_APPL, APPL_Set_ReferenceClock, 3,
                  (uint16_t)((refClock) >> 16),
                  (uint16_t)((refClock)),
                  (uint16_t)0x0000);

        i2c.write(MODULE_APPL, APPL_Activate, 1, 0x0001);
        delay(100);
    }

    if (this->readState() >= STATE_ACTIVE)
    {
        i2c.writeConfig(LITHIO_CONFIG);
        i2c.writeConfig(this->fmsi ? LITHIO_CONFIG_FMSI : LITHIO_CONFIG_STEREO);

        i2c.write(MODULE_APPL, APPL_Set_OperationMode, 1, 0);
        delay(32);

        /* The delay of 16 ms seems to be sufifcent for 6687/V205, but
           longer delay can improve compability with untested versions. */

        return true;
    }
    
    return false;
}

void
TEF668X::shutdown()
{
    i2c.write(MODULE_APPL, APPL_Set_OperationMode, 1, 1);
}

void
TEF668X::hello()
{
}

void
TEF668X::loop()
{
    if (this->timerQuality.check())
    {
        this->readQuality();
    }

    if (mode == MODE_FM &&
        this->timerRds.check())
    {
        this->readRds();
    }
}

void
TEF668X::resetQuality()
{
    this->rssi.reset();
    this->cci.reset();
}

void
TEF668X::resetRds()
{
    const uint16_t mode = (this->fmsi ? 3 : 1);
    constexpr uint16_t restart = 1;
    constexpr uint16_t interface = 0;
    i2c.write(MODULE_FM, FM_Set_RDS, 3, mode, restart, interface);
}

bool
TEF668X::setMode(Mode value)
{
    constexpr uint16_t modePreset = 1;
 
    switch (value)
    {
        case MODE_FM:
            if (this->frequency < this->minVhfFreq ||
                this->frequency > this->maxVhfFreq)
            {
                this->frequency = this->minVhfFreq;
            }

            this->step = 10;
            i2c.write(MODULE_FM, FM_Tune_To, 2, modePreset, this->frequency / 10);
            break;

        case MODE_AM:
            if (this->frequency < this->minHfFreq ||
                this->frequency > this->maxHfFreq)
            {
                this->frequency = this->minHfFreq;
            }

            this->step = 1;
            i2c.write(MODULE_AM, AM_Tune_To, 2, modePreset, this->frequency);
            break;

        default:
            return false;
    }

    this->mode = value;
    return true;
}

bool
TEF668X::setFrequency(uint32_t  value,
                      TuneFlags flags)
{
    constexpr uint16_t modePreset = 1;
    constexpr uint16_t modeJump = 4;
    const bool scanFlag = flags & TUNE_SCAN;

    if (value >= this->minVhfFreq &&
        value <= this->maxVhfFreq)
    {
        this->frequency = value / 10 * 10;

        if (this->mode != MODE_FM)
        {
            this->setMode(MODE_FM);
            return true;
        }
        
        if (!scanFlag)
        {
            i2c.write(MODULE_AUDIO, AUDIO_Set_Mute, 1, 1);
        }

        i2c.write(MODULE_FM, FM_Tune_To, 2, modeJump, this->frequency / 10);
        
        delay(5);

        if (!scanFlag)
        {
            delay(3);
            i2c.write(MODULE_AUDIO, AUDIO_Set_Mute, 1, 0);
        }
        
        return true;
    }

    if (value >= this->minHfFreq &&
        value <= this->maxHfFreq)
    {
        this->frequency = value;

        if (this->mode != MODE_AM)
        {
            this->setMode(MODE_AM);
            return true;
        }
        
        i2c.write(MODULE_AM, AM_Tune_To, 2, modePreset, this->frequency);
        return true;
    }

    return false;
}

bool
TEF668X::setBandwidth(uint32_t value)
{
    if (this->mode == MODE_FM)
    {
        const uint16_t mode = (value == 0) ? 1 : 0;
        i2c.write(MODULE_FM, FM_Set_Bandwidth, 4, mode, value / 100, 100, 100);
        this->bandwidth = value;
        return true;
    }
    
    if (this->mode == MODE_AM)
    {
        const uint16_t mode = 0;
        i2c.write(MODULE_AM, AM_Set_Bandwidth, 4, mode, value / 100, 100, 100);
        this->bandwidth = value;
        return true;
    }

    return false;
}

bool
TEF668X::setDeemphasis(uint32_t value)
{
    switch (value)
    {
        //case 50:
        case 0:
            i2c.write(MODULE_FM, FM_Set_Deemphasis, 1, 500);
            return true;

        //case 75:
        case 1:
            i2c.write(MODULE_FM, FM_Set_Deemphasis, 1, 750);
            return true;

        //case 0:
        case 2:
            i2c.write(MODULE_FM, FM_Set_Deemphasis, 1, 0);
            return true;
    }

    return false;
}

bool
TEF668X::setAgc(uint32_t value)
{
    if (this->mode == MODE_FM)
    {
        constexpr uint16_t minAgc = 840;
        constexpr uint16_t maxAgc = 920;
        
        const uint16_t agc = map(value, 0, 3, maxAgc, minAgc);
        i2c.write(MODULE_FM, FM_Set_RFAGC, 2, agc, 0);
        return true;
    }

    if (this->mode == MODE_AM)
    {
        constexpr uint16_t minAgc = 940;
        constexpr uint16_t maxAgc = 1020;

        const uint16_t agc = map(value, 0, 3, maxAgc, minAgc);
        i2c.write(MODULE_AM, AM_Set_RFAGC, 1, agc);
        return true;
    }

    return false;
}

bool
TEF668X::setAlignment(uint32_t value)
{
    return false;
}

bool
TEF668X::setVolume(uint8_t value)
{
    int16_t volume = map(value, 0, 100, -600, 0);
    i2c.write(MODULE_AUDIO, AUDIO_Set_Volume, 1, volume);
    return true;
}

bool
TEF668X::setOutputMode(OutputMode value)
{
    switch (value)
    {
        case OUTPUT_MODE_STEREO:
            i2c.write(MODULE_FM, FM_Set_Specials, 1, 0);
            //i2c.write(MODULE_FM, FM_Set_Stereo_Min, 2, 0, 400);
            return true;

        case OUTPUT_MODE_MONO:
            i2c.write(MODULE_FM, FM_Set_Specials, 1, 0);
            //i2c.write(MODULE_FM, FM_Set_Stereo_Min, 2, 2, 400);
            return true;

        case OUTPUT_MODE_MPX:
            i2c.write(MODULE_FM, FM_Set_Specials, 1, 1);
            return true;

        default:
            return false;
    }
}

bool
TEF668X::setCustom(const char *name,
                   const char *value)
{
    i2c.write(MODULE_FM, FM_Set_ChannelEqualizer, 1, (value[0] == '1'));
    i2c.write(MODULE_FM, FM_Set_MphSuppression, 1, (value[1] == '1'));
    /* TODO */
    return true;
}

int16_t
TEF668X::getQualityRssi(QualityMode mode)
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
TEF668X::getQualityCci(QualityMode mode)
{
    if (!this->cci.isAvailable())
    {
        return -1;
    }

    constexpr int16_t wamMin = 32;
    constexpr int16_t wamMax = 360;
    constexpr int16_t cciMin = 0;
    constexpr int16_t cciMax = 100;

    const bool fast = (mode == TunerDriver::QUALITY_FAST);
    const int16_t value = fast ? this->cci.getRaw() : this->cci.getAvg();
    return map(constrain(value, wamMin, wamMax), wamMin, wamMax, cciMin, cciMax);
}

int16_t
TEF668X::getQualityAci(QualityMode mode)
{
    /* USN detector is unreliable for ACI meter */
    return -1;
}

int16_t
TEF668X::getQualityModulation(QualityMode mode)
{
    /* TODO */
    return -1;
}

int16_t
TEF668X::getQualityOffset(QualityMode mode)
{
    /* TODO */
    return -1;
}

int16_t
TEF668X::getQualityBandwidth(QualityMode mode)
{
    /* TODO */
    return -1;
}

bool
TEF668X::getQualityStereo(QualityMode mode)
{
    if (this->mode == MODE_FM)
    {
        return this->readStereo();
    }

    return false;
}

State
TEF668X::readState()
{
    uint16_t state;
    i2c.read(MODULE_APPL, APPL_Get_Operation_Status, 1, &state);
    return (State)state;
}

void
TEF668X::readVersion()
{
    IdData data;
    i2c.read(MODULE_APPL, APPL_Get_Identification,
             sizeof(IdData) / sizeof(uint16_t),
             (uint16_t*)&data);

    const uint8_t type = (data.device >> 8);
    const uint8_t variant = (uint8_t)data.device;
    const uint8_t versionHwMajor = (uint8_t)(data.versionHw >> 8);
    const uint8_t versionSwMajor = (uint8_t)(data.versionSw >> 8);

    this->version = 0;
    this->fmsi = false;

    char id = 'X';
    if (type == 9)
    {
        switch (variant)
        {
            case 14: /* TEF6686A Lithio */
                id = '6';
                break;

            case 1: /* TEF6687A Lithio FMSI */
                id = '7';
                this->fmsi = true;
                break;

            case 9: /* TEF6688A Lithio DR */
                id = '8';
                break;

            case 3: /* TEF6689A Lithio FMSI DR */
                id = '9';
                this->fmsi = true;
                break;
        }
        
        this->version = 100 * versionHwMajor + versionSwMajor;
        snprintf(this->tuner, sizeof(this->tuner),
                 "TEF668%c/V%d", id, this->version);
    }
    else if (type == 7)
    {
        switch (variant)
        {
            case 7: /* TEA6850 Tiger-2 */
                id = '0';
                break;

            case 5: /* TEA6851 Tiger-2 Premium */
                id = '1';
                break;

            case 4: /* TEA6852 Tiger-2 Premium RDS */
                id = '2';
                break;

            case 6: /* TEA6853 Tiger-2 Digital */
                id = '3';
                break;
        }

        snprintf(this->tuner, sizeof(this->tuner),
                 "TEA685%c", id);
    }
    else
    {
        snprintf(this->tuner, sizeof(this->tuner),
                 "%d,%d/%d,%d", type, variant, versionHwMajor, versionSwMajor);
    }
}

void
TEF668X::readQuality()
{
    QualityData data;
    i2c.read((this->mode == MODE_FM ? MODULE_FM : MODULE_AM),
             (this->mode == MODE_FM ? FM_Get_Quality_Status : AM_Get_Quality_Status),
             sizeof(QualityData) / sizeof(uint16_t),
             (uint16_t*)&data);
    
    const uint16_t timestamp = data.status & 0x1FF;
    if (timestamp != 0)
    {
        this->rssi.add(data.level * 10);
        this->cci.add(data.coChannel);
    }
}

void
TEF668X::readRds()
{
    RdsData data; 
    i2c.read(MODULE_FM, FM_Get_RDS_Data,
             sizeof(RdsData) / sizeof(uint16_t),
             (uint16_t*)&data);

    const uint8_t status = (uint8_t)(data.status >> 8);
    if ((status & B10000000) == 0)
    {
        /* Data available flag == 0 */
        return;
    }

    if ((status & B00100000))
    {
        /* Ignore first PI data, will be repeated in continuous operation */
        /* TODO: Update RDS indicator */
        return;
    }

    const uint8_t errors = (uint8_t)(data.errors >> 8);
    this->piBuffer.add(data.blockA, errors & B11000000);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_A, data.blockA, (errors & B11000000) >> 6);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_B, data.blockB, (errors & B110000) >> 4);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_C, data.blockC, (errors & B1100) >> 2);
    this->rdsBuffer.set(RdsGroupBuffer::BLOCK_D, data.blockD, (errors & B11));
}

bool
TEF668X::readStereo()
{
    uint16_t data;
    i2c.read(MODULE_FM, FM_Get_Signal_Status, 1, &data);
    return (data >> 8) & B10000000;
}

void
TEF668X::patch()
{
#if TUNER_TEF668X_PATCH_V101
    if (this->version == 101)
    {
        i2c.writeConfig(LITHIO_PATCH);
        i2c.writePatch(LITHIO_PATCH_V101, LITHIO_PATCH_V101_LEN);

        i2c.writeConfig(LITHIO_LUT);
        i2c.writePatch(LITHIO_LUT_V101, LITHIO_LUT_V101_LEN);
        return;
    }
#endif
    
#if TUNER_TEF668X_PATCH_V102
    if (this->version == 102)
    {
        i2c.writeConfig(LITHIO_PATCH);
        i2c.writePatch(LITHIO_PATCH_V102, LITHIO_PATCH_V102_LEN);

        i2c.writeConfig(LITHIO_LUT);
        i2c.writePatch(LITHIO_LUT_V102, LITHIO_LUT_V102_LEN);
        return;
    }
#endif

#if TUNER_TEF668X_PATCH_V205
    if (this->version == 205)
    {
        i2c.writeConfig(LITHIO_PATCH);
        i2c.writePatch(LITHIO_PATCH_V205, LITHIO_PATCH_V205_LEN);

        i2c.writeConfig(LITHIO_LUT);
        i2c.writePatch(LITHIO_LUT_V205, LITHIO_LUT_V205_LEN);
        
        this->minVhfFreq = 64000;
        return;
    }
#endif
}
