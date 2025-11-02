/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2023-2025  Konrad Kosmatka
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

#include <stdint.h>
#include "TunerDriver.hpp"

Mode
TunerDriver::getMode()
{
    return this->mode;
}

uint32_t
TunerDriver::getFrequency()
{
    return this->frequency;
}

uint32_t
TunerDriver::getStep()
{
    return this->step;
}

uint32_t
TunerDriver::getBandwidth()
{
    return this->bandwidth;
}

uint32_t
TunerDriver::getAlignment()
{
    return this->alignment;
}
