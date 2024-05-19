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

## How to Install

- Download a release from https://github.com/EnAppelsin/WadSupport/releases
- Copy the DLLs folder to your Mod Launcher directory (merging with the DLLs folder there)
- Put the Hacks folder in your Mod Launcher settings directory, usually Documents\My Games\Lucas' Simpsons Hit & Run Mod Launcher
- To use the demonstration mod copy the Mods folder to the same directory

## How to Use

To use the Hack in your own Mod for whatever reason, just try to play an FMV like normal ending in ".wad"
Where the wad must be a valid DOOM IWAD and must be in the wads/ folder in SHAR's Virtual File System
(e.g. CustomFiles/wads/ in your Mod)

For more flexibility, including the ability to specify PWADS ("mods") you can use a ".dcl" file
This is a text file with one doom command line argument per line, e.g. to play a regular PWAD:

```
-iwad
wads\doom.wad
-file
wads\coolmod.wad
```

You can even use Lua to generate the .dcl, see the example mod (DoomInShar)

(For the really adventurous you can even use Lua to generate the .wad)

## How to Use (Mod)

There is an example Mod "DoomInShar", just load "Doom in SHAR" like any other mod.
You can choose which doom WADs to play in the Mod Settings.
The example comes with the free shareware DOOM WAD, if you want to play other versions you will need to
put your own WADs in CustomFiles/wads/

Drive to the Kwik-e-Mart in level1 and play the DOOM arcade machine to play DOOM! Then question why you did it

## System Requirements

- A PC that can launch SHAR and Lucas' Mod Launcher.
- An extra 16 MB of RAM over what SHAR needs
- An extra 16 MB of disk space over what SHAR needs

## DOOM Features

- Based on Chocolate DOOM, which is very close to Vanilla DOOM (get it?)
- Can play DOOM, DOOM II or other IWADs
- Reading an IWAD from the SHAR VFS (including Mod Launcher mods!)
- Respects the "No Audio" Hack
- The DOOM files (saves and config) are put in the SHAR save folder

## Limitations

- The controls are the default DOOM controls. ~~In theory you can edit the doom config file to remap them (doomgeneric has this patched out atm)~~
- DOOM doesn't really clean up properly because it's expecting the EXE to end. This means some memory leaks every time you quit DOOM in SHAR.
  If you reload DOOM a lot you might crash SHAR or something, but really you did it to yourself
- Likewise, relaunching DOOM may cause oddities as not everything is reset properly (lots of static stuff)

## Future Plans

- Suport some basic configuration as Hack settings (such as WASD)
- Test/improve mouse support
- ???

## Code Credits

- doomgeneric by ozkl
    - Chocolate Doom by Simon Howard
    - DOOM by Id Software
- SDL2 by the Sam Lantinga, SDL Community
- minhook by Tsuda Kageyu
- libpng by the PNG Development Group
- zlib by Jean-loup Gailly and Mark Adler
