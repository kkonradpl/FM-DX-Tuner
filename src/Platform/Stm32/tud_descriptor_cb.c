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
#include "SerialNumber.h"
#include "../../Version.hpp"

enum
{
    ITF_NUM_AUDIO_CONTROL = 0,
    ITF_NUM_AUDIO_STREAMING,
    ITF_NUM_CDC,
    ITF_NUM_CDC_DATA,
    ITF_NUM_COUNT
};

enum
{
    EP_NUM_CDC_OUT   = 0x04,
    EP_NUM_AUDIO_IN =  0x81,
    EP_NUM_CDC_NOTIF = 0x83,
    EP_NUM_CDC_IN    = 0x84
};

enum
{
    STR_IDX_LANGID = 0,
    STR_IDX_MANUFACTURER,
    STR_IDX_PRODUCT,
    STR_IDX_SERIAL_NUMBER,
    STR_IDX_NAME_CDC,
    STR_IDX_NAME_AUDIO,
    STR_IDX_COUNT
};

static char* const strings[] =
{
    (const char[]) { 0x09, 0x04 }, // 0: Language (English)
    "FMDX.org",                    // 1: Manufacturer
    "TEF668X Headless USB Tuner",  // 2: Product
    NULL,                          // 3: Serial number
    "TEF668X Serial",              // 4: CDC serial interface
    "TEF668X Audio",               // 5: Audio interface
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + \
                          CFG_TUD_AUDIO * FMDX_AUDIO_DESC_LEN + \
                          CFG_TUD_CDC * TUD_CDC_DESC_LEN)

#define BCD_CONVERT(x) ((x) / 10 * 16 + (x) % 10)
#define BCD_VERSION (BCD_CONVERT(FMDX_TUNER_VERSION_MAJOR) << 8) | \
                    (BCD_CONVERT(FMDX_TUNER_VERSION_MINOR))

uint8_t const*
tud_descriptor_device_cb(void)
{
    static const tusb_desc_device_t device =
    {
        .bLength            = sizeof(tusb_desc_device_t),
        .bDescriptorType    = TUSB_DESC_DEVICE,
        .bcdUSB             = 0x0200,

        .bDeviceClass       = TUSB_CLASS_MISC,
        .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
        .bDeviceProtocol    = MISC_PROTOCOL_IAD,
        .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

        .idVendor           = 0x1209,
        .idProduct          = 0x6687,
        .bcdDevice          = BCD_VERSION,

        .iManufacturer      = 0x01,
        .iProduct           = 0x02,
        .iSerialNumber      = 0x03,

        .bNumConfigurations = 0x01
    };

    return (uint8_t const*)&device;
}

uint8_t const*
tud_descriptor_configuration_cb(uint8_t index)
{
    static const uint8_t config[] =
    {
        TUD_CONFIG_DESCRIPTOR(/* config_num */ 1,
                              /* _itfcount  */ ITF_NUM_COUNT,
                              /* _stridx    */ 0,
                              /* _total_len */ CONFIG_TOTAL_LEN,
                              /* _attribute */ 0x00,
                              /* _power_ma  */ 500),

        FMDX_AUDIO_DESCRIPTOR(/* _itfnum             */ ITF_NUM_AUDIO_CONTROL,
                              /* _stridx             */ STR_IDX_NAME_AUDIO,
                              /* _nBytesPerSample    */ CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX,
                              /* _nBitsUsedPerSample */ CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX * 8,
                              /* _epin               */ EP_NUM_AUDIO_IN,
                              /* _epsize             */ CFG_TUD_AUDIO_EP_SZ_IN),

        TUD_CDC_DESCRIPTOR(/* _itfnum        */ ITF_NUM_CDC,
                           /* _stridx        */ STR_IDX_NAME_CDC,
                           /* _ep_notif      */ EP_NUM_CDC_NOTIF,
                           /* _ep_notif_size */ 8,
                           /* _epout         */ EP_NUM_CDC_OUT,
                           /* _epin          */ EP_NUM_CDC_IN,
                           /* _epsize        */ 64)
    };

    return config;
}

uint16_t const*
tud_descriptor_string_cb(uint8_t  index,
                         uint16_t langid)
{
    if (index >= STR_IDX_COUNT)
    {
        return NULL;
    }

    static uint16_t buffer[32 + 1];
    size_t length;

    switch (index)
    {
    case STR_IDX_LANGID:
        memcpy(&buffer[1], strings[0], 2);
        length = 1;
        break;

    case STR_IDX_SERIAL_NUMBER:
        length = Platform_Stm32_SerialNumber_Read(buffer + 1);
        break;

    default:
        const char *string = strings[index];
        const size_t maxLength = sizeof(buffer) / sizeof(buffer[0]) - 1;
        if (length > maxLength)
            length = maxLength;

        for (size_t i = 0; i < length; i++)
        {
            buffer[1 + i] = string[i];
        }
      break;
  }

    buffer[0] = (TUSB_DESC_STRING << 8);
    buffer[0] |= 2 * (length + 1);
    return buffer;
}

#endif /* ARDUINO_ARCH_STM32 */

