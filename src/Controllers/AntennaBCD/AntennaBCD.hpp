/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2024  Konrad Kosmatka
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

#ifndef FMDX_TUNER_ANTENNA_BCD_H
#define FMDX_TUNER_ANTENNA_BCD_H
#include <stdint.h>
#include "../Controller.hpp"
#include "../Tuner/Tuner.hpp"
#include "../../../Config.hpp"

#define BCD_SIZE(n) (((sizeof(unsigned int) * 8)) - (__builtin_clz((n - 1))))

class AntennaBCD : public Controller
{
public:
    template <typename... Args>
    AntennaBCD(Tuner &_tuner, Args... args) : tuner(_tuner), pins{ uint8_t(args)... }
    {
        constexpr int maxCount = 1 << sizeof...(Args);
        static_assert(ANTENNA_COUNT > 1, "Invalid count of antennas");
        static_assert(ANTENNA_COUNT <= maxCount, "Invalid count of pins");
    }

    void setup();
    void hello();
    void loop();
    const Command* getCommands(uint8_t *len);

private:
    Tuner &tuner;
    const uint8_t pins[BCD_SIZE(ANTENNA_COUNT)];
    uint8_t current = 0;

    void print();

    /* Callbacks for dispatcher */
    static bool cbRequest(Controller *instance, const char *args);
};

#endif
