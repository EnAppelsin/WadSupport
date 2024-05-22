#include "pch.h"

#include "HackFunctions.h"
#include "Shar.h"
#include "Rad_RadFile.h"

// Shar.cpp : All SHAR functions using hardcoded memory and fun disgusting stuff

static int SharVersion = 0;

inline static void* Choose(unsigned int rel0, unsigned int rel1, unsigned int rel2, unsigned int rel3)
{
	void* releases[] = { (void*)rel0, (void*)rel1, (void*)rel2, (void*)rel3 };
	return releases[SharVersion];
}

void IdentifySharVersion()
{
	SharVersion = Hack_GameRelease();
}

void* GetPlayFMV_Ptr()
{
	return Choose(0x465410, 0x4654c0, 0x465180, 0x464f40);
}

void rad::FileOpenSync(rad::IRadFile** pFile, const char* pFileName, bool write, FileFlags flags, int priority, unsigned int cachesize, int allocator, int cacheSpace)
{
	void* funcptr = Choose(0x562b46, 0x562b06, 0x562b06, 0x562b3e);
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
	HWND* wndptr = (HWND*)Choose(0x6c89bc, 0x6c897c, 0x6c897c, 0x6c89b4);
	return *wndptr;
}

void* SoundManager_Get()
{
	void** sndmgr_ptr = (void**)Choose(0x6c8590, 0x6c8550, 0x6c8550, 0x6c8588);
	return *sndmgr_ptr;
}

void SoundManager_StopForMovie(void* sndmgr)
{
	void* funcptr = Choose(0x4c2500, 0x4c2640, 0x4c28e0, 0x4c2630);
	__asm
	{
		mov edi, sndmgr
		call funcptr
	}
}

void SoundManager_ResumeAfterMovie(void* sndmgr)
{
	void* funcptr = Choose(0x4c2540, 0x4c2680, 0x4c2920, 0x4c2670);
	__asm
	{
		mov esi, sndmgr
		call funcptr
	}
}

bool SoundManager_IsStoppedForMovie(void* sndmgr)
{
	void* funcptr = Choose(0x4c2570, 0x4c26b0, 0x4c2950, 0x4c26a0);
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
	void* funcptr = Choose(0x4c18f0, 0x4c1a30, 0x4c1cd0, 0x4c1a20);
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
	void* funcptr = Choose(0x4c1940, 0x4c1a80, 0x4c1d20, 0x4c1a70);
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

bool SoundManager_IsMuted(void* sndmgr)
{
	char* ptr = (char*)sndmgr + 0x48;
	return *ptr != 0;
}

void* SoundTuner_Get()
{
	char** srmip = (char**)Choose(0x6c8554, 0x6c8514, 0x6c8514, 0x6c854c);
	char* srmi = *srmip;
	char** tuner = (char**)(srmi + 0x34);
	return (void *)*tuner;
}

float SoundTuner_GetMasterVolume(void* tuner)
{
	float* masterVolume = (float*)((char*)tuner + 0x118);
	return *masterVolume;
}

float SoundTuner_GetMusicVolume(void* tuner)
{
	float* musicVolume = (float*)((char*)tuner + 0x124);
	return *musicVolume;
}

float SoundTuner_GetSfxVolume(void* tuner)
{
	float* sfxVolume = (float*)((char*)tuner + 0x11c);
	return *sfxVolume;
}

void radMovieService2()
{
	void* funcptr = Choose(0x55e760, 0x55e810, 0x55ec50, 0x57dd90);
	__asm
	{
		call funcptr
	}
}

void radFileService()
{
	void* funcptr = Choose(0x567ded, 0x567e2a, 0x568225, 0x578906);
	__asm
	{
		call funcptr
	}
}
