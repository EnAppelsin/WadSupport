#pragma once

// Functions from Hacks.dll

// Init all the function pointers
void InitialiseHack();

// Bare function pointer because variable forwarding nonsense
typedef void (*Hack_Printf_t)(int type, int category, const wchar_t* msg, ...);
extern Hack_Printf_t Hack_Printf;

// Get the game release
int Hack_GameRelease();
int Hack_GameLanguage();

// Is a hack loaded
bool Hack_IsHackLoaded(wchar_t const* name);

// TODO: I need to find the user data path somewhere (or I want to)