/*
 * Filename: COFFSymbolClass.h
 * Author:   DisAsmCompany
 * Date:     25/10/2013
 *
 * Description: COFF Symbol Class
 *
 *
 *
 */

#pragma once
#ifndef __COFFSYMBOLCLASS_H__
#define __COFFSYMBOLCLASS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _COFFSymbolClass_t
{
	kCOFFSymbolClassExternal = 0x02,
	kCOFFSymbolClassFile     = 0x67
}
COFFSymbolClass;

static const SDFEnum _COFFSymbolClass[] =
{
	{"IMAGE_SYM_CLASS_NULL",             0x00},
	{"IMAGE_SYM_CLASS_AUTOMATIC",        0x01},
	{"IMAGE_SYM_CLASS_EXTERNAL",         kCOFFSymbolClassExternal},
	{"IMAGE_SYM_CLASS_STATIC",           0x03},
	{"IMAGE_SYM_CLASS_REGISTER",         0x04},
	{"IMAGE_SYM_CLASS_EXTERNAL_DEF",     0x05},
	{"IMAGE_SYM_CLASS_LABEL",            0x06},
	{"IMAGE_SYM_CLASS_UNDEFINED_LABEL",  0x07},
	{"IMAGE_SYM_CLASS_MEMBER_OF_STRUCT", 0x08},
	{"IMAGE_SYM_CLASS_ARGUMENT",         0x09},
	{"IMAGE_SYM_CLASS_STRUCT_TAG",       0x0A},
	{"IMAGE_SYM_CLASS_MEMBER_OF_UNION",  0x0B},
	{"IMAGE_SYM_CLASS_UNION_TAG",        0x0C},
	{"IMAGE_SYM_CLASS_TYPE_DEFINITION",  0x0D},
	{"IMAGE_SYM_CLASS_UNDEFINED_STATIC", 0x0E},
	{"IMAGE_SYM_CLASS_ENUM_TAG",         0x0F},
	{"IMAGE_SYM_CLASS_MEMBER_OF_ENUM",   0x10},
	{"IMAGE_SYM_CLASS_REGISTER_PARAM",   0x11},
	{"IMAGE_SYM_CLASS_BIT_FIELD",        0x12},
	{"IMAGE_SYM_CLASS_FAR_EXTERNAL",     0x44},
	{"IMAGE_SYM_CLASS_BLOCK",            0x64},
	{"IMAGE_SYM_CLASS_FUNCTION",         0x65},
	{"IMAGE_SYM_CLASS_END_OF_STRUCT",    0x66},
	{"IMAGE_SYM_CLASS_FILE",             kCOFFSymbolClassFile},
	{"IMAGE_SYM_CLASS_SECTION",          0x68},
	{"IMAGE_SYM_CLASS_WEAK_EXTERNAL",    0x69},
	{"IMAGE_SYM_CLASS_CLR_TOKEN",        0x6B},
	{"IMAGE_SYM_CLASS_END_OF_FUNCTION",  0xFF},
	{NULL}
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COFFSYMBOLCLASS_H__ */
