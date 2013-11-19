/*
 * Filename: MachODylib.h
 * Author:   DisAsmCompany
 * Date:     14/10/2013
 *
 * Description: Mach-O Dylib Structure
 *              (used in several load commands)
 *              
 *
 */

#pragma once
#ifndef __MACHODYLIB_H__
#define __MACHODYLIB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static const SDFElement MachODylib[] =
{
	{"Mach-O Dylib"},
	{"OffsetString        ", kUnsigned, 4, 1},
	{"TimeStamp           ", kUTC, 4, 1},
	{"CurrentVersion      ", kVersion, 4, 1},
	{"CompatibilityVersion", kVersion, 4, 1},
	{NULL}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MACHODYLIB_H__ */
