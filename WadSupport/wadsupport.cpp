#include "pch.h"

#include "HackFunctions.h"
#include "MinHook.h"
#include "Shar.h"
#include "SharDoomIo.h"
#include "Rad_RadFile.h"
#include "resource1.h"
#include <stdio.h>
#include <SDL.h>
#include <vector>

extern HMODULE dllHandle;

// DOOM
extern "C" {
#include <doomgeneric.h>
	void DG_SetHwnd(HWND hwnd);
}

#ifdef _DEBUG
#define printf(x, ...) Hack_Printf(2, 0, x, __VA_ARGS__)
#define debug_printf(x, ...) Hack_Printf(2, 0, x, __VA_ARGS__)
#else
#define printf(x, ...) Hack_Printf(2, 0, x, __VA_ARGS__)
#define debug_printf(x, ...) 
#endif

class FakePresentationManager
{
public:
	void PlayFMVOrDoom(void* thisptr, const char* fileName, void* callback, bool stop, bool local);
	static void* original;
};

void* FakePresentationManager::original = 0;

extern "C"  __declspec(dllexport)  unsigned int __cdecl HackEntryPoint(unsigned int, unsigned int*);

extern "C" unsigned int __cdecl HackEntryPoint(unsigned int event, unsigned int* data)
{
	if (event == 1 && data == 0)
	{
		InitialiseHack();
		IdentifySharVersion();
		debug_printf(L"WAD Support: Entry point reached %d %p\n", event, data);
		SDL_version linked;
		SDL_GetVersion(&linked);
		printf(L"WAD Support: Audio using SDL Version: %u.%u.%u.\n", linked.major, linked.minor, linked.patch);

		void* playfmv = GetPlayFMV_Ptr();
		FakePresentationManager::original = 0;

		if (MH_Initialize() != MH_OK)
		{
			printf(L"WAD Support: Did not initialise MinHook\n");
			return 1;
		}

		// Pointer heckery
		auto newfmv = &FakePresentationManager::PlayFMVOrDoom;
		void** newfmv_ptr = (void**)&newfmv;
		if (MH_CreateHook(playfmv, *newfmv_ptr, &FakePresentationManager::original) != MH_OK)
		{
			printf(L"WAD Support: Did create the PlayFMV Hook\n");
			return 1;
		}
		if (MH_EnableHook(playfmv) != MH_OK)
		{
			printf(L"WAD Support: Did not activate the PlayFMV Hook\n");
			return 1;
		}

		debug_printf(L"WAD Support: Hooked %p to %p\n", FakePresentationManager::original, *newfmv_ptr);

		debug_printf(L"Mod Launcher Game ID: %d\n", Hack_GameRelease());
		debug_printf(L"Is NoAudioLoaded?: %d\n", Hack_IsHackLoaded(L"NoAudio"));
	}
	return 0;
}

void FakePresentationManager::PlayFMVOrDoom(void* thisptr, const char* fileName, void* callback, bool stop, bool local)
{
	int skip = 0;
	// skip is passed in a register possibly?
	// weird optimiser nonsense?
	// it doesn't actually seem to be 1 or 0 but we keep it anyway for fun
	__asm
	{
		mov skip, eax
	}
	debug_printf(L"WAD Support: PlayFMV(%S, %p, %d, %d, %d)\n", fileName, callback, skip, stop, local);

	if (strstr(fileName, ".wad"))
	{
		// It's DOOMing time!
		// TODO: SHAR sets a flag in the FMV player for some gag related reason
		// It seems to not actually impact anything? Maybe Radical are just insane

		printf(L"WAD Support: Requested playback of WAD %S\n", fileName);

		debug_printf(L"WAD Support: Shutting down SHAR Audio\n");
		// Shutdown SHAR's audio
		void* smgr = SoundManager_Get();
		SoundManager_StopForMovie(smgr);
		while (!(SoundManager_IsStoppedForMovie(smgr)))
		{
			::radMovieService2();
			::radFileService();
			SoundManager_Update(smgr);
			SoundManager_UpdateOncePerFrame(smgr, 0, 0xC, false, false);
		}

		// Load the frame PNG into memory
		HRSRC hFrame = FindResource(dllHandle, MAKEINTRESOURCE(IDB_FRAME), L"PNG");
		HGLOBAL hFrameLoad = LoadResource(dllHandle, hFrame);
		void* frameMemory = LockResource(hFrameLoad);
		int frameSize = SizeofResource(dllHandle, hFrame);

		debug_printf(L"WAD Support: Preparing command line\n");
		std::vector<std::string> args;
#if _DEBUG
		std::string full_cmd = "";
#endif
		args.push_back("doom");
		args.push_back("-iwad");
		args.push_back(std::string("wads\\") + fileName);
		if (Hack_IsHackLoaded(L"NoAudio"))
		{
			args.push_back("-nosound");
		}
		
		int argc = args.size();
		char const** argv = new char const* [argc];
		for (int i = 0; i < argc; i++)
		{
#if _DEBUG
			full_cmd += args[i] + " ";
#endif
			argv[i] = args[i].c_str();
		}
#if _DEBUG
		debug_printf(L"DOOM Command Line: %S\n", full_cmd.c_str());
#endif

		// Preinit DOOM stuff
		DG_SetHwnd(Win32_GetHwnd());
		DG_LoadFrame(frameMemory, frameSize);
		DG_IOCallbacks(SharDoomOpen, SharDoomClose, SharDoomRead);
		// Save the window name
		char oldTitle[256];
		GetWindowTextA(Win32_GetHwnd(), oldTitle, 256);

		// Create DOOM with our fake command line
		doomgeneric_Create(argc, const_cast<char**>(argv));
		// DOOM ticks itself and pumps the message loop, SHAR doesn't run at all until DOOM exits
		for (int i = 0; DG_Exited != 1; i++)
		{
			doomgeneric_Tick();
		}
		// This doesn't cleanup everything (or get close) so someone who plays a lot of DOOM in SHAR 
		// may eventually leak too much to continue.
		// I don't think this is a practical problem
		debug_printf(L"WAD Support: DOOM loop ended, cleaning up some memory\n");
		DG_Cleanup();
		// DOOM doesn't close its WAD handles so we force them shut here
		SharDoomCloseAll();

		// Restore the Window title (back to SHAR)
		SetWindowTextA(Win32_GetHwnd(), oldTitle);

		// Let SHAR start playing sound again
		debug_printf(L"WAD Support: Resuming SHAR Audio\n");
		SoundManager_ResumeAfterMovie(smgr);

		// If callback is not null we should flag the movie as ended with it
		// But callback is null for loot.rmv and I am lazy :)
		debug_printf(L"WAD Support: Hoping callback %p is null\n", callback);

		return;
	}

	int intLocal = local;
	int intStop = stop;
	__asm
	{
		push intLocal
		push intStop
		push callback
		push fileName
		push thisptr
		mov eax, skip
		call FakePresentationManager::original
	}
}