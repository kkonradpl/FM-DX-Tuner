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

#ifndef FMDX_TUNER_DISPATCHER_H
#define FMDX_TUNER_DISPATCHER_H

#include "../Controller.hpp"

class Dispatcher : public Controller
{
public:
    Dispatcher(Stream &_stream) : stream(_stream) {};
    
    void setup();
    void hello();
    void loop();
    const Command* getCommands(uint8_t *len);

    void setControllers(Controller *ctrls[], size_t len);
    void await();
    bool isRunning();

private:
    Stream &stream;
    Controller **ctrl;
    uint8_t ctrl_len;

    static constexpr uint8_t buff_len = 16;
    char buff[buff_len];
    uint8_t buff_pos = 0;
    bool running = false;
    
    bool read();
    void process();
    char* parseArgs();
    
    /* Callbacks for dispatcher */
    static bool cbStartup(Controller *instance, const char *args);
    static bool cbShutdown(Controller *instance, const char *args);
};

#endif
