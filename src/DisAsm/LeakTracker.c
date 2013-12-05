/*
 * Filename: LeakTracker.h
 * Author:   DisAsmCompany
 * Date:     11/11/2013
 *
 * Description: memory leaks tracker
 *
 *
 *
 */

#include "DisAsm"

/* OpCode for NOP (No Operation - instruction that does nothing) */
static const uint8_t nop = 0x90;
/* OpCode for JMP (Jump) */
static const uint8_t jmp = 0xE9;

enum {ThunkSize = 20 };
enum {PageSize = 4096 };

uint8_t xThunkHeapAlloc[2 * ThunkSize], xThunkHeapReAlloc[2 * ThunkSize], xThunkHeapFree[2 * ThunkSize], xThunkRtlFreeHeap[2 * ThunkSize];

typedef struct Allocation_t
{
	void * address;
    void * heap;
	uint32_t size;
	uint8_t freed;
	native_t callstack[MaxCallStack];
}
Allocation;

static size_t g_nActiveAllocations = 0;
static size_t g_nTotalAllocations = 0;
static size_t g_nCapacity = 0;

Allocation * g_Allocations = NULL;

static uint8_t xHeapCalled = false;

void * xAlloc(uint32_t size);
void * xReAlloc(void * address, uint32_t size);
void xFree(void * address);

uint32_t xFindAllocation(void * address, void * heap)
{
	uint32_t i = 0;
	for (i = 0; i < g_nTotalAllocations; ++i)
	{
		if (g_Allocations[i].address == address && g_Allocations[i].heap == heap)
		{
			return i;
		}
	}
	return 0xFFFFFFFFUL;
}

void xUpdateAllocation(void * address, void * heap, uint32_t size, uint8_t freed);

void xAddAllocation(void * address, void * heap, uint32_t size, uint8_t freed)
{
    size_t index = xFindAllocation(address, heap);
    if (0xFFFFFFFFUL != index)
    {
		/* already has entry for allocation, so just update it */
        xUpdateAllocation(address, heap, size, freed);
    }
    else
    {
        ++g_nActiveAllocations;
        ++g_nTotalAllocations;
	    if (g_nTotalAllocations >= g_nCapacity)
	    {
		    g_nCapacity += 1024; /* linear grow */
			if (!g_Allocations)
			{
				g_Allocations = (Allocation*) xAlloc(g_nCapacity * sizeof(Allocation));
			}
			else
			{
				g_Allocations = (Allocation*) xReAlloc(g_Allocations, g_nCapacity * sizeof(Allocation));
				if (!g_Allocations)
				{
					/* if realloc fails, then try to do new alloc, then copy data */
					Allocation * allocations = xAlloc(g_nCapacity * sizeof(Allocation));
					if (allocations)
					{
						memcpy(allocations, g_Allocations, (g_nCapacity - 1024) * sizeof(Allocation));
						xFree(g_Allocations);
						g_Allocations = allocations;
					}
				}
			}
			if (!g_Allocations)
		    {
#ifdef OS_WINDOWS
			    DebugBreak();
#endif /* OS_WINDOWS */
#ifdef OS_MACOSX
				__asm { int 3 };
#endif /* OS_MACOSX */
		    }
	    }
	    g_Allocations[g_nTotalAllocations - 1].address = address;
	    g_Allocations[g_nTotalAllocations - 1].heap    = heap;
        g_Allocations[g_nTotalAllocations - 1].size    = size;
	    g_Allocations[g_nTotalAllocations - 1].freed   = freed;
		StackWalk(g_Allocations[g_nTotalAllocations - 1].callstack, NULL);
    }
}

void xUpdateAllocation(void * address, void * heap, uint32_t size, uint8_t freed)
{
	size_t index = xFindAllocation(address, heap);
	if (0xFFFFFFFFUL != index)
	{
		g_Allocations[index].address = address;
        g_Allocations[index].heap    = heap;
		g_Allocations[index].size    = freed ? g_Allocations[index].size : size;
		g_Allocations[index].freed   = freed;
	}
	else
	{
		xAddAllocation(address, heap, size, freed);
		if (freed) --g_nActiveAllocations;
	}
}

