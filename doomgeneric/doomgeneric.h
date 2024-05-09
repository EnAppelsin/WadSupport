#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include <stdlib.h>
#include <stdint.h>

#ifndef DOOMGENERIC_RESX
#define DOOMGENERIC_RESX 640
#endif  // DOOMGENERIC_RESX

#ifndef DOOMGENERIC_RESY
#define DOOMGENERIC_RESY 400
#endif  // DOOMGENERIC_RESY


#ifdef CMAP256

typedef uint8_t pixel_t;

#else  // CMAP256

typedef uint32_t pixel_t;

#endif  // CMAP256


extern pixel_t* DG_ScreenBuffer;
extern int DG_Exited;

void doomgeneric_Create(int argc, char **argv);
void doomgeneric_Tick();


//Implement below functions for your platform
void DG_Init();
void DG_DrawFrame();
void DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs();
int DG_GetKey(int* pressed, unsigned char* key);
void DG_SetWindowTitle(const char * title);
void DG_LoadFrame(void* buffer, int size);

void DG_Cleanup();

typedef void* (*callback_open_t)(char* path, unsigned int* length);
typedef void (*callback_close_t)(void* user);
typedef int (*callback_read_t)(void* user, unsigned int offset, void* buffer, size_t bufferlen);
void DG_IOCallbacks(callback_open_t o, callback_close_t c, callback_read_t r);

#endif //DOOM_GENERIC
