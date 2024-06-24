/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  FM-DX Tuner
 *  Copyright (C) 2024  Konrad Kosmatka
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

#ifdef ARDUINO_ARCH_STM32
#include <Arduino.h>
#include "UsbCdcStream.hpp"
#include "tusb.h"

#define BUFF_SIZE 64
uint8_t rxBuff[BUFF_SIZE];
uint8_t rxWritePos = 0;
uint8_t rxReadPos = 0;

uint8_t txBuff[BUFF_SIZE];
uint8_t txWritePos = 0;
uint8_t txReadPos = 0;

UsbCdcStream::operator bool()
{
    return true;
}

int
UsbCdcStream::available(void)
{
    return ((unsigned int)(BUFF_SIZE + rxWritePos - rxReadPos)) % BUFF_SIZE;
}

int
UsbCdcStream::read(void)
{
    if (!this->available())
    {
        return '\0';
    }

    uint8_t value = rxBuff[rxReadPos];
    rxReadPos = (rxReadPos + 1) % BUFF_SIZE;

    return value;
}

void
UsbCdcStream::flush(void)
{
    rxReadPos = rxWritePos;
}

int
UsbCdcStream::peek(void)
{
    if (!this->available())
    {
        return '\0';
    }

    return rxBuff[rxReadPos];
}

size_t
UsbCdcStream::write(uint8_t value)
{
    txBuff[txWritePos] = value;
    txWritePos = (txWritePos + 1) % BUFF_SIZE;

    /* Continue in an interrupt */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    return 1;
}

int
UsbCdcStream::availableTx(void)
{
    return ((unsigned int)(BUFF_SIZE + txWritePos - txReadPos)) % BUFF_SIZE;
}

uint8_t
UsbCdcStream::readTx(void)
{
    if (!this->availableTx())
    {
        return '\0';
    }

    uint8_t value = txBuff[txReadPos];
    txReadPos = (txReadPos + 1) % BUFF_SIZE;

    return value;
}

extern "C"
void tud_cdc_rx_cb(uint8_t itf)
{
    uint8_t value;
    while (tud_cdc_n_available(itf))
    {
        uint32_t count = tud_cdc_n_read(itf, &value, 1);
        if (count)
        {
            rxBuff[rxWritePos] = value;
            rxWritePos = (rxWritePos + 1) % BUFF_SIZE;
        }
    }
}


UsbCdcStream UsbCdcSerial;

#endif /* ARDUINO_ARCH_STM32 */
