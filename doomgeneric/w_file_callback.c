//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	WAD I/O functions.
//

#include <stdio.h>

#include "m_misc.h"
#include "w_file.h"
#include "z_zone.h"

typedef struct
{
    wad_file_t wad;
    void *user;
} callback_wad_file_t;

extern wad_file_class_t callback_wad_file;
int callback_valid = 0;

typedef void* (*callback_open_t)(const char* path, unsigned int* length);
typedef void (*callback_close_t)(void* user);
typedef int (*callback_read_t)(void* user, unsigned int offset, void* buffer, size_t bufferlen);

callback_open_t callback_open_func;
callback_close_t callback_close_func;
callback_read_t callback_read_func;

void DG_IOCallbacks(callback_open_t o, callback_close_t c, callback_read_t r)
{
    if (o && c && r) {
        callback_open_func = o;
        callback_close_func = c;
        callback_read_func = r;
        callback_valid = 1;
    }
    else {
        callback_valid = 0;
    }
}

static wad_file_t *W_Callback_OpenFile(const char *path)
{
    callback_wad_file_t *result;
    void* user;
    unsigned int length = 0;

    user = callback_open_func(path, &length);

    if (user == NULL)
    {
        return NULL;
    }

    // Create a new stdc_wad_file_t to hold the file handle.

    result = Z_Malloc(sizeof(callback_wad_file_t), PU_STATIC, 0);
    result->wad.file_class = &callback_wad_file;
    result->wad.mapped = NULL;
    result->wad.length = length;
    result->user = user;

    return &result->wad;
}

static void W_Callback_CloseFile(wad_file_t *wad)
{
    callback_wad_file_t* callback_wad;

    callback_wad = (callback_wad_file_t*) wad;

    callback_close_func(callback_wad->user);
    Z_Free(callback_wad);
}

// Read data from the specified position in the file into the 
// provided buffer.  Returns the number of bytes read.

size_t W_Callback_Read(wad_file_t *wad, unsigned int offset,
                   void *buffer, size_t buffer_len)
{
    callback_wad_file_t* callback_wad;
    size_t result;

    callback_wad = (callback_wad_file_t*)wad;

    result = callback_read_func(callback_wad->user, offset, buffer, buffer_len);

    return result;
}


wad_file_class_t callback_wad_file = 
{
    W_Callback_OpenFile,
    W_Callback_CloseFile,
    W_Callback_Read,
};


