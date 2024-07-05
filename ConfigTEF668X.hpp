#ifndef FMDX_TUNER_CONFIG_TEF668X_H
#define FMDX_TUNER_CONFIG_TEF668X_H

/* -----------------------------------------------------------------------
   TEF668X configuration
   ----------------------------------------------------------------------- */
/* Reference clock frequency, supported values:
 * 0 - GPIO selected 4/9.216 MHz
 * 4000000 - 4.000 MHz
 * 9216000 - 9.216 MHz
 * 12000000 - 12.000 MHz
 * 55466670 - 55.46667 MHz */
#define TUNER_TEF668X_REF_CLOCK 0

/* Reference clock selection pin (GPIO) */
#define TUNER_TEF668X_PIN_REF_CLOCK 15

/* RDS DAVN GPIO mode (true) or I2C polling (false) */
#define TUNER_TEF668X_RDS_DAVN false
/* RDS DAVN pin (GPIO) */
#define TUNER_TEF668X_PIN_RDS_DAVN PB7

/* DAC configuration via GPIO */
#define TUNER_TEF668X_DAC_CONF false
/* DAC configuration pin (GPIO) */
#define TUNER_TEF668X_PIN_DAC_CONF PB3

/* Versions of tuner patches embedded into firmware
   Warning: There is no enough space on AVR for all of them at once */
#define TUNER_TEF668X_PATCH_V101 0
#define TUNER_TEF668X_PATCH_V102 1
#define TUNER_TEF668X_PATCH_V205 1

/* FM signal level offset for calibration */
#define TUNER_TEF668X_FM_LEVEL_OFFSET -7.5

/* AM signal level offset for calibration */
#define TUNER_TEF668X_AM_LEVEL_OFFSET 0
/* ----------------------------------------------------------------------- */

#endif
