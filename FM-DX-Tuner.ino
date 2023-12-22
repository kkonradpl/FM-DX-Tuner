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

#include "src/Controllers/Antenna/Antenna.hpp"
#include "src/Controllers/Button/Button.hpp"
#include "src/Controllers/Rotator/Rotator.hpp"
#include "src/Controllers/Dispatcher/Dispatcher.hpp"
#include "src/Controllers/Tuner/Tuner.hpp"
#include "src/Controllers/Tuner/SAF7730/SAF7730.hpp"
#include "src/Controllers/Tuner/TEF668X/TEF668X.hpp"
#include "Config.hpp"

void cbButton(uint8_t  state,
              void    *userData)
{
    (void)userData;

    if (state == Button::STATE_PRESSED)
    {
        Serial.print("!\n");
    }
}  

Dispatcher dispatcher(Serial);
Button button(BUTTON_PIN, &cbButton, NULL);
Rotator rotator(ROTATOR_PIN_CW, ROTATOR_PIN_CCW);
TUNER_DRIVER driver;
Tuner tuner(driver);
Antenna antenna(tuner, ANTENNA_PINS);

Controller *ctrls[] =
{
    &dispatcher,
#if BUTTON_ENABLED
    &button,
#endif
#if ANTENNA_ENABLED
    &antenna,
#endif
#if ROTATOR_ENABLED
    &rotator,
#endif
    &tuner
};

void setup()
{
    Serial.begin(SERIAL_PORT_SPEED);
    while(!Serial);

    const size_t count = sizeof(ctrls) / sizeof(ctrls[0]);
    dispatcher.setControllers(ctrls, count);
}

void loop()
{
    for (auto &controller : ctrls)
    {
        controller->setup();
    }

    bool status;
    do
    {
        dispatcher.await();
        status = tuner.start(); 
    } while (!status);

    for (auto &controller : ctrls)
    {
        controller->hello();
    }

    for (;;)
    {
        for (auto &controller : ctrls)
        {
            controller->loop();
        }

        if (!dispatcher.isRunning())
        {
            tuner.shutdown();
            break;
        }
    }

/*
    for (auto &controller : ctrls)
    {
        controller->shutdown();
    }
*/
}