native_t CalculateOffsetForJMP(native_t from, native_t to)
{
	/* 5 is size of JMP instruction itself */
	return to - from - 5;
}

uint32_t PatchLength(uint8_t * pData, uint8_t * pOut, uint32_t required)
{
    uint32_t total = 0;
	CallbackReader reader = {0};
    reader.pRead    = CallbackRead;
    reader.pPrivate = &reader;
    reader.buffer = pData;
    reader.offset = 0;
	
	while (total < required)
	{
        uint32_t length = DisAsmInstructionDecode(kBitnessNative, &reader, NULL);
		if (0 == length)
		{
			return 0;
		}
		if (jmp == pData[total])
		{
			uint32_t offset = 0;
			/* we're unlucky enough - patched function starts from relative jump (JMP) that need to be corrected */
			memcpy(&offset, pData + total + 1, 4);
			/* math is simple :
			 old offset = destination - original
			 destination = old offset + original
			 new offset = destination - thunk = old offset + original - thunk
			 */
			offset = offset + pData - pOut;
			memcpy(pOut + total + 1, &offset, 4);
			pOut[total] = jmp;
		}
        else if (0x0F == pData[total] && 0x80 <= pData[total + 1] && pData[total + 1] <= 0x8F)
        {
            /* 0x0F 0x8* Jcc Rel32 */
            uint32_t offset = 0;
            memcpy(&offset, pData + total + 2, 4);
            offset = offset + pData - pOut;
            memcpy(pOut + total + 2, &offset, 4);
            pOut[total] = 0x0F;
            pOut[total + 1] = pData[total + 1];
        }
		else
		{
			memcpy(pOut + total, pData + total, length);
		}
		total += length;
	}
	return total;
}

#ifdef OS_WINDOWS

typedef void * (__stdcall * pfnHeapAlloc)(void * hHeap, uint32_t flags, uint32_t size);
typedef void * (__stdcall * pfnHeapReAlloc)(void * hHeap, uint32_t flags, void * memory, uint32_t size);
typedef int (__stdcall * pfnHeapFree)(void * hHeap, uint32_t flags, LPVOID lpMem);

static pfnHeapFree    pOriginalRtlFreeHeap = NULL;
static pfnHeapAlloc   pOriginalHeapAlloc = NULL;
static pfnHeapReAlloc pOriginalHeapReAlloc = NULL;
static pfnHeapFree    pOriginalHeapFree = NULL;

void * __stdcall xHeapAlloc(void * heap, uint32_t flags, uint32_t size)
{
	void * result = NULL;
	if (xHeapCalled)
	{
		return pOriginalHeapAlloc(heap, flags, size);
	}
	xHeapCalled = true;
	result = pOriginalHeapAlloc(heap, flags, size);
	if (NULL != result)
	{
		xAddAllocation(result, heap, size, false);
	}
	xHeapCalled = false;
	return result;
}

void * __stdcall xHeapReAlloc(void * heap, uint32_t flags, void * address, uint32_t size)
{
	void * result = NULL;
	if (xHeapCalled)
	{
		return pOriginalHeapReAlloc(heap, flags, address, size);
	}
	xHeapCalled = true;
	result = pOriginalHeapReAlloc(heap, flags, address, size);
	if (NULL != result)
	{
		if (result == address) 
		{
			/* same address, just update it */
			xUpdateAllocation(result, heap, size, false);
		}
		else
		{
			/* new address : mark old as freed and add entry for new */
			xUpdateAllocation(address, heap, 0, true);
			xAddAllocation(result, heap, size, false);
		}
	}
	xHeapCalled = false;
	return result;
}

int __stdcall xHeapFree(void * heap, uint32_t flags, void * address)
{
	int result = 0;
	if (xHeapCalled)
	{
		return pOriginalHeapFree(heap, flags, address);
	}
	xHeapCalled = true;
	result = pOriginalHeapFree(heap, flags, address);
	if (result && NULL != address)
	{
		xUpdateAllocation(address, heap, 0, true);
	}
	xHeapCalled = false;
	return result;
}

