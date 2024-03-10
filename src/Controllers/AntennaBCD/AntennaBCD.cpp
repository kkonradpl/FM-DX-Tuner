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
#include "AntennaBCD.hpp"

void
AntennaBCD::setup()
{
    for (uint8_t i = 0; i < sizeof(this->pins); i++)
    {
        pinMode(this->pins[i], OUTPUT);
        digitalWrite(this->pins[i], 0);
    }
}

void
AntennaBCD::hello()
{
    //Serial.print("ANT:0-");
    //Serial.print(ANTENNA_COUNT, DEC);
    //Serial.print('\n');

    this->print();
}

void
AntennaBCD::loop()
{
}

const Command*
AntennaBCD::getCommands(uint8_t *len)
{
    static constexpr Command commands[] =
    {
        { "Z", &this->cbRequest }
    };

    *len = sizeof(commands) / sizeof(Command);
    return commands;
}

void AntennaBCD::print()
{
    Serial.print('Z');
    Serial.print(this->current, DEC);
    Serial.print('\n');
}

bool
AntennaBCD::cbRequest(Controller *instance,
                      const char *args)
{
    AntennaBCD *antenna = (AntennaBCD*)instance;
    uint32_t value = atol(args);

    if (value >= ANTENNA_COUNT)
        return false;

    antenna->current = value;
    for (int8_t i = sizeof(AntennaBCD::pins) - 1; i >= 0; i--)
    {
        const uint8_t state = value & 1;
        digitalWrite(antenna->pins[i], state);
        value >>= 1;
    }

    delay(ANTENNA_SWITCH_DELAY);
    antenna->print();
    antenna->tuner.clear();

    return true;
}
