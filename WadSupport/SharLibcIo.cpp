#include "pch.h"
#include "Rad_RadFile.h"
#include "HackFunctions.h"
#include <cassert>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

// Libc-Style Functions that use Shar/Hack Functions
// Used to override some stuff in Doom

static void SharPutLine(char const* msg)
{
	size_t strsize = strlen(msg) + 1;
	wchar_t* wcstring = new wchar_t[strsize];
	size_t converted = 0;
	mbstowcs_s(&converted, wcstring, strsize, msg, _TRUNCATE);
	Hack_Printf(2, 0, wcstring);
	delete[] wcstring;
}

static char LineBuf[80];
static int LineBufPos = 0;

void SharPutc(char msg)
{
	if (msg == '\n' || LineBufPos == 80) {
		SharPutLine(LineBuf);
		LineBufPos = 0;
		LineBuf[0] = '\0';
	}
	LineBuf[LineBufPos++] = msg;
	LineBuf[LineBufPos] = '\0';
}

void SharPuts(const char* msg)
{
	int len = strlen(msg);
	for (int i = 0; i < len; i++)
	{
		SharPutc(msg[i]);
	}
	SharPutc('\n');
}

void SharPrintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int len = _vscprintf(format, args) + 1;
	char* buffer = new char[len];
	vsprintf_s(buffer, len, format, args);
	SharPutLine(buffer);
	delete[] buffer;
}

struct SharLibcFile
{
	rad::IRadFile* file;
};

void* SharFopen(const char* name, const char* mode)
{
	bool write = false;
	bool append = false;
	rad::FileFlags flags = rad::FileFlags::OpenExisting;
	int modelen = strlen(mode);
	switch (mode[0])
	{
	case 'r':
		if (modelen > 1 && mode[1] == '+') {
			write = true;
		}
		else {
			write = false;
		}
		break;
	case 'w':
		write = true;
		flags = rad::FileFlags::AlwaysCreate;
		break;
	case 'a':
		write = true;
		append = true;
		flags = rad::FileFlags::OpenOrCreate;
		break;
	default:
		return NULL;
	}
	rad::IRadFile* fptr = NULL;
	rad::FileOpenSync(&fptr, name, write, flags);
	if (fptr == NULL)
	{
		return NULL;
	}
	SharLibcFile* fp = new SharLibcFile;
	fp->file = fptr;
	if (append) {
		unsigned int size = 0;
		fptr->GetSizeSync(&size);
		fptr->SetPositionSync(size);
	}
	return fp;
}

void SharFclose(void* fpv)
{
	SharLibcFile* fp = (SharLibcFile*)fpv;
	fp->file->Release();
	delete fp;
}

size_t SharFread(void* buffer, size_t size, size_t count, void* fpv)
{
	SharLibcFile* fp = (SharLibcFile*)fpv;
	size_t charSize = size * count;
	fp->file->ReadSync(buffer, charSize);
	return count;
}

size_t SharFwrite(void const* buffer, size_t size, size_t count, void* fpv)
{
	SharLibcFile* fp = (SharLibcFile*)fpv;
	size_t charSize = size * count;
	fp->file->WriteSync(buffer, charSize);
	return count;
}