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

#ifdef ARDUINO_ARCH_STM32
#include <Arduino.h>
#include <stm32f0xx_hal.h>
#include "../../../Config.hpp"
#include "../../Comm.hpp"

#if STM32_USB_ENABLED
#include <tusb.h>
void
PlatformDriver_Setup(void)
{
    __HAL_RCC_USB_CLK_ENABLE();
    HAL_NVIC_SetPriority(USB_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_IRQn);
    HAL_NVIC_SetPriority(PendSV_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(PendSV_IRQn);
    tud_init(0);
}
#endif /* STM32_USB_ENABLED */

void
PlatformDriver_PostSetup(void)
{
    /* Override speed of output drivers to the lowest */
    GPIOA->OSPEEDR = 0;
    GPIOB->OSPEEDR = 0;
    GPIOC->OSPEEDR = 0;
    GPIOD->OSPEEDR = 0;
}

#if STM32_USB_ENABLED
/* The USB_IRQHandler will take care of the 1ms isochronous
   interrupt for UAC2 audio and incoming CDC data (RX) */
extern "C" void
USB_IRQHandler(void)
{
    tud_int_handler(0);

    /* Continue in a lower priority interrupt */
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
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
#endif /* STM32_USB_ENABLED */

extern "C" void
SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = (STM32_USE_INTERNAL_CLOCK ? RCC_OSCILLATORTYPE_HSI : RCC_OSCILLATORTYPE_HSE);
#if STM32_USE_INTERNAL_CLOCK
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
#else
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
#endif
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = (STM32_USE_INTERNAL_CLOCK ? RCC_PLLSOURCE_HSI : RCC_PLLSOURCE_HSE);
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }

#if STM32_USB_ENABLED
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = (STM32_USE_INTERNAL_CLOCK ? RCC_USBCLKSOURCE_HSI48 : RCC_USBCLKSOURCE_PLL);

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

#if STM32_USE_INTERNAL_CLOCK
    RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};
    RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;
    RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;
    RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
    RCC_CRSInitStruct.ReloadValue =  RCC_CRS_RELOADVALUE_DEFAULT;
    RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;
    RCC_CRSInitStruct.HSI48CalibrationValue = RCC_CRS_HSI48CALIBRATION_DEFAULT;

    __HAL_RCC_CRS_CLK_ENABLE();
    HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);
#endif /* STM32_USE_INTERNAL_CLOCK */
#endif /* STM32_USB_ENABLED */
}

#endif /* ARDUINO_ARCH_STM32 */
