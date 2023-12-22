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

#ifndef FMDX_TUNER_CONFIG_SAF7730_H
#define FMDX_TUNER_CONFIG_SAF7730_H

/* -----------------------------------------------------------------------
   SAF7730 configuration
   ----------------------------------------------------------------------- */
#define TUNER_SAF7730_PIN_RDS   2
#define TUNER_SAF7730_PIN_POWER 4
#define TUNER_SAF7730_PIN_RESET 5

/* Alignment value for the first threshold */
#define TUNER_SAF7730_ALIGNMENT_START 52

/* Aligment frequency thresholds */
const uint32_t TUNER_SAF7730_ALIGNMENT[] PROGMEM =
{
    107300, // 52 for >= 107300
    104500, // 53 for >= 104500
    100400, // 54 ...
    100400, // 55
    97500,  // 56
    95600,  // 57
    92900,  // 58
    90500,  // 59
    87800,  // 60
    87000,  // 61
    85000,  // 62
    84000,  // 63
    83000,  // 64
    78000,  // 65
    76000,  // 66
    74000,  // 67
    73000,  // 68
    70000,  // 69
    0       // 70
};

/* FM signal level offset for calibration */
#define TUNER_SAF7730_FM_LEVEL_OFFSET 0

/* AM signal level offset for calibration */
#define TUNER_SAF7730_AM_LEVEL_OFFSET 0

/* ----------------------------------------------------------------------- */

#endif
