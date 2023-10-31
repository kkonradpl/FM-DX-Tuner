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

#ifndef FMDX_TUNER_PROTOCOL_H
#define FMDX_TUNER_PROTOCOL_H

#define FMDX_TUNER_PROTOCOL_VERSION_MAJOR 1
#define FMDX_TUNER_PROTOCOL_VERSION_MINOR 0

#define FMDX_TUNER_PROTOCOL_STARTUP     "x"
#define FMDX_TUNER_PROTOCOL_SHUTDOWN    "X"

#define FMDX_TUNER_PROTOCOL_MODE        "M"
#define FMDX_TUNER_PROTOCOL_TUNE        "T"
#define FMDX_TUNER_PROTOCOL_DEEMPHASIS  "D"
#define FMDX_TUNER_PROTOCOL_AGC         "A"
#define FMDX_TUNER_PROTOCOL_ALIGNMENT   "V"
#define FMDX_TUNER_PROTOCOL_BANDWIDTH   "W"
#define FMDX_TUNER_PROTOCOL_VOLUME      "Y"
#define FMDX_TUNER_PROTOCOL_SQUELCH     "Q"
#define FMDX_TUNER_PROTOCOL_OUTPUT_MODE "B"
#define FMDX_TUNER_PROTOCOL_QUALITY     "I"
#define FMDX_TUNER_PROTOCOL_SCAN        "S"
#define FMDX_TUNER_PROTOCOL_CUSTOM      "G"
#define FMDX_TUNER_PROTOCOL_ROTATOR     "C"
#define FMDX_TUNER_PROTOCOL_ANTENNA     "Z"

/* TODO */
enum TunerError
{
    ERROR_NONE = 0,
    ERROR_BUS = 1,
    ERROR_INIT = 2
};

enum Mode
{
    MODE_NONE = -1,
    MODE_FM = 0,
    MODE_AM = 1
};

enum SquelchMode
{
    SQUELCH_NONE = 0,
    SQUELCH_RSSI = 1,
    SQUELCH_STEREO = 2,
    SQUELCH_CCI = 3
};

enum OutputMode
{
    OUTPUT_MODE_STEREO = 0,
    OUTPUT_MODE_MONO = 1,
    OUTPUT_MODE_MPX = 2
};

#endif
