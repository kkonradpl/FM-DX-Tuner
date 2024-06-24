/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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
#include "../../Comm.hpp"
#include "Antenna.hpp"

void
Antenna::setup()
{
    for (uint8_t i = 0; i < ANTENNA_COUNT; i++)
    {
        pinMode(this->pins[i], OUTPUT);
        digitalWrite(this->pins[i], (i == this->current ? HIGH : LOW));
    }
}

void
Antenna::hello()
{
    //Comm.print("ANT:0-");
    //Comm.print(ANTENNA_COUNT, DEC);
    //Comm.print('\n');
    
    this->print();
}

void
Antenna::loop()
{
}

const Command*
Antenna::getCommands(uint8_t *len)
{
    static constexpr Command commands[] =
    {
        { "Z", &this->cbRequest }
    };

    *len = sizeof(commands) / sizeof(Command);
    return commands;
}

void Antenna::print()
{
    Comm.print('Z');
    Comm.print(this->current, DEC);
    Comm.print('\n');
}
    
bool
Antenna::cbRequest(Controller *instance,
                   const char *args)
{
    Antenna *antenna = (Antenna*)instance;
    const uint32_t value = atol(args);
    
    if (value >= ANTENNA_COUNT)
        return false;

    digitalWrite(antenna->pins[antenna->current], LOW);
    antenna->current = value;
    digitalWrite(antenna->pins[value], HIGH);

    delay(ANTENNA_SWITCH_DELAY);
    antenna->print();
    antenna->tuner.clear();
    
    return true;
}
