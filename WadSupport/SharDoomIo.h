#pragma once

void* SharDoomOpen(char* path, unsigned int* length);
void SharDoomClose(void* user);
int SharDoomRead(void* user, unsigned int offset, void* buffer, size_t bufferlen);
void SharDoomCloseAll();