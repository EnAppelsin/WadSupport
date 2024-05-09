# WadSupport

Support WADs in SHAR, for sensible and practical reasons.

This is literally DOOM (doomgeneric, Chocolate Doom) inside SHAR.
Yes it is stupid.

To do so just ask to play an FMV (like a gag) but specify a file ending in .wad instead of .rmv
Instead of an FMV, DOOM will start with the chosen IWAD
Your .wad should be in the wads\ folder of a Mod. File name is 8.3 because of SHAR reasons

doomgeneric (and DOOM) isn't really designed for this use, so not everything will work smoothly yet
Some features (Mouse support, etc) are missing

As a general note DOOM playback just stops SHAR, so don't expect anything from SHAR to carry over
unless explicitly done so.

## System Requirements

- A PC that can launch SHAR and Lucas' Mod Launcher.
- An extra 4 MB of RAM over what SHAR needs
- An extra 8 MB of disk space over what SHAR needs

## DOOM Features

- Based on Chocolate DOOM, which is very close to Vanilla DOOM (get it?)
- Can play DOOM, DOOM II or other IWADs
- Reading an IWAD from the SHAR VFS (including Mod Launcher mods!)
- Respects the "No Audio" Hack

## Limitations

- Mouse is not supported
- PWADs are not supported yet because PlayFMV can only accept an 8.3 file name.
- The controls are the default DOOM controls. In theory you can edit the doom config file to remap them (or use chocolate-setup I think)
- The DOOM files (saves and config) are put in the SHAR folder (probably UAC virtualised)
- DOOM doesn't really clean up properly because it's expecting the EXE to end. This means some memory leaks every time you quit DOOM in SHAR.
  If you reload DOOM a lot you might crash SHAR or something, but really you did it to yourself
- Likewise, relaunching DOOM may cause oddities as not everything is reset properly (lots of static stuff)

## Code Credits

- doomgeneric by ozkl
    - Chocolate Doom by Simon Howard
    - DOOM by Id Software
- SDL2 by the Sam Lantinga, SDL Community
- minhook by Tsuda Kageyu
- libpng by the PNG Development Group
- zlib by Jean-loup Gailly and Mark Adler
