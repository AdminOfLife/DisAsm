/*
* Filename: MemoryReader.c
* Author:   DisAsmCompany
* Date:     27/09/2013
*
* Description: implementation of Reader API
*              for file system data source
*              
*
*/

#include "DisAsm"

typedef struct MemoryReaderContext_t
{
	uint8_t * buffer;
	uint32_t size;
	uint32_t offset;
}
MemoryReaderContext;

#define AccessPrivateData(x) ((MemoryReaderContext*)(x->pPrivate))

int MemoryReaderRead(ReaderContext * pContext, void * buffer, uint32_t size)
{
	memcpy(buffer, AccessPrivateData(pContext)->buffer + AccessPrivateData(pContext)->offset, size);
	AccessPrivateData(pContext)->offset += size;
	return 1;
}

int MemoryReaderSeek(ReaderContext * pContext, uint32_t pos)
{
	AccessPrivateData(pContext)->offset = pos;
	return 1;
}

void MemoryReaderDestroy(ReaderContext * pContext)
{
	free(pContext->pPrivate);
	free(pContext);
}

HREADER MemoryReaderCreate(void * buffer, uint32_t size)
{
	ReaderContext * pContext = NULL;
	MemoryReaderContext * pPrivate = NULL;
	if (NULL == buffer)
	{
		return NULL;
	}
	pContext = (ReaderContext*) malloc(sizeof(ReaderContext));
	if (NULL == pContext)
	{
		return NULL;
	}
	pPrivate = (MemoryReaderContext*) malloc(sizeof(MemoryReaderContext));
	if (NULL == pPrivate)
	{
		free(pContext);
		return NULL;
	}
	pPrivate->buffer = buffer;
	pPrivate->size = size;
	pPrivate->offset = 0;
	pContext->pRead    = MemoryReaderRead;
	pContext->pSeek    = MemoryReaderSeek;
	pContext->pDestroy = MemoryReaderDestroy;
	pContext->pPrivate = pPrivate;
	return (HREADER) pContext;
}
