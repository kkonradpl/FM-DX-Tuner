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
#include "Scan.hpp"
#include "TunerDriver.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Protocol.h"

Scan::Scan(TunerDriver &_driver, Volume &_volume)
    : driver(_driver), volume(_volume)
{
}

void
Scan::setFrom(uint32_t value)
{
    this->from = value;
}

uint32_t
Scan::getFrom(void)
{
    return this->from;
}

void
Scan::setTo(uint32_t value)
{
    this->to = value;
}

uint32_t
Scan::getTo(void)
{
    return this->to;
}

void
Scan::setStep(uint32_t value)
{
    this->step = value;
}

uint32_t
Scan::getStep(void)
{
    return this->step;
}

void
Scan::setBandwidth(uint32_t value)
{
    this->bandwidth = value;
}

uint32_t
Scan::getBandwidth(void)
{
    return this->bandwidth;
}

void
Scan::setRepeat(bool value)
{
    this->repeat = value;
}

bool
Scan::getRepeat(void)
{
    return this->repeat;
}

bool
Scan::isActive()
{
    return (this->state != None);
}

bool
Scan::start()
{
    if (this->from == 0 ||
        this->to == 0 ||
        this->step == 0 ||
        this->from > this->to)
    {
        return false;
    }

    this->prevFrequency = this->driver.getFrequency();
    this->prevBandwidth = this->driver.getBandwidth();
        
    this->volume.mute();
    this->driver.setFrequency(this->from, TunerDriver::TUNE_SCAN);
    this->driver.setBandwidth(this->bandwidth);
    this->driver.resetQuality();
    
    /* Update bandwidth to the value set by driver */
    this->bandwidth = this->driver.getBandwidth();

    this->init();
    return true;
}

void
Scan::stop()
{
    if (this->state == None)
    {
        return;
    }

    this->driver.setFrequency(this->prevFrequency, TunerDriver::TUNE_DEFAULT);
    this->driver.setBandwidth(this->prevBandwidth);
    this->driver.resetQuality();
    this->driver.resetRds();
    this->volume.unMute();
    this->state = None;

    Serial.print('\n');
}

void
Scan::process()
{
    if (this->state != Sample ||
        !this->driver.getQuality())
    {
        return;
    }

    TunerDriver::QualityMode mode = TunerDriver::QUALITY_FAST;

    Serial.print(this->driver.getFrequency(), DEC);
    Serial.print('=');
    Utils::serialDecimal(this->driver.getQualityRssi(mode), 2);

    this->next();
}

void
Scan::init(void)
{
    /* TODO: Due to asynchronous nature of scanning,
       the message will be changed in new protocol */
    Serial.print('U');
    /* Currently, it can be easily corrupted by another
       message coming from a different controller */

    this->current = this->from;
    this->state = Sample;
}

void
Scan::next(void)
{
    this->current += this->step;

    if (this->current > this->to)
    {
        if (!this->repeat)
        {
            this->stop();
            return;
        }

        Serial.print('\n');
        this->init();
    }

    this->driver.setFrequency(this->current, TunerDriver::TUNE_SCAN);
    this->driver.resetQuality();
    Serial.print(',');
}
