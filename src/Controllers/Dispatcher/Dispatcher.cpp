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
#include "Dispatcher.hpp"
#include "../../Protocol.h"
#include "../../../Config.hpp"


void
Dispatcher::setup()
{
}

void
Dispatcher::hello()
{
    this->stream.flush();
    this->stream.print("\nOK\n");
}

void
Dispatcher::loop()
{
    if (this->read())
    {
        this->process();
    }
}

const Command*
Dispatcher::getCommands(uint8_t *len)
{
    static constexpr Command commands[] =
    {
        { FMDX_TUNER_PROTOCOL_STARTUP, &this->cbStartup },
        { FMDX_TUNER_PROTOCOL_SHUTDOWN, &this->cbShutdown }
    };

    *len = sizeof(commands) / sizeof(Command);
    return commands;
}

void
Dispatcher::setControllers(Controller **ctrl,
                           size_t       len)
{
    this->ctrl = ctrl;
    this->ctrl_len = len;
}

void
Dispatcher::await()
{
    for (;;)
    {
        if (this->read() && 
            strcmp(buff, FMDX_TUNER_PROTOCOL_STARTUP) == 0)
        {
            this->running = true;
            return;
        }
    }
}

bool
Dispatcher::isRunning()
{
    return this->running;
}

bool
Dispatcher::read()
{
    if (!this->stream.available())
    {
        return false;
    }

    buff[buff_pos] = this->stream.read();
    if(buff[buff_pos] != '\n')
    {
        /* If this command is too long to
         * fit into a buffer, clip it */
        if (buff_pos != buff_len - 1)
        {
            buff_pos++;
        }

        return false;
    }

    buff[buff_pos] = '\0';
    buff_pos = 0;
    return true;
}

void
Dispatcher::process()
{
    const char name[] = { buff[0] , '\0'};
    char *args = this->parseArgs();
    bool ret = false;
    
    for (uint8_t i = 0; i < ctrl_len; i++)
    {
        uint8_t cmd_len;
        const Command *cmd = (*this->ctrl[i]).getCommands(&cmd_len);

        for (uint8_t j = 0; j < cmd_len; j++)
        {
            if (strcmp(cmd[j].name, name) == 0)
            {
                ret = cmd[j].callback(this->ctrl[i], args);
                break;
            }
        }
    }

    /*
    if (!ret)
        this->stream.print("ERROR\n");
    else
        this->stream.print("OK\n");
    */
    (void)ret;
}

char*
Dispatcher::parseArgs()
{
/*
    char *args = strchr(buff, '=');

    if (args)
    {
        *args = '\0';
        args++;
    }
    return args;
*/
    return buff+1;
}


bool
Dispatcher::cbStartup(Controller *instance,
                      const char *args)
{
    Dispatcher *dispatcher = (Dispatcher*)instance;
    (void)args;    

    dispatcher->stream.print("\nOK\n");
    return true;
}

bool
Dispatcher::cbShutdown(Controller *instance,
                       const char *args)
{
    Dispatcher *dispatcher = (Dispatcher*)instance;
    (void)args;
    
    dispatcher->running = false;
    return true;
}
