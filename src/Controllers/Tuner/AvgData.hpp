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

#ifndef FMDX_TUNER_AVG_DATA_H
#define FMDX_TUNER_AVG_DATA_H

template <class Type, class AccType, class CountType, CountType Count>
class AvgData
{
public:
    AvgData()
    {
        this->reset();
    };

    void
    reset(void)
    {
        this->pos = (CountType)-1;
        this->fill = 0;
    }

    void
    add(Type value)
    {
        this->pos = (CountType)(this->pos + 1) % Count;
        this->data[this->pos] = value;

        if (this->fill < Count)
        {
            this->fill++;
        }
    }

    bool
    isAvailable(void)
    {
        return (bool)this->fill;
    }

    Type
    getRaw(void)
    {
        return this->data[this->pos];
    }

    Type
    getAvg(void)
    {
        AccType total = 0;
        for (CountType i = 0; i < this->fill; i++)
        {
            total += this->data[i];
        }
        return total / this->fill;
    }

private:
    Type data[Count];
    CountType pos;
    CountType fill;
};

#endif
