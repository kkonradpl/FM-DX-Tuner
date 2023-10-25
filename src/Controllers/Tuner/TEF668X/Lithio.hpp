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

#ifndef FMDX_RADIO_LITHIO_H
#define FMDX_RADIO_LITHIO_H

#include "../../../../Config.hpp"

#define LITHIO_I2C 0xC8

#define LITHIO_FOREACH_1(f, x) f(x)
#define LITHIO_FOREACH_2(f, x, ...) f(x), LITHIO_FOREACH_1(f, __VA_ARGS__)
#define LITHIO_FOREACH_3(f, x, ...) f(x), LITHIO_FOREACH_2(f, __VA_ARGS__)
#define LITHIO_FOREACH_4(f, x, ...) f(x), LITHIO_FOREACH_3(f, __VA_ARGS__)
#define LITHIO_FOREACH_5(f, x, ...) f(x), LITHIO_FOREACH_4(f, __VA_ARGS__)
#define LITHIO_FOREACH_6(f, x, ...) f(x), LITHIO_FOREACH_5(f, __VA_ARGS__)
#define LITHIO_FOREACH_7(f, x, ...) f(x), LITHIO_FOREACH_6(f, __VA_ARGS__)
#define LITHIO_FOREACH_N(_7, _6, _5, _4, _3, _2, _1, N, ...) LITHIO_FOREACH_##N
#define LITHIO_FOREACH(f, ...) LITHIO_FOREACH_N(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)(f, __VA_ARGS__)

#define LITHIO_VALUE(x) (uint8_t)(x >> 8), (uint8_t)(x)
#define LITHIO_VALUE_COUNT(...) ((sizeof((uint16_t[]){ __VA_ARGS__ }) / sizeof(uint16_t)))
#define LITHIO_DATA_LENGTH(...) (3 + 2 * LITHIO_VALUE_COUNT(__VA_ARGS__))

#define LITHIO_DATA(module, command, index, ...) \
    LITHIO_DATA_LENGTH(__VA_ARGS__), module, command, index, LITHIO_FOREACH(LITHIO_VALUE, __VA_ARGS__)


enum Module : uint8_t
{
    MODULE_START = 20,
    MODULE_PATCH = 27,
    MODULE_CONTROL = 28,
    MODULE_RESET = 30,
    MODULE_FM = 32,
    MODULE_AM = 33,
    MODULE_AUDIO = 48,
    MODULE_APPL = 64
};

enum FM : uint8_t
{
    FM_Tune_To = 1,
    FM_Set_Tune_Options = 2,
    FM_Set_Bandwidth = 10,
    FM_Set_RFAGC = 11,
    FM_Set_Antenna = 12,
    FM_Set_MphSuppression = 20,
    FM_Set_ChannelEqualizer = 22,
    FM_Set_NoiseBlanker = 23,
    FM_Set_NoiseBlanker_Options = 24,
    FM_Set_DigitalRadio = 30,
    FM_Set_Deemphasis = 31,
    FM_Set_StereoImprovement = 32,
    FM_Set_Highcut_Fix = 33,
    FM_Set_Set_Lowcut_Fix = 34,
    FM_Set_LevelStep = 38,
    FM_Set_LevelOffset = 39,
    FM_Set_SoftMute_Time = 40,
    FM_Set_SoftMute_Level = 42,
    FM_Set_SoftMute_Noise = 43,
    FM_Set_SoftMute_Mph = 44,
    FM_Set_SoftMute_Max = 45,
    FM_Set_HighCut_Time = 50,
    FM_Set_HighCut_Mod = 51,
    FM_Set_HighCut_Level = 52,
    FM_Set_HighCut_Noise = 53,
    FM_Set_HighCut_Mph = 54,
    FM_Set_HighCut_Max = 55,
    FM_Set_HighCut_Min = 56,
    FM_Set_LowCut_Max = 57,
    FM_Set_LowCut_Min = 58,
    FM_Set_HighCut_Options = 59,
    FM_Set_Stereo_Time = 60,
    FM_Set_Stereo_Mod = 61,
    FM_Set_Stereo_Level = 62,
    FM_Set_Stereo_Noise = 63,
    FM_Set_Stereo_Mph = 64,
    FM_Set_Stereo_Max = 65,
    FM_Set_Stereo_Min = 66,
    FM_Set_StHiBlend_Time = 70,
    FM_Set_StHiBlend_Mod = 71,
    FM_Set_StHiBlend_Level = 72,
    FM_Set_StHiBlend_Noise = 73,
    FM_Set_StHiBlend_Mph = 74,
    FM_Set_StHiBlend_Max = 75,
    FM_Set_StHiBlend_Min = 76,
    FM_Set_Scaler = 80,
    FM_Set_RDS = 81,
    FM_Set_QualityStatus = 82,
    FM_Set_DR_Blend = 83,
    FM_Set_DR_Options = 84,
    FM_Set_Specials = 85,
    FM_Set_Bandwidth_Options = 86,
    FM_Set_StBandBlend_Time = 90,
    FM_Set_StBandBlend_Gain = 91,
    FM_Set_StBandBlend_Bias = 92,
    FM_Get_Quality_Status = 128,
    FM_Get_Quality_Data = 129,
    FM_Get_RDS_Status = 130,
    FM_Get_RDS_Data = 131,
    FM_Get_AGC = 132,
    FM_Get_Signal_Status = 133,
    FM_Get_Processing_Status = 134,
    FM_Get_Interface_Status = 135
};

