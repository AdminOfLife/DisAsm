/*
* Filename: LXOSType.h
* Author:   DisAsmCompany
* Date:     26/10/2013
*
* Description: LX OS Type
* 
*
*
*/

#pragma once
#ifndef __LXOSTYPE_H__D1E0EDF8_A303_4B49_995A_E3C76C517012__
#define __LXOSTYPE_H__D1E0EDF8_A303_4B49_995A_E3C76C517012__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static const SDFEnum LXOSType[] =
{
	{"Unknown",     0x00, 0},
	{"OS/2",        0x01, 0},
	{"Windows",     0x02, 0},
	{"DOS 4.x",     0x03, 0},
	{"Windows 386", 0x04, 0},
	{NULL, 0, 0}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LXOSTYPE_H__D1E0EDF8_A303_4B49_995A_E3C76C517012__ */
