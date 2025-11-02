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
#include <stdbool.h>
#include "../../../Config.hpp"

#if STM32_USB_ENABLED
#include <tusb.h>

/* CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX + 1 for master channel */
#define CHANNELS (CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX + 1)

static const audio_desc_channel_cluster_t cluster =
{
    .bNrChannels = 1,
    .bmChannelConfig = (audio_channel_config_t) 0,
    .iChannelNames = 0
};

static const uint8_t clkValid = 1;
static const uint32_t sampleRate = CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE;
static audio_control_range_4_n_t(1) sampleRange =
{
    .wNumSubRanges = 1,
    .subrange[0].bMin = CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE,
    .subrange[0].bMax = CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE,
    .subrange[0].bRes = 0
};

static bool mute[CHANNELS] = {0};
static uint16_t volume[CHANNELS] = {0};
static const audio_control_range_2_n_t(1) volumeRange =
{
    .wNumSubRanges = 1,
    .subrange[0].bMin = 0,
    .subrange[0].bMax = 0,
    .subrange[0].bRes = 1
};


static inline bool
usb_audio_req_input_terminal(uint8_t                      rhport,
                            tusb_control_request_t const *p_request,
                            const uint8_t                 ctrlSel)
{
    switch (ctrlSel)
    {
    case AUDIO_TE_CTRL_CONNECTOR:
        return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, (void*)&cluster, sizeof(cluster));

    default:
        TU_BREAKPOINT();
        return false;
    }
}

static inline bool
usb_audio_req_feature_unit(uint8_t                      rhport,
                          tusb_control_request_t const *p_request,
                          const uint8_t                 ctrlSel,
                          const uint8_t                 channelNum)
{
    switch (ctrlSel)
    {
    case AUDIO_FU_CTRL_MUTE:
        return tud_control_xfer(rhport, p_request, &mute[channelNum], 1);

    case AUDIO_FU_CTRL_VOLUME:
        switch (p_request->bRequest)
        {
        case AUDIO_CS_REQ_CUR:
            return tud_control_xfer(rhport, p_request, &volume[channelNum], sizeof(volume[channelNum]));

        case AUDIO_CS_REQ_RANGE:
            return tud_audio_buffer_and_schedule_control_xfer(rhport, p_request, (void*)&volumeRange, sizeof(volumeRange));

        default:
            TU_BREAKPOINT();
            return false;
        }
        break;

    default:
        TU_BREAKPOINT();
        return false;
    }
}

static inline bool
usb_audio_req_clock_source(uint8_t                       rhport,
                           tusb_control_request_t const *p_request,
                           const uint8_t                 ctrlSel)
{
    switch (ctrlSel)
    {
    case AUDIO_CS_CTRL_SAM_FREQ:
        switch (p_request->bRequest)
        {
        case AUDIO_CS_REQ_CUR:
            return tud_control_xfer(rhport, p_request, &sampleRate, sizeof(sampleRate));

        case AUDIO_CS_REQ_RANGE:
            return tud_control_xfer(rhport, p_request, &sampleRange, sizeof(sampleRange));

        default:
            TU_BREAKPOINT();
            return false;
        }
        break;

    case AUDIO_CS_CTRL_CLK_VALID:
        return tud_control_xfer(rhport, p_request, &clkValid, sizeof(clkValid));

    default:
        TU_BREAKPOINT();
        return false;
    }
}

bool
tud_audio_get_req_entity_cb(uint8_t                       rhport,
                            tusb_control_request_t const *p_request)
{
    const uint8_t ctrlSel = TU_U16_HIGH(p_request->wValue);
    const uint8_t channelNum = TU_U16_LOW(p_request->wValue);
    const uint8_t entityID = TU_U16_HIGH(p_request->wIndex);

    switch (entityID)
    {
    case 1:
        return usb_audio_req_input_terminal(rhport, p_request, ctrlSel);

    case 2:
        return usb_audio_req_feature_unit(rhport, p_request, ctrlSel, channelNum);

    case 4:
        return usb_audio_req_clock_source(rhport, p_request, ctrlSel);

    default:
        TU_BREAKPOINT();
        return false;
    }
}

#endif /* STM32_USB_ENABLED */

#endif /* ARDUINO_ARCH_STM32 */
