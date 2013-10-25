/*
 * Filename: ELFABI.h
 * Author:   DisAsmCompany
 * Date:     25/10/2013
 *
 * Description: ELF ABI definition
 *              (Application Binary Interface)
 *
 *
 */

#pragma once
#ifndef __ELFABI_H__
#define __ELFABI_H__

static const SDFEnum ELFABI[] =
{
	{"ELFOSABI_SYSV",       0x00},
	{"ELFOSABI_HPUX",       0x01},
	{"ELFOSABI_NETBSD",     0x02},
	{"ELFOSABI_LINUX",      0x03},
	{"ELFOSABI_HURD",       0x04},
	{"ELFOSABI_86OPEN",     0x05},
	{"ELFOSABI_SOLARIS",    0x06},
	{"ELFOSABI_AIX",        0x07},
	{"ELFOSABI_IRIX",       0x08},
	{"ELFOSABI_FREEBSD",    0x09},
	{"ELFOSABI_TRU64",      0x0A},
	{"ELFOSABI_MODESTO",    0x0B},
	{"ELFOSABI_OPENBSD",    0x0C},
	{"ELFOSABI_OPENVMS",    0x0D},
	{"ELFOSABI_NSK",        0x0E},
	{"ELFOSABI_AROS",       0x0F},
	{"ELFOSABI_ARM_AEABI",  0x40},
	{"ELFOSABI_ARM",        0x61},
	{"ELFOSABI_STANDALONE", 0xFF},
	{NULL}
};

#endif /* __ELFABI_H__ */