int __stdcall xRtlFreeHeap(void * heap, uint32_t flags, void * address)
{
    int result = 0;
    if (xHeapCalled)
    {
        return pOriginalRtlFreeHeap(heap, flags, address);
    }
    xHeapCalled = true;
    if (NULL != address)
    {
        result = pOriginalRtlFreeHeap(heap, flags, address);
        if (result)
        {
            xUpdateAllocation(address, heap, 0, true);
        }
    }
    xHeapCalled = false;
    return result;
}

void * xAlloc(uint32_t size)
{
	return pOriginalHeapAlloc(GetProcessHeap(), 0, size);
}

void * xReAlloc(void * address, uint32_t size)
{
	return pOriginalHeapReAlloc(GetProcessHeap(), 0, address, size);
}

void xFree(void * address)
{
	pOriginalHeapFree(GetProcessHeap(), 0, address);
}

#else /* OS_WINDOWS */

typedef void * (*pfnmalloc)(size_t);
typedef void * (*pfnrealloc)(void *, size_t);
typedef void   (*pfnfree)(void *);
typedef void * (*pfncalloc)(size_t, size_t);

static pfnmalloc  pOriginalMalloc = NULL;
static pfncalloc  pOriginalCalloc = NULL;
static pfnrealloc pOriginalRealloc = NULL;
static pfnfree    pOriginalFree = NULL;

uint8_t xThunkMalloc[2 * ThunkSize], xThunkCalloc[2 * ThunkSize], xThunkRealloc[2 * ThunkSize], xThunkFree[2 * ThunkSize];

void * xmalloc(size_t size)
{
	void * result = NULL;
	if (xHeapCalled)
	{
		return pOriginalMalloc(size);
	}
	xHeapCalled = true;
	result = pOriginalMalloc(size);
	if (NULL != result)
	{
		xAddAllocation(result, NULL, size, false);
	}
	xHeapCalled = false;
	return result;
}

void * xcalloc(size_t count, size_t size)
{
	void * result = NULL;
	if (xHeapCalled)
	{
		return pOriginalCalloc(count, size);
	}
	xHeapCalled = true;
	result = pOriginalCalloc(count, size);
	if (NULL != result)
	{
		xAddAllocation(result, NULL, count * size, false);
	}
	xHeapCalled = false;
	return result;
}

void * xrealloc(void * address, size_t size)
{
	void * result = NULL;
	if (xHeapCalled)
	{
		return pOriginalRealloc(address, size);
	}
	xHeapCalled = true;
	result = pOriginalRealloc(address, size);
	if (NULL != result)
	{
		if (address == result) 
		{
			/* same address, just update it */
			xUpdateAllocation(result, NULL, size, false);
		}
		else
		{
			/* new address : mark old as freed and add entry for new */
			xUpdateAllocation(address, NULL, 0, true);
			xAddAllocation(result, NULL, size, false);
		}
	}
	xHeapCalled = false;
	return result;
}

void xfree(void * address)
{
	if (xHeapCalled)
	{
		pOriginalFree(address);
		return;
	}
	xHeapCalled = true;
	pOriginalFree(address);
	if (NULL != address)
	{
		xUpdateAllocation(address, NULL, 0, true);
	}
	xHeapCalled = false;
}

void * xAlloc(uint32_t size)
{
	return pOriginalMalloc(size);
}

void * xReAlloc(void * address, uint32_t size)
{
	return pOriginalRealloc(address, size);
}

void xFree(void * address)
{
	pOriginalFree(address);
}

#endif /* OS_WINDOWS */

typedef enum ProtectType_t
{
	ProtectTypeRead = 0x01,
	ProtectTypeWrite = 0x02,
	ProtectTypeExecute = 0x04
}
ProtectType;

