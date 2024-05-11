#pragma once

#include "Rad_RadFile.h"

void IdentifySharVersion();

void* GetPlayFMV_Ptr();
HWND Win32_GetHwnd();

void* SoundManager_Get();
void SoundManager_StopForMovie(void* sndmgr);
void SoundManager_ResumeAfterMovie(void* sndmgr);
bool SoundManager_IsStoppedForMovie(void* sndmgr);
void SoundManager_Update(void* sndmgr);
void SoundManager_UpdateOncePerFrame(void* sndmgr, unsigned int elapsedTime, int context, bool useContext, bool isPausedForErrors);
bool SoundManager_IsMuted(void* sndmgr);

void* SoundTuner_Get();
float SoundTuner_GetMasterVolume(void* tuner);
float SoundTuner_GetMusicVolume(void* tuner);
float SoundTuner_GetSfxVolume(void* tuner);

void radMovieService2();
void radFileService();