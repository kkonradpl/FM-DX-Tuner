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
#include "../../../Config.hpp"
#include <stm32f0xx_hal.h>

#if STM32_USB_ENABLED
#include <tusb.h>
#endif

void
Platform_Stm32_Bootloader(void)
{
#if STM32_USB_ENABLED
    tud_disconnect();
#endif

    HAL_NVIC_DisableIRQ(PendSV_IRQn);
    HAL_NVIC_DisableIRQ(USB_IRQn);
    __HAL_RCC_USB_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);
    __HAL_RCC_DMA1_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(SPI1_IRQn);
    __HAL_RCC_SPI1_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(I2C1_IRQn);
    __HAL_RCC_I2C1_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(USART1_IRQn);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
    __HAL_RCC_USART1_CLK_DISABLE();
    __HAL_RCC_USART2_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
    HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);
    HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, 0xFF);
    HAL_GPIO_DeInit(GPIOB, 0xFF);
    HAL_GPIO_DeInit(GPIOC, 0xFF);
    HAL_GPIO_DeInit(GPIOD, 0xFF);

    HAL_RCC_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

    const uint32_t addr = 0x1FFFC800;
    __set_MSP(*(uint32_t *)addr);

    /* Jump to System Memory */
    void (*bootloader)(void) = (void (*)(void)) (*((uint32_t *)(addr + 4)));
    bootloader();
    while(1);
}

#endif /* ARDUINO_ARCH_STM32 */
