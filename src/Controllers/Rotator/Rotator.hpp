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

#ifndef FMDX_TUNER_ROTATOR_H
#define FMDX_TUNER_ROTATOR_H

#include "../Controller.hpp"

class Rotator : public Controller
{
public:
    enum State : uint8_t
    {
        ROTATION_NONE = 0,
        ROTATION_CW = 1,
        ROTATION_CCW = 2
    };
    
    Rotator(uint8_t _pin_cw, uint8_t _pin_ccw) : pin_cw(_pin_cw), pin_ccw(_pin_ccw) {};
    
    void setup();
    void hello();
    void loop();
    const Command* getCommands(uint8_t *len);

private:
    const uint8_t pin_cw;
    const uint8_t pin_ccw;
    State state = ROTATION_NONE;
    State last_state = ROTATION_NONE;
    State req_state = ROTATION_NONE;
    bool waiting = false;
    uint32_t timer;

    void stop();
    void print();

    /* Callbacks for dispatcher */
    static bool cbRequest(Controller *instance, const char *args);
};

#endif
