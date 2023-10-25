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

#include <Arduino.h>
#include "Button.hpp"
#include "../../../Config.hpp"

void
Button::setup()
{
    pinMode(this->pin, INPUT);
    digitalWrite(this->pin, HIGH);
}

void
Button::hello()
{
}

void
Button::loop()
{
    uint8_t state = this->update();
    
    if (state != STATE_NONE)
    {
        this->callback(state, this->userData);
    }
}

const Command* Button::getCommands(uint8_t *len)
{
    *len = 0;
    return NULL;
}

uint8_t
Button::update()
{
    uint8_t current = digitalRead(pin);
    uint8_t ret = STATE_NONE;

    if (current != this->last_state)
    {
        this->timer = millis();
    }

    if ((millis() - this->timer) > BUTTON_DEBOUNCE)
    {
        if (current != this->state)
        {
            ret = (current ? STATE_RELEASED : STATE_PRESSED);
            this->state = current;
        }
    }

    this->last_state = current;
    return ret;
}
