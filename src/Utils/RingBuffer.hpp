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

#ifndef FMDX_TUNER_RING_BUFFER_H
#define FMDX_TUNER_RING_BUFFER_H
#include <limits.h>

template <bool Condition, typename TrueType, typename FalseType>
struct conditional {
    using type = TrueType;
};

template <typename TrueType, typename FalseType>
struct conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

template <size_t Count>
struct CountTypeStruct {
    static_assert(Count < UINT32_MAX, "Invalid RingBuffer size");
    using type = typename conditional<(Count < UINT8_MAX), uint8_t,
                    typename conditional<(Count < UINT16_MAX), uint16_t,
                        uint32_t
                    >::type
                 >::type;
};

template <typename T>
struct isSigned {
    static constexpr bool value = (T(-1) < T(0));
};

template <typename T>
constexpr T minValue()
{
    return isSigned<T>::value ?
        T(1) << (sizeof(T) * 8 - 1) :
        T(0);
}

template <typename T>
constexpr T maxValue() {
    return isSigned<T>::value ?
        ~(T(1) << (sizeof(T) * 8 - 1)) :
        ~T(0);
}

template <class Type, class AccType, size_t Count>
class RingBuffer
{
    using CountType = typename CountTypeStruct<Count>::type;
public:
    RingBuffer()
    {
        this->reset();
    };

    void
    reset(void)
    {
        this->pos = static_cast<CountType>(-1);
        this->fill = 0;
    }

    void
    add(Type value)
    {
        this->pos = static_cast<CountType>(this->pos + 1) % Count;
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
    getLast(void)
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

    Type
    getMin(void)
    {
        Type value = maxValue<Type>();
        for (CountType i = 0; i < this->fill; i++)
        {
            if (this->data[i] < value)
            {
                value = this->data[i];
            }
        }
        return value;
    }

    Type
    getMax(void)
    {
        Type value = minValue<Type>();
        for (CountType i = 0; i < this->fill; i++)
        {
            if (this->data[i] > value)
            {
                value = this->data[i];
            }
        }
        return value;
    }

private:
    Type data[Count];
    CountType pos;
    CountType fill;
};

#endif
