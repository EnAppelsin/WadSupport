#pragma once

void IdentifySharVersion();

void* GetPlayFMV_Ptr();
HWND Win32_GetHwnd();

void* SoundManager_Get();
void SoundManager_StopForMovie(void* sndmgr);
void SoundManager_ResumeAfterMovie(void* sndmgr);
bool SoundManager_IsStoppedForMovie(void* sndmgr);
void SoundManager_Update(void* sndmgr);
void SoundManager_UpdateOncePerFrame(void* sndmgr, unsigned int elapsedTime, int context, bool useContext, bool isPausedForErrors);
void radMovieService2();
void radFileService();