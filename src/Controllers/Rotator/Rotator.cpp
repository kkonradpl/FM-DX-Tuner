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
#include "Rotator.hpp"
#include "../../../Config.hpp"

void
Rotator::setup()
{
    pinMode(pin_cw, OUTPUT);
    digitalWrite(pin_cw, LOW);
    pinMode(pin_ccw, OUTPUT);
    digitalWrite(pin_ccw, LOW);
}

void
Rotator::hello()
{
    print();
}

void
Rotator::loop()
{
    /* Request rotator stop automatically after a specified time */
    if (this->state != ROTATION_NONE &&
        (millis() - this->timer) >= ROTATOR_TIMEOUT * 1000UL)
    {
        this->stop();
        this->print();
        return;
    }
    
    if (this->req_state == ROTATION_NONE)
        return;

    if (((this->last_state == ROTATION_CW && this->req_state == ROTATION_CCW) ||
         (this->last_state == ROTATION_CCW && this->req_state == ROTATION_CW)) &&
        (millis() - this->timer) <= ROTATOR_DELAY)
    {
        /* Wait before changing a rotation direction */
        if (!this->waiting)
        {
            waiting = true;
            this->print();
        }
        return;
    }

    if (this->req_state == ROTATION_CW)
        digitalWrite(this->pin_cw, HIGH);
    else if (this->req_state == ROTATION_CCW)
        digitalWrite(this->pin_ccw, HIGH);

    this->timer = millis();
    this->state = this->req_state;
    this->req_state = ROTATION_NONE;
    this->print();
}

const Command*
Rotator::getCommands(uint8_t *len)
{
    static constexpr Command commands[] =
    {
        { "C", &this->cbRequest }
    };

    *len = sizeof(commands) / sizeof(Command);
    return commands;
}

void
Rotator::stop()
{
    digitalWrite(this->pin_cw, LOW);
    digitalWrite(this->pin_ccw, LOW);
    this->timer = millis();
    this->last_state = this->state;
    this->state = ROTATION_NONE;
}

void
Rotator::print()
{
    Serial.print('C');
    if (this->req_state)
    {
        Serial.print('-');
        Serial.print(this->req_state, DEC);
    }
    else
    {
        Serial.print(this->state, DEC);
    }
    Serial.print('\n');
}

bool
Rotator::cbRequest(Controller *instance,
                   const char *args)
{
    Rotator *rotator = (Rotator*)instance;
    const uint32_t new_state = atol(args);

    if (new_state == ROTATION_NONE)
    {
        /* Cancel any queued action */
        rotator->req_state = ROTATION_NONE;
    }

    if (new_state == rotator->state)
    {
        rotator->print();
        return true;
    }

    if (rotator->state != ROTATION_NONE)
    {
        rotator->stop();
    }

    if (new_state == ROTATION_NONE)
    {
        rotator->print();
        return true;
    }
    
    if (new_state == ROTATION_CW ||
        new_state == ROTATION_CCW)
    {
        rotator->req_state = (State)new_state;
        rotator->waiting = false;
        return true;
    }

    return false;
}
