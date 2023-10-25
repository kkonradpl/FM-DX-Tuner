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

#ifndef FMDX_RADIO_BUTTON_H
#define FMDX_RADIO_BUTTON_H

#include "../Controller.hpp"

class Button : public Controller
{
public:
    enum State : uint8_t
    {
        STATE_NONE = 0,
        STATE_PRESSED = 1,
        STATE_RELEASED = 2
    };
    
    Button(uint8_t _pin, void (*_callback)(uint8_t, void*), void *_userData) : pin(_pin), callback(_callback), userData(_userData) {};
    
    void setup();
    void hello();
    void loop();
    const Command* getCommands(uint8_t *len);
    
private:
    const uint8_t pin;
    void (*callback)(uint8_t, void*);
    void *userData;
    uint32_t timer = 0;
    bool last_state = HIGH;
    bool state = HIGH;

    uint8_t update();
};

#endif
