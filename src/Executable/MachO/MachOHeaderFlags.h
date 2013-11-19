/*
 * Filename: MachOHeaderFlahs.h
 * Author:   DisAsmCompany
 * Date:     21/10/2013
 *
 * Description: Mach-O Header Flags
 *             
 *              
 *
 */

#pragma once
#ifndef __MACHOHEADERFLAGS_H__
#define __MACHOHEADERFLAGS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static const SDFEnum MachOHeaderFlags[] =
{
	{"MH_NOUNDEFS",                0x00000001UL, 0x00000001UL},
	{"MH_INCRLINK",                0x00000002UL, 0x00000002UL},
	{"MH_DYLDLINK",                0x00000004UL, 0x00000004UL},
	{"MH_BINDATLOAD",              0x00000008UL, 0x00000008UL},
	{"MH_PREBOUND",                0x00000010UL, 0x00000010UL},
	{"MH_SPLIT_SEGS",              0x00000020UL, 0x00000020UL},
	{"MH_LAZY_INIT",               0x00000040UL, 0x00000040UL},
	{"MH_TWOLEVEL",                0x00000080UL, 0x00000080UL},
	{"MH_FORCE_FLAT",              0x00000100UL, 0x00000100UL},
	{"MH_NOMULTIDEFS",             0x00000200UL, 0x00000200UL},
	{"MH_NOFIXPREBINDING",         0x00000400UL, 0x00000400UL},
	{"MH_PREBINDABLE",             0x00000800UL, 0x00000800UL},
	{"MH_ALLMODSBOUND",            0x00001000UL, 0x00001000UL},
	{"MH_SUBSECTIONS_VIA_SYMBOLS", 0x00002000UL, 0x00002000UL},
	{"MH_CANONICAL",               0x00004000UL, 0x00004000UL},
	{"MH_WEAK_DEFINES",            0x00008000UL, 0x00008000UL},
	{"MH_BINDS_TO_WEAK",           0x00010000UL, 0x00010000UL},
	{"MH_ALLOW_STACK_EXECUTION",   0x00020000UL, 0x00020000UL},
	{"MH_ROOT_SAFE",               0x00040000UL, 0x00040000UL},
	{"MH_SETUID_SAFE",             0x00080000UL, 0x00080000UL},
	{"MH_NO_REEXPORTED_DYLIBS",    0x00100000UL, 0x00100000UL},
	{"MH_PIE",                     0x00200000UL, 0x02000000UL},
	{"MH_DEAD_STRIPPABLE_DYLIB",   0x00400000UL, 0x04000000UL},
	{"MH_HAS_TLV_DESCRIPTORS",     0x00800000UL, 0x00800000UL},
	{"MH_NO_HEAP_EXECUTION",       0x01000000UL, 0x01000000UL},
	{NULL}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MACHOHEADERFLAGS_H__ */
