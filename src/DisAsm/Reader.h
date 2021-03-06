/*
* Filename: ReaderReader.h
* Author:   DisAsmCompany
* Date:     27/09/2013
*
* Description: abstract Reader API
*              (e.g. read from memory,
*              file, HTTP, FTP, etc)
*
*/

#pragma once
#ifndef __READER_H__8BC98649_9F3A_4B24_B656_5B6E41194AC2__
#define __READER_H__8BC98649_9F3A_4B24_B656_5B6E41194AC2__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void * HREADER;

struct ReaderContext_t;

typedef uint8_t (*pfnRead)(struct ReaderContext_t * hReader, void * buffer, uint32_t size);
typedef uint8_t (*pfnSeek)(struct ReaderContext_t * hReader, uint64_t pos);
typedef uint8_t (*pfnSkip)(struct ReaderContext_t * hReader, int64_t count);
typedef uint8_t (*pfnSize)(struct ReaderContext_t * hReader, uint64_t * size);
typedef void (*pfnDestroy)(struct ReaderContext_t * hReader);

typedef struct ReaderContext_t
{
	pfnRead    pRead;
	pfnSeek    pSeek;
	pfnSkip    pSkip;
	pfnSize    pSize;
	pfnDestroy pDestroy;
	void *     pPrivate;
	uint64_t   base;
}
ReaderContext;

uint8_t ReaderRead(HREADER hReader, void * buffer, uint32_t size);
uint8_t ReaderSeek(HREADER hReader, uint64_t pos);
uint8_t ReaderSkip(HREADER hReader, int64_t count);
uint8_t ReaderSize(HREADER hReader, uint64_t * size);
uint8_t ReaderSetBase(HREADER hReader, uint64_t base);
void ReaderDestroy(HREADER hReader);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __READER_H__8BC98649_9F3A_4B24_B656_5B6E41194AC2__ */
