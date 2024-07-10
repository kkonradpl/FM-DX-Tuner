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
#include <tusb.h>
#include <stm32f0xx_hal.h>
#include "../../Comm.hpp"

void
PlatformDriver_Setup(void)
{
    __HAL_RCC_USB_CLK_ENABLE();
    HAL_NVIC_SetPriority(USB_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USB_IRQn);
    tud_init(0);
}

void
PlatformDriver_PostSetup(void)
{
    /* Override speed of output drivers to the lowest */
    GPIOA->OSPEEDR = 0;
    GPIOB->OSPEEDR = 0;
    GPIOC->OSPEEDR = 0;
    GPIOD->OSPEEDR = 0;
}

/* The USB_IRQHandler will take care of the 1ms isochronous
   interrupt for UAC2 audio and incoming CDC data (RX) */
extern "C" void
USB_IRQHandler(void)
{
    tud_int_handler(0);

    /* Continue in a lower priority interrupt */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* The PendSV_Handler will take care of the USB
   tasks in a lower priority interrupt */
extern "C" void
PendSV_Handler(void)
{
    tud_task_ext(0, true);

    if (Comm.availableTx())
    {
        while (Comm.availableTx())
        {
            uint8_t value = Comm.readTx();
            tud_cdc_n_write_char(0, value);

            if (value == '\n')
            {
                tud_cdc_n_write_flush(0);
            }
        }

        tud_task_ext(0, true);
    }
}

#endif /* ARDUINO_ARCH_STM32 */
