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

#ifndef FMDX_TUNER_DIRANA3_H
#define FMDX_TUNER_DIRANA3_H

namespace Dirana3
{

constexpr uint8_t I2C_Addr = 0x38;

enum Write : uint8_t
{
    Write_Radio_Tune = 0x00,
    Write_Radio_Bandwidth = 0x03,
    Write_Radio_Options = 0x05,
    Write_Radio_RDS = 0x09,
    Write_Audio = 0xF4,
    Write_Start = 0xE8
};

enum Read : uint8_t
{
    Read_Radio_Quality_Status = 0x00,
    Read_Radio_Quality_Data = 0x01,
    Read_Radio_RDS_Status = 0x07,
    Read_Radio_RDS_Data = 0x08,
    Read_Antenna1_Quality = 0x74,
    Read_Antenna2_Quality = 0x75
};

struct QualityData
{
    uint8_t level;
    uint8_t noise;
    uint8_t coChannel;
    int8_t offset;
    uint8_t bandwidth;
    uint8_t modulation;
};

struct RdsData
{
    uint8_t blockA_H;
    uint8_t blockA_L;
    uint8_t blockB_H;
    uint8_t blockB_L;
    uint8_t blockC_H;
    uint8_t blockC_L;
    uint8_t blockD_H;
    uint8_t blockD_L;
    uint8_t errors;
};

/* I2C data dump from Mini SAF775X Radio from Aliexpress */

const uint8_t BOOT[] PROGMEM =
{
    25, 0xE1, 0xF7, 0x7F, 0xC4, 0xE9, 0xE8, 0xBE, 0x1B, 0x94, 0x98, 0x0F, 0xE3, 0x1A, 0xDD, 0x72, 0x34, 0xB3, 0x91, 0x0A, 0x59, 0x3B, 0x80, 0xD4, 0x1D, 0xDE,
    25, 0xE1, 0x45, 0x0F, 0x00, 0x33, 0x6B, 0xD4, 0x77, 0x36, 0x3C, 0xC3, 0xA4, 0xF3, 0xF5, 0x6D, 0x94, 0xFB, 0x1E, 0xC9, 0xCF, 0x52, 0x19, 0xEE, 0xDD, 0xB1,
    25, 0xE1, 0xE8, 0x12, 0x3D, 0xA4, 0x93, 0xA2, 0x8C, 0x24, 0xC8, 0x20, 0xEF, 0x64, 0x82, 0xE3, 0xB6, 0x9A, 0xC4, 0x23, 0x8D, 0xDB, 0x22, 0x65, 0xD8, 0xE8,
    25, 0xE1, 0x8E, 0x32, 0x75, 0x14, 0xC1, 0x45, 0x8A, 0x19, 0x2A, 0x40, 0x71, 0x50, 0x7C, 0xFF, 0x3B, 0x5F, 0xC9, 0x50, 0x16, 0xD3, 0xDE, 0xE0, 0xC3, 0x92,
    25, 0xE1, 0x83, 0xBA, 0xC9, 0x2F, 0x52, 0xB9, 0xCF, 0xE7, 0x2F, 0x10, 0x9E, 0x09, 0x58, 0x63, 0x1D, 0xCE, 0x17, 0x69, 0x91, 0x26, 0x7C, 0x60, 0xBA, 0xB1,
    25, 0xE1, 0xEB, 0x2E, 0x8E, 0x0B, 0x4F, 0x35, 0x3B, 0x28, 0x14, 0xC8, 0xE1, 0x39, 0x7D, 0x0C, 0x5A, 0x65, 0x34, 0x3B, 0x83, 0x47, 0x73, 0xDB, 0x81, 0xBA,
    25, 0xE1, 0xF7, 0x7F, 0xC4, 0xE9, 0xE8, 0xBE, 0x1B, 0x94, 0x98, 0x0F, 0xE3, 0x1A, 0xDD, 0x72, 0x34, 0xB3, 0x91, 0x0A, 0x59, 0x3B, 0x80, 0xD4, 0x1D, 0xDE,
    25, 0xE1, 0x56, 0xB8, 0x45, 0x3A, 0x28, 0xEA, 0xC0, 0xB0, 0x2E, 0xBA, 0xA8, 0x12, 0x40, 0xB3, 0xD3, 0xC0, 0x25, 0x4F, 0xD1, 0xEE, 0xFF, 0xF7, 0xCB, 0xAA,
    25, 0xE1, 0xB5, 0x29, 0x6C, 0xD9, 0x2D, 0xCF, 0xAC, 0xBB, 0xED, 0x84, 0xB4, 0xDE, 0xC8, 0x76, 0xF0, 0x39, 0xD9, 0xA2, 0x87, 0xA2, 0x61, 0x7B, 0x60, 0x4B,
    25, 0xE1, 0x34, 0xFF, 0x44, 0x46, 0x4D, 0x66, 0x33, 0x64, 0x70, 0xF3, 0x41, 0x90, 0x4B, 0x6B, 0x09, 0xDA, 0x39, 0xCB, 0x4D, 0x56, 0x9E, 0x9B, 0xF1, 0x08,
    25, 0xE1, 0xF9, 0xAC, 0xB3, 0x03, 0x75, 0x0D, 0xA1, 0x8A, 0xD8, 0x6E, 0x8C, 0x21, 0x4D, 0xF0, 0x1A, 0xC8, 0x69, 0x17, 0xF5, 0xA0, 0x1B, 0x46, 0x8C, 0x45,
    25, 0xE1, 0xEB, 0xFC, 0x0A, 0x05, 0x68, 0x9B, 0x4E, 0x9B, 0xC7, 0xDF, 0x40, 0x11, 0x6E, 0xFA, 0x01, 0x2F, 0xD9, 0x3E, 0x60, 0xEA, 0xEB, 0x13, 0x3D, 0x73,
    25, 0xE1, 0x66, 0x8D, 0x66, 0x31, 0xA6, 0x3C, 0xD8, 0x9B, 0x8C, 0x22, 0x64, 0x5A, 0x79, 0xE5, 0xFF, 0x8B, 0x1A, 0x73, 0xA7, 0xBE, 0xE3, 0x2C, 0xE6, 0x9F,
    25, 0xE1, 0x06, 0xB3, 0x3E, 0xC0, 0x00, 0xF5, 0xFE, 0xBC, 0x22, 0xF8, 0xDD, 0xD7, 0xF3, 0xAF, 0x71, 0x01, 0xB2, 0x52, 0xC0, 0x63, 0x73, 0x02, 0xE1, 0x60,
    25, 0xE1, 0x4A, 0xD7, 0x5C, 0x81, 0xF7, 0x37, 0xCD, 0x4A, 0xFA, 0x60, 0x16, 0x0E, 0x25, 0xAE, 0xFA, 0x75, 0x17, 0xAD, 0xA7, 0x69, 0xFA, 0x6A, 0x8E, 0xEF,
    25, 0xE1, 0xF0, 0x0F, 0x4A, 0x45, 0x40, 0x60, 0x8E, 0x05, 0x0D, 0xC5, 0x79, 0x70, 0x7C, 0xEB, 0xD6, 0x3D, 0xC2, 0xEC, 0x9A, 0xE1, 0x3E, 0x4F, 0x31, 0x6C,
    25, 0xE1, 0x07, 0x6A, 0xB4, 0xFE, 0xAB, 0x73, 0x97, 0xC7, 0x99, 0x2C, 0x98, 0x46, 0xBC, 0xAA, 0xCC, 0xEF, 0x26, 0x8E, 0x3A, 0x7C, 0x84, 0x5E, 0x63, 0x8E,
    25, 0xE1, 0x97, 0xB3, 0x4E, 0xBF, 0xB6, 0xA2, 0x9C, 0x71, 0x82, 0xC9, 0xBC, 0x02, 0x7D, 0xC9, 0xB4, 0x14, 0x6C, 0x6F, 0x03, 0x88, 0xBD, 0x70, 0xF0, 0x86,
    25, 0xE1, 0xA4, 0xEE, 0x3A, 0xB5, 0xF8, 0x1B, 0x34, 0x3B, 0xFF, 0x99, 0x7A, 0x2F, 0x1D, 0x09, 0x3B, 0x77, 0x31, 0x63, 0x5D, 0x05, 0x40, 0xE8, 0x7F, 0x2E,
    25, 0xE1, 0x2E, 0xAA, 0x63, 0x2F, 0xC8, 0x6C, 0x00, 0x8A, 0xC9, 0xCC, 0x1B, 0xC5, 0xBC, 0xD2, 0xD8, 0x4D, 0xB6, 0x1E, 0x01, 0xC3, 0xD2, 0x89, 0x89, 0x07,
    25, 0xE1, 0x5F, 0xBA, 0x1B, 0xA0, 0x75, 0xC3, 0x0D, 0x40, 0x83, 0x83, 0x2A, 0x71, 0xC3, 0x60, 0xB3, 0x23, 0xC6, 0x88, 0x33, 0x27, 0x74, 0xE6, 0x4A, 0xFD,
    25, 0xE1, 0x9C, 0x40, 0xCC, 0x6B, 0x8C, 0x70, 0xE4, 0xD6, 0x01, 0xBB, 0xDB, 0x00, 0x45, 0x75, 0x2C, 0x68, 0xA1, 0x39, 0x33, 0x53, 0xF5, 0x4E, 0x9D, 0x8F,
    25, 0xE1, 0x6E, 0x24, 0x4B, 0x07, 0xDD, 0xC5, 0x43, 0x36, 0x0F, 0x71, 0x9D, 0x06, 0x64, 0x69, 0x77, 0x91, 0xD6, 0x58, 0x29, 0xB4, 0x41, 0xFF, 0xD6, 0x65,
    25, 0xE1, 0x2B, 0xC3, 0xA0, 0x0D, 0xC7, 0x85, 0xA4, 0x9D, 0x99, 0x4D, 0x5C, 0x15, 0x44, 0x08, 0x16, 0x52, 0x8E, 0x6C, 0xAA, 0xF5, 0xB5, 0x09, 0x08, 0x25,
    25, 0xE1, 0x34, 0x04, 0x67, 0xE1, 0x77, 0xC5, 0x34, 0xB7, 0xCD, 0x37, 0x32, 0x4E, 0x56, 0x9F, 0x77, 0xE8, 0x5C, 0x60, 0x13, 0x34, 0x7E, 0x44, 0xD3, 0xEE,
    25, 0xE1, 0x9A, 0xBE, 0xEB, 0x91, 0xCC, 0x98, 0xBC, 0x4D, 0x89, 0x8E, 0x0F, 0x18, 0x8C, 0x3A, 0xD3, 0xD2, 0x11, 0xA7, 0x49, 0xE8, 0x69, 0x4A, 0x3C, 0x22,
    25, 0xE1, 0x46, 0x8F, 0x9F, 0x06, 0xD3, 0x9E, 0x22, 0x38, 0xA0, 0x62, 0x28, 0x45, 0x26, 0xC4, 0x80, 0x72, 0xF4, 0x22, 0xF0, 0x4B, 0x45, 0xD9, 0xA1, 0x7D,
    25, 0xE1, 0xA8, 0x90, 0xF1, 0x8B, 0x74, 0xC3, 0x97, 0xA1, 0xEB, 0x25, 0x4C, 0x56, 0xBC, 0x29, 0x46, 0x85, 0xA1, 0x52, 0x22, 0x75, 0x3E, 0xBD, 0xC6, 0xFC,
    25, 0xE1, 0x54, 0xD9, 0x0D, 0x88, 0x02, 0x9C, 0x95, 0x51, 0xAE, 0x97, 0x91, 0xA1, 0xB8, 0x66, 0x33, 0xE4, 0x87, 0x1D, 0xC7, 0xB8, 0x00, 0x1E, 0x7B, 0x30,
    25, 0xE1, 0x4B, 0x76, 0xE7, 0x85, 0x74, 0xBB, 0x2E, 0x3E, 0x14, 0xFE, 0xAF, 0xAC, 0x37, 0xF0, 0x8A, 0xA2, 0x9D, 0x90, 0x9E, 0xC3, 0xC8, 0xC9, 0xEB, 0x22,
    25, 0xE1, 0x3C, 0xE5, 0x08, 0x7F, 0x39, 0xC4, 0x54, 0x2C, 0x3D, 0xFF, 0xAF, 0xF8, 0x1D, 0xF5, 0xDB, 0x8F, 0xE9, 0xEE, 0x9F, 0x1B, 0x38, 0x42, 0x96, 0x48,
    25, 0xE1, 0x59, 0xC7, 0x0D, 0x1F, 0xD5, 0xAB, 0x27, 0xDD, 0xA1, 0x19, 0xC2, 0x93, 0x53, 0xE1, 0x9F, 0x7D, 0x23, 0xC4, 0x74, 0xC7, 0x7B, 0x7A, 0x28, 0xBA,
    25, 0xE1, 0x59, 0x8F, 0x8B, 0xFC, 0x69, 0xEB, 0x27, 0x21, 0x90, 0xAA, 0xBE, 0xE8, 0xC9, 0x8A, 0xFC, 0xD8, 0xB7, 0x24, 0xD5, 0x4D, 0x54, 0x9D, 0x69, 0x86,
    25, 0xE1, 0x15, 0x1E, 0xC8, 0x14, 0x81, 0x38, 0xD7, 0x8F, 0xF1, 0x61, 0xD2, 0xC9, 0xCD, 0x61, 0x5A, 0xC2, 0xBC, 0xE0, 0x43, 0x35, 0x4D, 0xA8, 0x03, 0xC2,
    25, 0xE1, 0xC7, 0xF7, 0xD0, 0x71, 0x03, 0x45, 0xDF, 0x35, 0xE6, 0xF8, 0x93, 0x03, 0x0B, 0x2E, 0x22, 0x5E, 0x24, 0xA7, 0x4A, 0x04, 0xD3, 0x57, 0x33, 0xCF,
    25, 0xE1, 0x23, 0xBD, 0xE0, 0xD8, 0xAF, 0x67, 0x74, 0x98, 0x4A, 0xA2, 0xC9, 0xF9, 0x5D, 0x09, 0x36, 0x91, 0xD2, 0x77, 0xA6, 0x98, 0x22, 0xF1, 0x74, 0xB8,
    0
};

const uint8_t INIT[] PROGMEM = 
{
    3, 0xA9, 0x28, 0x01,
    2, 0x20, 0x00,
    2, 0x04, 0x00,
    2, 0x64, 0x04,
    4, 0x00, 0x10, 0x22, 0x74,
    2, 0x3F, 0x01,
    2, 0xA8, 0x00,
    3, 0xA9, 0x32, 0x00,
    3, 0xA9, 0x33, 0x00,
    6, 0xF3, 0x03, 0x82, 0x80, 0x00, 0x00,
    5, 0xF4, 0x40, 0x9B, 0x07, 0xFF,
    6, 0xF4, 0x02, 0xE3, 0xF4, 0x01, 0x67,
    6, 0xF4, 0x02, 0xE4, 0xF4, 0x01, 0x69,
    6, 0xF4, 0x02, 0xAC, 0xF4, 0x01, 0x67,
    6, 0xF4, 0x02, 0xAD, 0xF4, 0x01, 0x69,
    6, 0xF4, 0x02, 0xAE, 0xF4, 0x01, 0x67,
    6, 0xF4, 0x02, 0xAF, 0xF4, 0x01, 0x69,
    6, 0xF4, 0x02, 0xB0, 0xF4, 0x01, 0x6B,
    6, 0xF4, 0x02, 0xB1, 0xF4, 0x01, 0x67,
    3, 0xA9, 0x22, 0x00,
    3, 0xA9, 0x23, 0x00,
    6, 0xF4, 0x02, 0xAE, 0xF4, 0x01, 0x67,
    6, 0xF4, 0x02, 0xAF, 0xF4, 0x01, 0x69,
    6, 0xF4, 0x02, 0xB0, 0xF4, 0x01, 0x6B,
    6, 0xF4, 0x02, 0xB1, 0xF4, 0x01, 0x67,
    3, 0xA9, 0x16, 0x00,
    3, 0xA9, 0x17, 0x00,
    3, 0xA9, 0x18, 0x00,
    3, 0xA9, 0x19, 0x00,
    3, 0xA9, 0x60, 0x01,
    3, 0xA9, 0x61, 0x01,
    3, 0xA9, 0x62, 0x01,
    5, 0xF4, 0x40, 0x9B, 0x00, 0x00,
    3, 0xA9, 0x28, 0x01,
    2, 0x20, 0x00,
    5, 0xF4, 0x40, 0x9B, 0x07, 0xFF,
    0
};

const uint8_t FM[] PROGMEM =
{
    4, 0x00, 0x10, 0x28, 0xBE,
    2, 0x03, 0x00,
    2, 0x04, 0x01,
    2, 0x05, 0xEA,
    2, 0x06, 0x21,
    2, 0x07, 0x00,
    2, 0x19, 0x00,
    2, 0x1D, 0x00,
    7, 0xF4, 0x40, 0x84, 0x02, 0xD7, 0x03, 0x18,
    5, 0xF4, 0x46, 0x2E, 0x01, 0x7E,
    5, 0xF4, 0x46, 0x22, 0x00, 0xC2,
    5, 0xF4, 0x46, 0x34, 0x00, 0xC2,
    7, 0xF4, 0x40, 0x67, 0x07, 0xFF, 0x07, 0xFF,
    7, 0xF4, 0x40, 0x65, 0x07, 0xFF, 0x07, 0xFF,
    0
};

const uint8_t AM[] PROGMEM =
{
    4, 0x00, 0x11, 0x00, 0x99,
    2, 0x03, 0x00,
    2, 0x05, 0x6A, 
    2, 0x19, 0x00, 
    2, 0x1D, 0x00, 
    0
};

const uint16_t BANDWIDTH_FM[] PROGMEM
{
    720,
    890,
    1070,
    1240,
    1410,
    1590,
    1760,
    1930,
    2110,
    2280,
    2450,
    2620,
    2800,
    2970,
    3140,
    0
};

const uint16_t BANDWIDTH_AM[] PROGMEM
{
    18,
    22,
    26,
    30,
    34,
    40,
    46,
    52,
    60,
    70,
    80,
    92,
    104,
    120,
    136,
    0
};

};

#endif