enum AM : uint8_t
{
    AM_Tune_To = 1,
    AM_Set_Bandwidth = 10,
    AM_Set_RFAGC = 11,
    AM_Set_Antenna = 12,
    AM_Set_CoChannelDet = 14,
    AM_Set_NoiseBlanker = 23,
    AM_Set_NoiseBlanker_Audio = 24,
    AM_Set_DigitalRadio = 30,
    AM_Set_Highcut_Fix = 33,
    AM_Set_Set_Lowcut_Fix = 34,
    AM_Set_LevelStep = 38,
    AM_Set_LevelOffset = 39,
    AM_Set_SoftMute_Time = 40,
    AM_Set_SoftMute_Mod = 41,
    AM_Set_SoftMute_Level = 42,
    AM_Set_SoftMute_Max = 45,
    AM_Set_HighCut_Time = 50,
    AM_Set_HighCut_Mod = 51,
    AM_Set_HighCut_Level = 52,
    AM_Set_HighCut_Max = 55,
    AM_Set_HighCut_Min = 56,
    AM_Set_LowCut_Max = 57,
    AM_Set_LowCut_Min = 58,
    AM_Set_Scaler = 80,
    AM_Set_QualityStatus = 82,
    AM_Set_DR_Blend = 83,
    AM_Set_DR_Options = 84,
    AM_Set_Specials = 85,
    AM_Get_Quality_Status = 128,
    AM_Get_Quality_Data = 129,
    AM_Get_AGC = 132,
    AM_Get_Signal_Status = 133,
    AM_Get_Processing_Status = 134,
    AM_Get_Interface_Status = 135
};

enum Audio : uint8_t
{
    AUDIO_Set_Volume = 10,
    AUDIO_Set_Mute = 11,
    AUDIO_Set_Input = 12,
    AUDIO_Set_Output_Source = 13,
    AUDIO_Set_Ana_Out = 21,
    AUDIO_Set_Dig_IO = 22,
    AUDIO_Set_Input_Scaler = 23,
    AUDIO_Set_WaveGen = 24
};

enum APPL : uint8_t
{
    APPL_Set_OperationMode = 1,
    APPL_Set_GPIO = 3,
    APPL_Set_ReferenceClock = 4,
    APPL_Activate = 5,
    APPL_Get_Operation_Status = 128,
    APPL_Get_GPIO_Status = 129,
    APPL_Get_Identification = 130,
    APPL_Get_LastWrite = 131,
    APPL_Get_Interface_Status = 135
};

enum State : uint16_t
{
    STATE_BOOT = 0,
    STATE_IDLE = 1,
    STATE_ACTIVE = 2,
    STATE_ACTIVE_FM = 3,
    STATE_ACTIVE_AM = 4
};

struct IdData
{
    uint16_t device;
    uint16_t versionHw;
    uint16_t versionSw;
};

struct QualityData
{
    uint16_t status;
    int16_t level;
    uint16_t noise;
    uint16_t coChannel;
    int16_t offset;
    uint16_t bandwidth;
    uint16_t modulation;
};

struct RdsData
{
    uint16_t status;
    uint16_t blockA;
    uint16_t blockB;
    uint16_t blockC;
    uint16_t blockD;
    uint16_t errors;
};

const uint8_t LITHIO_PATCH[] PROGMEM =
{
    3, MODULE_CONTROL, 0x00, 0x00,
    3, MODULE_CONTROL, 0x00, 0x74,
    0
};

