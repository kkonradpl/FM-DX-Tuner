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

#include <Arduino.h>
#include "Led.hpp"

void
Led::setup()
{
    for (uint8_t id = 0; id < LED_COUNT; id++)
    {
        pinMode(this->pins[id], OUTPUT);
        digitalWrite(this->pins[id], LOW);
    }
}

void
Led::hello()
{
}

void
Led::loop()
{
    for (uint8_t id = 0; id < LED_COUNT; id++)
    {
        if (this->timers[id].process(Timer::Mode::Oneshot))
        {
            digitalWrite(this->pins[id], LOW);
        }
    }
}

const Command*
Led::getCommands(uint8_t *len)
{
    *len = 0;
    return NULL;
}

void
Led::on(uint8_t id)
{
    digitalWrite(this->pins[id], HIGH);
    this->timers[id].unset();
}

void
Led::off(uint8_t id)
{
    digitalWrite(this->pins[id], LOW);
    this->timers[id].unset();
}

void
Led::blink(uint8_t         id,
           Timer::Interval timeout)
{
    digitalWrite(this->pins[id], HIGH);
    this->timers[id].set(timeout);
}

#if LED_ENABLED
Led led{LED_PINS};
#endif