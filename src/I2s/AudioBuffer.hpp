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

#ifndef FMDX_TUNER_I2S_AUDIO_BUFFER_H
#define FMDX_TUNER_I2S_AUDIO_BUFFER_H

#include <cstdint>

template <uint8_t ChunkSize,
          uint8_t Capacity>
class AudioBuffer
{
public:
    AudioBuffer()
    {
        static_assert((Capacity % 2) == 0, "Capacity must be even");
        this->clear();
    }

    void
    clear()
    {
        this->readIndex = 0;
        this->writeIndex = 0;
        this->ready = false;
        this->full = false;
    }

    uint8_t
    fill() const
    {
        return this->full ? Capacity : (Capacity + this->writeIndex - this->readIndex) % Capacity;
    }

    const uint32_t*
    pop()
    {
        if (!this->ready || 
            (!this->full && this->readIndex == this->writeIndex))
        {
            return (uint32_t*)silence;
        }

        uint32_t *ret = this->buffer + (this->readIndex * ChunkSize);
        this->readIndex = (this->readIndex + 1) % Capacity;
        this->full = false;
        return ret;
    }

    void
    push(const uint32_t *data)
    {
        constexpr uint8_t halfFill = Capacity / 2;
        const uint8_t currentFill = this->fill();

        uint16_t peak = 0;
        for (uint8_t i = 0; i < ChunkSize; i++)
        {
            const int16_t channelA = data[i] >> 16;
            const int16_t channelB = (int16_t) data[i];

            const uint16_t modA = (channelA > 0) ? channelA : -channelA;
            const uint16_t modB = (channelB > 0) ? channelB : -channelB;

            const uint16_t peakAB = std::max(modA, modB);
            peak = std::max(peak, peakAB);
        }

        if (currentFill == Capacity)
        {
            /* Drop one chunk from the beginning of the buffer */
            this->readIndex = (this->readIndex + 1) % Capacity;
        }
        else if (currentFill > halfFill)
        {
            const uint16_t threshold = 1 << (15 * (currentFill - halfFill) / halfFill);

            if (peak < threshold)
            {
                /* Drop new chunk */
                return;
            }
        }
        
        this->insert(data);

        if ((currentFill + 1) < halfFill)
        {
            const uint16_t threshold = 1 << (15 - (15 * currentFill / halfFill));

            if (peak < threshold)
            {
                /* Insert one chunk of silence */
                this->insert((uint32_t*)this->silence);
            }
        }
        
        if (this->writeIndex == this->readIndex)
        {
            this->full = true;
        }

        if (!this->ready && 
            this->writeIndex >= Capacity / 2)
        {
            this->ready = true;
        }
    }
private:
    void
    insert(const uint32_t *data)
    {
        const uint16_t pos = (uint16_t)this->writeIndex * ChunkSize;
        this->writeIndex = (this->writeIndex + 1) % Capacity;

        for (uint8_t i = 0; i < ChunkSize; i++)
        {
            this->buffer[pos + i] = data[i];
        }
    }

    static constexpr uint32_t silence[ChunkSize] = {0};
    static constexpr uint16_t size = (uint16_t)ChunkSize * Capacity;
    uint32_t buffer[size];

    uint8_t readIndex;
    uint8_t writeIndex;
    bool ready;
    bool full;
};

#endif