const uint8_t LITHIO_LUT[] PROGMEM =
{
    3, MODULE_CONTROL, 0x00, 0x00,
    3, MODULE_CONTROL, 0x00, 0x75,
    0
};

const uint8_t LITHIO_START[] PROGMEM =
{
    3, MODULE_CONTROL, 0x00, 0x00,
    3, MODULE_START, 0x00, 0x01,
    0
};

const uint8_t LITHIO_RESET[] PROGMEM =
{
    LITHIO_DATA(MODULE_RESET, 0x5A, 1, 0x5A5A),
    0
};

const uint8_t LITHIO_CONFIG[] PROGMEM =
{
    LITHIO_DATA(MODULE_FM, FM_Set_NoiseBlanker, 1, 1, 1000, 1000, 1250, 80, 900, 900),
    LITHIO_DATA(MODULE_FM, FM_Set_NoiseBlanker_Options, 1, 225, 112, 250, 225),
    LITHIO_DATA(MODULE_FM, FM_Set_LevelOffset, 1, (int16_t)(TUNER_TEF668X_FM_LEVEL_OFFSET * 10)),
    LITHIO_DATA(MODULE_FM, FM_Set_SoftMute_Time, 1, 120, 500, 10, 20),
    LITHIO_DATA(MODULE_FM, FM_Set_SoftMute_Level, 1, 1, 50, 220),
    LITHIO_DATA(MODULE_FM, FM_Set_SoftMute_Noise, 1, 1, 500, 1000),
    LITHIO_DATA(MODULE_FM, FM_Set_SoftMute_Mph, 1, 1, 500, 1000),
    LITHIO_DATA(MODULE_FM, FM_Set_SoftMute_Max, 1, 1, 60),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Time, 1, 100, 200, 10, 80),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Mod, 1, 0, 250, 130, 500),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Level, 1, 3, 200, 300),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Noise, 1, 0, 360, 300),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Mph, 1, 3, 200, 320),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Max, 1, 1, 3000),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Min, 1, 0, 15000),
    LITHIO_DATA(MODULE_FM, FM_Set_LowCut_Max, 1, 1, 30),
    LITHIO_DATA(MODULE_FM, FM_Set_LowCut_Min, 1, 1, 20),
    LITHIO_DATA(MODULE_FM, FM_Set_HighCut_Options, 1, 1),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Time, 1, 500, 2000, 20, 20),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Mod, 1, 0, 240, 120, 670),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Level, 1, 3, 600, 240),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Noise, 1, 3, 160, 140),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Mph, 1, 3, 160, 140),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Max, 1, 1, 4000),
    LITHIO_DATA(MODULE_FM, FM_Set_StHiBlend_Min, 1, 0, 7000),

    LITHIO_DATA(MODULE_AM, AM_Set_LevelOffset, 1, (int16_t)(TUNER_TEF668X_AM_LEVEL_OFFSET * 10)),
    
    LITHIO_DATA(MODULE_AUDIO, AUDIO_Set_Volume, 1, 0),
    LITHIO_DATA(MODULE_AUDIO, AUDIO_Set_Mute, 1, 0),
    0
};

const uint8_t LITHIO_CONFIG_STEREO[] PROGMEM =
{
    LITHIO_DATA(MODULE_FM, FM_Set_StereoImprovement, 1, 0),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Time, 1, 200, 4000, 20, 80),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Mod, 1, 0, 210, 90, 500),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Level, 1, 1, 460, 240),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Noise, 1, 3, 240, 200),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Mph, 1, 3, 240, 200),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Max, 1, 1),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Min, 1, 0, 400),
    0
};

const uint8_t LITHIO_CONFIG_FMSI[] PROGMEM =
{
    LITHIO_DATA(MODULE_FM, FM_Set_StereoImprovement, 1, 1),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Mod, 1, 0, 210, 90, 500),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Level, 1, 0, 460, 240),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Noise, 1, 0, 240, 200),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Mph, 1, 0, 240, 200),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Max, 1, 0),
    LITHIO_DATA(MODULE_FM, FM_Set_Stereo_Min, 1, 0, 400),
    LITHIO_DATA(MODULE_FM, FM_Set_StBandBlend_Time, 1, 10, 10),
    LITHIO_DATA(MODULE_FM, FM_Set_StBandBlend_Gain, 1, 700, 600, 500, 400),
    LITHIO_DATA(MODULE_FM, FM_Set_StBandBlend_Bias, 1, 100, 75, 50, 25),
    0
};

#endif
