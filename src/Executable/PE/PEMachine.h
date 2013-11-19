/*
* Filename: PEMachine.h
* Author:   DisAsmCompany
* Date:     14/10/2013
*
* Description: PE Machine (Processor Architecture)
*              such as x86, x64, PowerPC, etc.
*              
*
*/

#pragma once
#ifndef __PEMACHINE_H__
#define __PEMACHINE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum PEMachine_t
{
	kPEMachineX86 = 0x014C,
	kPEMachineX64 = 0x8664
}
PEMachine;

static const SDFEnum _PEMachine[] =
{
	{"IMAGE_FILE_MACHINE_I386",      kPEMachineX86},
	{"IMAGE_FILE_MACHINE_R3000",     0x0162},
	{"IMAGE_FILE_MACHINE_R4000",     0x0166},
	{"IMAGE_FILE_MACHINE_R10000",    0x0168},
	{"IMAGE_FILE_MACHINE_WCEMIPSV2", 0x0169},
	{"IMAGE_FILE_MACHINE_ALPHA",     0x0184},
	{"IMAGE_FILE_MACHINE_SH3",       0x01A2},
	{"IMAGE_FILE_MACHINE_SH3DSP",    0x01A3},
	{"IMAGE_FILE_MACHINE_SH3E",      0x01A4},
	{"IMAGE_FILE_MACHINE_SH4",       0x01A6},
	{"IMAGE_FILE_MACHINE_SH5",       0x01A8},
	{"IMAGE_FILE_MACHINE_ARM",       0x01C0},
	{"IMAGE_FILE_MACHINE_THUMB",     0x01C2},
	{"IMAGE_FILE_MACHINE_AM33",      0x01D3},
	{"IMAGE_FILE_MACHINE_POWERPC",   0x01F0},
	{"IMAGE_FILE_MACHINE_POWERPCFP", 0x01F1},
	{"IMAGE_FILE_MACHINE_IA64",      0x0200},
	{"IMAGE_FILE_MACHINE_MIPS16",    0x0266},
	{"IMAGE_FILE_MACHINE_ALPHA64",   0x0284},
	{"IMAGE_FILE_MACHINE_MIPSFPU",   0x0366},
	{"IMAGE_FILE_MACHINE_MIPSFPU16", 0x0466},
	{"IMAGE_FILE_MACHINE_TRICORE",   0x0520},
	{"IMAGE_FILE_MACHINE_CEF",       0x0CEF},
	{"IMAGE_FILE_MACHINE_EBC",       0x0EBC},
	{"IMAGE_FILE_MACHINE_AMD64",     kPEMachineX64},
	{"IMAGE_FILE_MACHINE_M32R",      0x9041},
	{"IMAGE_FILE_MACHINE_CEE",       0xC0EE},
	{NULL}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PEMACHINE_H__ */