void Protect(native_t address, ProtectType type)
{
#ifdef OS_WINDOWS
	DWORD protect = 0;
    switch (type)
    {
    case 0: protect = PAGE_NOACCESS; break;
    case ProtectTypeRead: protect = PAGE_READONLY; break;
    case ProtectTypeWrite: protect = PAGE_WRITECOPY; break;
    case ProtectTypeExecute: protect = PAGE_EXECUTE; break;
    case ProtectTypeRead | ProtectTypeWrite: protect = PAGE_READWRITE; break;
    case ProtectTypeRead | ProtectTypeExecute: protect = PAGE_EXECUTE_READ; break;
    case ProtectTypeExecute | ProtectTypeWrite: protect = PAGE_EXECUTE_WRITECOPY; break;
    case ProtectTypeExecute | ProtectTypeWrite | ProtectTypeRead: protect = PAGE_EXECUTE_READWRITE; break;
    default: break;
    }
	VirtualProtect((void*)(address - address % PageSize), PageSize, protect, &protect);
#else /* OS_WINDOWS */
	int protect = 0;
	protect |= (type & ProtectTypeRead) ? PROT_READ : 0;
	protect |= (type & ProtectTypeWrite) ? PROT_WRITE : 0;
	protect |= (type & ProtectTypeExecute) ? PROT_EXEC : 0;
	mprotect((void*)(address - address % PageSize), PageSize, protect);
#endif /* OS_WINDOWS */
}

void PatchFunctionX86(uint8_t * pOriginal, uint8_t * pHook, uint8_t * pThunk)
{
	uint32_t length = 0;
	native_t offset = CalculateOffsetForJMP((native_t)pOriginal, (native_t)pHook);
	
	memset(pThunk, nop, 2 * ThunkSize);
	
    if (0 == (length = PatchLength(pOriginal, pThunk, 5)))
    {
        return;
    }
	Protect((native_t) pThunk, ProtectTypeRead | ProtectTypeWrite | ProtectTypeExecute);
	Protect((native_t) pOriginal, ProtectTypeWrite);
	pOriginal[0] = jmp;
	memcpy(pOriginal + 1, &offset, 4);
	memset(pOriginal + 5, nop, length - 5);
	Protect((native_t) pOriginal, ProtectTypeExecute | ProtectTypeRead);
	
	offset = CalculateOffsetForJMP((native_t)(pThunk + ThunkSize), (native_t)(pOriginal + length));
	pThunk[ThunkSize] = jmp;
	memcpy(pThunk + ThunkSize + 1, &offset, 4);
}

void PatchFunctionX64(uint8_t * pOriginal, uint8_t * pHook, uint8_t * pThunk)
{
    uint32_t length = 0;
    native_t target = 0;
    memset(pThunk, nop, 2 * ThunkSize);

    if (0 == (length = PatchLength(pOriginal, pThunk, 14)))
    {
        return;
    }
    Protect((native_t) pThunk, ProtectTypeRead | ProtectTypeWrite | ProtectTypeExecute);
    Protect((native_t) pOriginal, ProtectTypeWrite);

    target = (native_t) pHook;
    pOriginal[0] = 0xFF;
    pOriginal[1] = 0x25;
    pOriginal[2] = 0x00;
    pOriginal[3] = 0x00;
    pOriginal[4] = 0x00;
    pOriginal[5] = 0x00;
    memcpy(pOriginal + 6, &target, 8);

    Protect((native_t) pOriginal, ProtectTypeExecute | ProtectTypeRead);

    target = (native_t) (pOriginal + length);
    pThunk[ThunkSize + 0] = 0xFF;
    pThunk[ThunkSize + 1] = 0x25;
    pThunk[ThunkSize + 2] = 0x00;
    pThunk[ThunkSize + 3] = 0x00;
    pThunk[ThunkSize + 4] = 0x00;
    pThunk[ThunkSize + 5] = 0x00;
    memcpy(pThunk + ThunkSize + 6, &target, 8);
}

void PatchFunction(uint8_t * pOriginal, uint8_t * pHook, uint8_t * pThunk)
{
#ifdef CPU_X86
    PatchFunctionX86(pOriginal, pHook, pThunk);
#endif /* CPU_X86 */
#ifdef CPU_X64
    PatchFunctionX64(pOriginal, pHook, pThunk);
#endif /* CPU_X64 */
}

void RestoreFunction(uint8_t * pOriginal, uint8_t * pThunk)
{
	Protect((native_t) pOriginal, ProtectTypeWrite);
	PatchLength(pThunk, pOriginal, 5);
	Protect((native_t) pOriginal, ProtectTypeExecute);
}

