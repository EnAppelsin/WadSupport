#include "pch.h"

#include "HackFunctions.h"
#include "MinHook.h"
#include "Shar.h"
#include "SharDoomIo.h"
#include "Rad_RadFile.h"
#include "resource1.h"
#include <stdio.h>
#include <filesystem>
#include <sstream>
#include <SDL.h>
#include <vector>
#include <cassert>

extern HMODULE dllHandle;

// DOOM
extern "C" {
#include <doomgeneric.h>
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

extern "C"  __declspec(dllexport)  unsigned int __cdecl HackEntryPoint(HackEvent, void*);

extern "C" unsigned int __cdecl HackEntryPoint(HackEvent event, void* data)
{
#ifdef DEBUG_HACKENTRYPOINT
	if (Hack_Printf != 0)
	{
		debug_printf(L"HackEntryPoint(%d, %p)\n", event, data);
	}
#endif
	if (event == HackEvent::InstallHacks)
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
			printf(L"WAD Support: Did not create the PlayFMV Hook\n");
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
	else if (event == HackEvent::Process && DG_ConfigDir == NULL)
	{
		debug_printf(L"WAD Support: Discovering savedata directory\n");
		// Try to discover the directory for SavedGames in a stupid way, we will put a blank file there and try to open it
		// Then we get the handle
		// In the future we might reuse this to store something idk
		rad::IRadFile* file = nullptr;
		rad::FileOpenSync(&file, "/UserData/SavedGames/wadsupport.ini", true, rad::FileFlags::OpenOrCreate);
		if (file)
		{
			const char* fname = file->GetFilename();
			const char* dname = file->GetDrivename();
			//unsigned int handle = file->GetHandle();
			HANDLE handle = *((HANDLE*)((char*)file + 0x154));
			debug_printf(L"SHAR I/O Test: (%S, %S, %x)\n", fname, dname, handle);
			// No idea if this is always the case
			char* lucas_cstream = *((char**)((char*)handle + 0x08));
			handle = *((HANDLE*)(lucas_cstream + 0x28));
			wchar_t fpath[256] = { 0 };
			DWORD rv = GetFinalPathNameByHandle((HANDLE)handle, fpath, 256, 0);
			debug_printf(L"WinAPI Path: %s\n", fpath);
			// it's in experimental because I guess VS2017 because of minhook, ugh
			std::experimental::filesystem::path p(&fpath[4]);
			p.remove_filename();
			printf(L"WAD Support: Saving data in: %s\n", p.wstring().c_str());
			DG_ConfigDir = new char[p.string().length() + 1];
			strcpy_s(DG_ConfigDir, p.string().length() + 1, p.string().c_str());
			
			file->Release();
		}
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

	const char* wadfile = strstr(fileName, ".wad");
	const char* dclfile = strstr(fileName, ".dcl");
	if (wadfile || dclfile)
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
		if (Hack_IsHackLoaded(L"NoAudio") || SoundManager_IsMuted(smgr))
		{
			args.push_back("-nosound");
		}
		// If its a .wad we just load it as an IWAD
		if (wadfile)
		{
			args.push_back("-iwad");
			args.push_back(std::string("wads\\") + fileName);
		}
		// If we're a .dcl we will read it to generate the command line arguments
		else
		{
			rad::IRadFile* dcl = 0;
			std::string dclpath = "wads\\";
			dclpath += fileName;
			rad::FileOpenSync(&dcl, dclpath.c_str());
			unsigned int size = 0;
			dcl->GetSizeSync(&size);
			assert(size > 0 && size < 65535);
			char* buf = new char[size];
			dcl->ReadSync(buf, size);
			dcl->Release();
			std::stringstream sbuf;
			sbuf.write(buf, size);
			delete[] buf;
			while (sbuf.good())
			{
				std::string line;
				std::getline(sbuf, line);
				line.erase(std::remove_if(line.begin(), line.end(), [](auto ch)
					{
						return isprint(ch) == 0;
					}), line.end());
				if (line.length())
					args.push_back(line);
			}
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
		// Restore the HWND
		DG_RestoreHwnd();
		DG_Cleanup();
		// DOOM doesn't close its WAD handles so we force them shut here
		SharDoomCloseAll();


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