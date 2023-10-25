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

#ifndef FMDX_TUNER_CONTROLLER_H
#define FMDX_TUNER_CONTROLLER_H

struct Command;

class Controller
{
public:
    virtual void setup() = 0;
    virtual void hello() = 0;
    virtual void loop() = 0;
    virtual const Command* getCommands(uint8_t *len) = 0;
};

struct Command
{
    const char* name;
    bool (*callback)(Controller*, const char*);
};

#endif