void LeakTrackerInstall(uint8_t install)
{
#ifdef CPU_X86
#ifdef OS_WINDOWS
    HMODULE hModule = GetModuleHandleA("ntdll.dll");
    void * RtlFreeHeap = (void*) GetProcAddress(hModule, "RtlFreeHeap");
#endif /* OS_WINDOWS */

	if (install)
	{
#ifdef OS_WINDOWS
		g_Allocations = (Allocation*)HeapAlloc(GetProcessHeap(), 0, sizeof(Allocation));

		pOriginalHeapFree    = (pfnHeapFree)   (&xThunkHeapFree);
		pOriginalHeapAlloc   = (pfnHeapAlloc)  (&xThunkHeapAlloc);
		pOriginalHeapReAlloc = (pfnHeapReAlloc)(&xThunkHeapReAlloc);
		pOriginalRtlFreeHeap = (pfnHeapFree)   (&xThunkRtlFreeHeap);
		/* need to capture RtlFreeHeap, because sometimes memory allocated by HeapAlloc
		is being freed directly by this function instead of HeapFree
		but if RtlFreeHeap is the same function as HeapFree, no need to patch twice */

		PatchFunction((uint8_t*)HeapReAlloc, (uint8_t*)xHeapReAlloc, xThunkHeapReAlloc);
		PatchFunction((uint8_t*)HeapAlloc,   (uint8_t*)xHeapAlloc,   xThunkHeapAlloc);
		PatchFunction((uint8_t*)HeapFree,    (uint8_t*)xHeapFree,    xThunkHeapFree);
		if (RtlFreeHeap != HeapFree)
		{
			PatchFunction((uint8_t*)RtlFreeHeap, (uint8_t*)xRtlFreeHeap, xThunkRtlFreeHeap);
		}
#else /* OS_WINDOWS */
		g_Allocations = (Allocation*) malloc(sizeof(Allocation));
		
		pOriginalMalloc = (pfnmalloc) (&xThunkMalloc);
		pOriginalCalloc = (pfncalloc) (&xThunkCalloc);
		pOriginalRealloc = (pfnrealloc) (&xThunkRealloc);
		pOriginalFree = (pfnfree) (&xThunkFree);
		
		PatchFunction((uint8_t*) malloc,  (uint8_t*) xmalloc,  xThunkMalloc);
		PatchFunction((uint8_t*) realloc, (uint8_t*) xrealloc, xThunkRealloc);
		PatchFunction((uint8_t*) calloc,  (uint8_t*) xcalloc,  xThunkCalloc);
		PatchFunction((uint8_t*) free,    (uint8_t*) xfree,    xThunkFree);
#endif /* OS_WINDOWS */
	}
	else
	{
		uint32_t i = 0;
		uint32_t j = 0;
		uint32_t total = 0;
#ifdef OS_WINDOWS
		RestoreFunction((uint8_t*)HeapAlloc,   xThunkHeapAlloc);
		RestoreFunction((uint8_t*)HeapReAlloc, xThunkHeapReAlloc);
		RestoreFunction((uint8_t*)HeapFree,    xThunkHeapFree);
		if (RtlFreeHeap != HeapFree)
		{
			RestoreFunction((uint8_t*)RtlFreeHeap, xThunkRtlFreeHeap);
		}
#endif /* OS_WINDOWS */
		for (i = 0; i < g_nTotalAllocations; ++i)
		{
			if (0 == g_Allocations[i].freed)
			{
				total += g_Allocations[i].size;
				ConsoleIOPrintFormatted("[ERROR] memory leak 0x%08X of %d bytes\n", g_Allocations[i].address, g_Allocations[i].size);

				for (j = 0; j < MaxCallStack; ++j)
				{
					if (0 == g_Allocations[i].callstack[j])
					{
						break;
					}
					ConsoleIOPrintFormatted("0x%08X ", g_Allocations[i].callstack[j]);
					StackWalkSymbol(g_Allocations[i].callstack[j]);
					ConsoleIOPrint("\n");
				}
				ConsoleIOPrint("\n");
			}
		}
		if (total > 0)
		{
			ConsoleIOPrintFormatted("[ERROR] total leaked %d bytes\n", total);
		}
	}
#endif /* CPU_X86 */
}
