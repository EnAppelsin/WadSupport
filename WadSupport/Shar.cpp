#include "pch.h"

#include "HackFunctions.h"
#include "Shar.h"
#include "Rad_RadFile.h"

// Shar.cpp : All SHAR functions using hardcoded memory and fun disgusting stuff

static int SharVersion = 0;

inline static void* Choose(unsigned int originalEnglish, unsigned int international, unsigned int bestSellerSeries, unsigned int demo)
{
	void* releases[] = { (void*)originalEnglish, (void*)international, (void*)bestSellerSeries, (void*)demo };
	return releases[SharVersion];
}

void IdentifySharVersion()
{
	SharVersion = Hack_GameRelease();
}

void* GetPlayFMV_Ptr()
{
	return Choose(0x465410, 0x465180, 0x464F40, 0x4654C0);
}

void rad::FileOpenSync(rad::IRadFile** pFile, const char* pFileName, bool write, FileFlags flags, int priority, unsigned int cachesize, int allocator, int cacheSpace)
{
	void* funcptr = Choose(0x562B46, 0x562FDE, 0x573B7E, 0x562B9E);
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
	HWND* wndptr = (HWND*)Choose(0x6C89BC, 0x6C897C, 0x6C89B4, 0x6C897C);
	return *wndptr;
}

void* SoundManager_Get()
{
	void** sndmgr_ptr = (void**)Choose(0x6C8590, 0x6C8550, 0x6C8588, 0x6C8550);
	return *sndmgr_ptr;
}

void SoundManager_StopForMovie(void* sndmgr)
{
	void* funcptr = Choose(0x4C2500, 0x4C28E0, 0x4C2630, 0x4C2640);
	__asm
	{
		mov edi, sndmgr
		call funcptr
	}
}

void SoundManager_ResumeAfterMovie(void* sndmgr)
{
	void* funcptr = Choose(0x4C2540, 0x4C2920, 0x4C2670, 0x4C2680);
	__asm
	{
		mov esi, sndmgr
		call funcptr
	}
}

bool SoundManager_IsStoppedForMovie(void* sndmgr)
{
	void* funcptr = Choose(0x4C2570, 0x4C2950, 0x4C26A0, 0x4C26B0);
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
	void* funcptr = Choose(0x4C18F0, 0x4C1CD0, 0x4C1A20, 0x4C1A30);
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
	void* funcptr = Choose(0x4C1940, 0x4C1D20, 0x4C1A70, 0x4C1A80);
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
	char** srmip = (char**)Choose(0x6C8554, 0x6C8514, 0x6C854C, 0x6C8514);
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
	void* funcptr = Choose(0x55E760, 0x55EC50, 0x57DD90, 0x55E810);
	__asm
	{
		call funcptr
	}
}

void radFileService()
{
	void* funcptr = Choose(0x567DED, 0x568225, 0x578906, 0x567E2A);
	__asm
	{
		call funcptr
	}
}
