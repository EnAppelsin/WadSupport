#include "pch.h"

#include "Shar.h"
#include "Rad_RadFile.h"

// Shar.cpp : All SHAR functions using hardcoded memory and fun disgusting stuff

static int SharVersion = 0;

void IdentifySharVersion()
{
	// Do something here to find the version of SHAR
	SharVersion = 0;
}

void rad::FileOpenSync(rad::IRadFile** pFile, const char* pFileName, bool write, int flags, int priority, unsigned int cachesize, int allocator, int cacheSpace)
{
	void* funcptr = (void*)0x562b46;
	int writeInt = write;
	__asm
	{
		push cacheSpace
		push allocator
		push cachesize
		push priority
		push flags
		push writeInt
		push pFileName
		push pFile
		call funcptr
		add esp, 32
	}
}

HWND Win32_GetHwnd()
{
	HWND* wndptr = (HWND*)0x006c89bc;
	return *wndptr;
}

void* SoundManager_Get()
{
	void** sndmgr_ptr = (void**)0x006c8590;
	return *sndmgr_ptr;
}

void SoundManager_StopForMovie(void* sndmgr)
{
	void* funcptr = (void*)0x4c2500;
	__asm
	{
		mov edi, sndmgr
		call funcptr
	}
}

void SoundManager_ResumeAfterMovie(void* sndmgr)
{
	void* funcptr = (void*)0x4c2540;
	__asm
	{
		mov esi, sndmgr
		call funcptr
	}
}

bool SoundManager_IsStoppedForMovie(void* sndmgr)
{
	void* funcptr = (void*)0x4c2570;
	int rv;
	__asm
	{
		mov esi, sndmgr
		call funcptr
		mov rv, eax
	}
	return ((rv & 0xff) != 0);
}

void SoundManager_Update(void* sndmgr)
{
	void* funcptr = (void*)0x4c18f0;
	__asm
	{
		mov edi, sndmgr
		call funcptr
	}
}

void SoundManager_UpdateOncePerFrame(void* sndmgr, unsigned int elapsedTime, int context, bool useContext, bool isPausedForErrors)
{
	// isPaused (0)
	// usecontect (0)
	// contect (0xc)
	// sndmgr
	// eax = elapsedTime
	void* funcptr = (void*)0x4c1940;
	int intIPFE = isPausedForErrors;
	int intUC = useContext;
	__asm
	{
		push intIPFE
		push intUC
		push context
		push sndmgr
		mov eax, elapsedTime
		call funcptr
	}
}

void radMovieService2()
{
	void* funcptr = (void*)0x55e760;
	__asm
	{
		call funcptr
	}
}

void radFileService()
{
	void* funcptr = (void*)0x567ded;
	__asm
	{
		call funcptr
	}
}