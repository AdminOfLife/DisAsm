/*
 * Filename: MachOHeader.h
 * Author:   DisAsmCompany
 * Date:     02/10/2013
 *
 * Description: Mach-O header
 *
 *
 *
 */

#pragma once
#ifndef __MACHOHEADER_H__
#define __MACHOHEADER_H__

#define MachOHeaderCountCommands 1

static const SDFElement MachOHeader[] =
{
	{"Mach-O Header"},
	{"Magic        ", 0, kUnsigned, 4, 1},
	{"CpuType      ", 0, kUnsigned, 4, 1},
	{"CpuSubType   ", 0, kUnsigned, 4, 1},
	{"FileType     ", 0, kUnsigned, 4, 1},
	{"CountCommands", MachOHeaderCountCommands, kUnsigned, 4, 1},
	{"SizeCommands ", 0, kUnsigned, 4, 1},
	{"Flags        ", 0, kUnsigned, 4, 1},
};
static const uint32_t MachOHeaderSize = sizeof(MachOHeader) / sizeof(MachOHeader[0]);

#endif /* __MACHOHEADER_H__ */
