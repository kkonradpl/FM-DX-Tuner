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

#ifndef FMDX_TUNER_TUNER_H
#define FMDX_TUNER_TUNER_H

#include "../Controller.hpp"
#include "../../Utils/Timer.hpp"
#include "TunerDriver.hpp"
#include "Squelch.hpp"
#include "RdsGroupBuffer.hpp"
#include "RdsPiBuffer.hpp"

class Tuner : public Controller
{
public:
    Tuner(TunerDriver &_driver) : driver(_driver) {};
    void setup() override;
    void hello() override;
    void loop() override;
    const Command* getCommands(uint8_t *len) override;

    bool start();
    void shutdown();
    void clear();

private:
    void handleRds();
    void handleQuality();
    void handleSquelch();

    /* Callbacks for dispatcher */
    static bool cbMode(Controller *instance, const char *args);
    static bool cbFrequency(Controller *instance, const char *args);
    static bool cbDeemphasis(Controller *instance, const char *args);
    static bool cbAgc(Controller *instance, const char *args);
    static bool cbAlignment(Controller *instance, const char *args);
    static bool cbBandwidth(Controller *instance, const char *args);
    static bool cbVolume(Controller *instance, const char *args);
    static bool cbSquelch(Controller *instance, const char *args);
    static bool cbOutputMode(Controller *instance, const char *args);
    static bool cbQuality(Controller *instance, const char *args);
    static bool cbScan(Controller *instance, const char *args);
    static bool cbCustom(Controller *instance, const char *args);

    void feedback(const char *message, uint32_t value);
    void feedback2(const char *message, uint32_t value, uint32_t value2);

    /* Callbacks for tuner */
    /* ... */

    TunerDriver &driver;
    TunerDriver::QualityMode qualityMode = TunerDriver::QUALITY_DEFAULT;
    Timer timerQuality{66};
    Timer timerSquelch{100};
    uint8_t volume = 100;

    static const uint8_t squelchTimeout = 3;
    Squelch squelch{squelchTimeout};
};

#endif
