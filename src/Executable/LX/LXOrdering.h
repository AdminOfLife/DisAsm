/*
* Filename: LXOrdering.h
* Author:   DisAsmCompany
* Date:     26/10/2013
*
* Description: LX Byte/Word Ordering
* 
*
*
*/

#pragma once
#ifndef __LXORDERING_H__
#define __LXORDERING_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static const SDFEnum LXOrdering[] =
{
	{"Little Endian", 0x00},
	{"Big Endian",    0x01},
	{NULL}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LXORDERING_H__ */
