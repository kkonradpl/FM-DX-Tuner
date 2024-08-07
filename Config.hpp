#ifndef FMDX_TUNER_CONFIG_H
#define FMDX_TUNER_CONFIG_H

/* -----------------------------------------------------------------------
   Generic tuner configuration
   ----------------------------------------------------------------------- */
/* Tuner driver (available drivers: TEF668X, SAF7730, SAF775X) */
/* Warning: SAF775X support is currently very experimental */
#define TUNER_DRIVER TEF668X

/* I2C bus clock speed */
/* Typical values: 100 kHz (100000L) or 400 kHz (400000L)
   Reduce speed to decrease interference (for bad PCB design) or when
   capacitors are added to the bus lines (the speed is limited) */
#define TUNER_I2C_CLOCK 400000L

/* I2C pull-ups in Arduino AVR driver */
/* Enable only on Arduino boards modified to 3.3V operation
   For 5V boards use external 10 kΩ pull-ups from SDA and SCL to 3.3V */
#define TUNER_AVR_PULLUP false

/* Legacy RDS message (R) in the communication protocol */
/* Keep enabled for pre-v1.2 XDR-GTK compatibility. New message additionally
   contains the first block (PI code), which is useful for direct
   passthrough to RDS Spy. When using the legacy message, the first block
   is reconstructed from the buffered PI code message (P) for RDS Spy. */
#define TUNER_LEGACY_RDS_MSG true
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   Audio configuration
   ----------------------------------------------------------------------- */
#define AUDIO_I2S_ENABLED true
/* ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
   STM32 configuration
   ----------------------------------------------------------------------- */
#define STM32_USE_INTERNAL_CLOCK false
/* ----------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
   LED configuration
   ----------------------------------------------------------------------- */
#define LED_ENABLED 0

#define LED_COUNT 1
#define LED_PINS 13

#define LED_ID_POWER 0
//#define LED_ID_COMMAND 1
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
