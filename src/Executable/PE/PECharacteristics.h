/*
* Filename: PECharacteristics.h
* Author:   DisAsmCompany
* Date:     17/10/2013
*
* Description: PE Characteristics
*              
*              
*
*/

#pragma once
#ifndef __PECHARACTERISTICS_H__
#define __PECHARACTERISTICS_H__

static const SDFEnum PECharacteristics[] =
{
    {"IMAGE_FILE_RELOCS_STRIPPED",         0x0001, 0x0001},
    {"IMAGE_FILE_EXECUTABLE_IMAGE",        0x0002, 0x0002},
    {"IMAGE_FILE_LINE_NUMS_STRIPPED",      0x0004, 0x0004},
    {"IMAGE_FILE_LOCAL_SYMS_STRIPPED",     0x0008, 0x0008},
    {"IMAGE_FILE_AGGRESIVE_WS_TRIM",       0x0010, 0x0010},
    {"IMAGE_FILE_LARGE_ADDRESS_AWARE",     0x0020, 0x0020},
    {"IMAGE_FILE_40h",                     0x0040, 0x0040},
    {"IMAGE_FILE_BYTES_REVERSED_LO",       0x0080, 0x0080},
    {"IMAGE_FILE_32BIT_MACHINE",           0x0100, 0x0100},
    {"IMAGE_FILE_DEBUG_STRIPPED",          0x0200, 0x0200},
    {"IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP", 0x0400, 0x0400},
    {"IMAGE_FILE_NET_RUN_FROM_SWAP",       0x0800, 0x0800},
    {"IMAGE_FILE_SYSTEM",                  0x1000, 0x1000},
    {"IMAGE_FILE_DLL",                     0x2000, 0x2000},
    {"IMAGE_FILE_UP_SYSTEM_ONLY",          0x4000, 0x4000},
    {"IMAGE_FILE_BYTES_REVERSED_HI",       0x8000, 0x8000},
    {NULL}
};

#endif /* __PECHARACTERISTICS_H__ */