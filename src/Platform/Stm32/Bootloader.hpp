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

#ifndef FMDX_TUNER_PLATFORM_STM32_BOOTLOADER_H
#define FMDX_TUNER_PLATFORM_STM32_BOOTLOADER_H
#ifdef ARDUINO_ARCH_STM32

#include <stddef.h>

void Platform_Stm32_Bootloader(void);

#endif /* ARDUINO_ARCH_STM32 */

#endif /* FMDX_TUNER_PLATFORM_STM32_BOOTLOADER_H */
