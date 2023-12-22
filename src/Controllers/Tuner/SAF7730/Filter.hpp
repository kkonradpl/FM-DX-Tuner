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

#ifndef FMDX_TUNER_SAF7730_FILTER_H
#define FMDX_TUNER_SAF7730_FILTER_H

#include "I2cDirana.hpp"
#include "../../../Protocol.h"

class Filter
{
public:
    Filter(I2cDirana &_i2c) : i2c(_i2c) {};
    void setAdaptive();
    void setFixed(Mode mode, uint8_t filter);

private:
    void setFilter(uint8_t bank, uint8_t filter);

    I2cDirana &i2c;
    uint8_t flag = 0;
};


#endif