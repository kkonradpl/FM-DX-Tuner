#ifndef FMDX_TUNER_CONFIG_H
#define FMDX_TUNER_CONFIG_H

/* -----------------------------------------------------------------------
   Generic tuner configuration
   ----------------------------------------------------------------------- */
/* Tuner driver (available drivers: TEF668X, SAF7730, SAF775X) */
/* Warning: SAF775X support is currently very experimental */
#define TUNER_DRIVER TEF668X

/* I2C pull-ups in Arduino AVR driver */
/* Enable only on Arduino boards modified to 3.3V operation
   For 5V boards use external 10 kΩ pull-ups from SDA and SCL to 3.3V */
#define TUNER_AVR_PULLUP false
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   Audio configuration
   ----------------------------------------------------------------------- */
#define AUDIO_I2S_ENABLED true
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   Antenna switch configuration
   ----------------------------------------------------------------------- */
#define ANTENNA_ENABLED 0

/* Antenna driver (available drivers: Antenna, AntennaBCD) */
#define ANTENNA_DRIVER Antenna

#define ANTENNA_COUNT 4
#define ANTENNA_PINS  8, 9, 10, 11

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
