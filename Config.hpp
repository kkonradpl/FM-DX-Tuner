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

#ifndef FMDX_TUNER_CONFIG_H
#define FMDX_TUNER_CONFIG_H

/* -----------------------------------------------------------------------
   Generic tuner configuration
   ----------------------------------------------------------------------- */
/* Tuner driver (available drivers: TEF668X, SAF7730) */
#define TUNER_DRIVER TEF668X
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   Antenna switch configuration
   ----------------------------------------------------------------------- */
#define ANTENNA_ENABLED 0
#define ANTENNA_COUNT   4
#define ANTENNA_PINS    8, 9, 10, 11

/* Delay of the antenna switch [miliseconds] */
#define ANTENNA_SWITCH_DELAY 50
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   Rotator controller configuration
   ----------------------------------------------------------------------- */
#define ROTATOR_ENABLED 0
#define ROTATOR_PIN_CW  6
#define ROTATOR_PIN_CCW 7

/* Automatic rotator stop after specified time [seconds] */
#define ROTATOR_TIMEOUT 90

/* Rotator delay between direction change [miliseconds] */
#define ROTATOR_DELAY 1000
/* ----------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
   External button configuration
   ----------------------------------------------------------------------- */
#define BUTTON_ENABLED 0
#define BUTTON_PIN 13

/* Button debounce time [miliseconds] */
#define BUTTON_DEBOUNCE 50
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   Serial port configuration
   ----------------------------------------------------------------------- */
#define SERIAL_PORT_SPEED 115200
/* ----------------------------------------------------------------------- */

#endif
