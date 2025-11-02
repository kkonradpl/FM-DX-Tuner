/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2024-2025  Konrad Kosmatka
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

#ifndef FMDX_TUNER_PLATFORM_STM32_USB_CDC_STREAM_H
#define FMDX_TUNER_PLATFORM_STM32_USB_CDC_STREAM_H
#ifdef ARDUINO_ARCH_STM32
#include "../../../Config.hpp"

#if STM32_USB_ENABLED
#include <Arduino.h>

class UsbCdcStream : public Stream
{
public:
    void begin(unsigned long baud) {};
    void end();
    virtual int available(void);
    virtual int read(void);
    virtual void flush(void);
    virtual int peek(void);

    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }

    using Print::write;
    operator bool();

    int availableTx(void);
    uint8_t readTx(void);
};

extern UsbCdcStream UsbCdcSerial;

#endif /* STM32_USB_ENABLED */

#endif /* ARDUINO_ARCH_STM32 */

#endif /* FMDX_TUNER_PLATFORM_STM32_USB_CDC_STREAM_H */