#include "pch.h"
#include "Rad_RadFile.h"
#include <cassert>

static constexpr size_t MAX_DOOM_FILES = 10;
static rad::IRadFile* doomFiles[MAX_DOOM_FILES] = { 0 };
static unsigned int doomFileCounter = 0;

void* SharDoomOpen(char* path, unsigned int* length)
{
	assert(doomFileCounter < 10);
	rad::IRadFile* file;
	rad::FileOpenSync(&file, path, false);
	if (file)
	{
		file->GetSizeSync(length);
		for (size_t i = 0; i < MAX_DOOM_FILES; i++)
		{
			if (doomFiles[i] == 0)
			{
				doomFiles[i] = file;
				break;
			}
		}
		doomFileCounter++;
	}
	return file;
}

void SharDoomClose(void* user)
{
	rad::IRadFile* file = (rad::IRadFile*)user;
	for (size_t i = 0; i < MAX_DOOM_FILES; i++)
	{
		if (doomFiles[i] == file)
		{
			doomFiles[i] = 0;
			break;
		}
	}
	doomFileCounter--;
	file->Release();
}

int SharDoomRead(void* user, unsigned int offset, void* buffer, size_t bufferlen)
{
	rad::IRadFile* file = (rad::IRadFile*)user;
	file->SetPositionSync(offset);
	file->ReadSync(buffer, bufferlen);
	return bufferlen;
}

void SharDoomCloseAll()
{
	for (size_t i = 0; i < MAX_DOOM_FILES; i++)
	{
		if (doomFiles[i] != 0)
		{
			doomFiles[i]->Release();
			doomFiles[i] = 0;
		}
	}
	doomFileCounter = 0;
}
