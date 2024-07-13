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

#ifndef FMDX_TUNER_LED_H
#define FMDX_TUNER_LED_H
#include <stdint.h>
#include "../Controller.hpp"
#include "../../Utils/Timer.hpp"
#include "../../../Config.hpp"

class Led : public Controller
{
public:
    template <typename... Args>
    Led(Args... args) : pins{ uint8_t(args)... }
    {
        static_assert(sizeof...(Args) == LED_COUNT, "Invalid count of pins");
    }

    void setup();
    void hello();
    void loop();
    const Command* getCommands(uint8_t *len);

    void on(uint8_t id);
    void off(uint8_t id);
    void blink(uint8_t id, Timer::Interval timeout);

private:
    const uint8_t pins[LED_COUNT];
    Timer timers[LED_COUNT];
};

#if LED_ENABLED
extern Led led;
#endif /* LED_ENABLED */

#endif /* FMDX_TUNER_LED_H */
