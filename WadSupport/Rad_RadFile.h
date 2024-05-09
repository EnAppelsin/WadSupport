#pragma once

#include "Rad_IRefCount.h"

namespace rad
{
	enum radMemorySpace
	{
		radMemoryNull = 0,
		radMemoryMain = 1
	};

	class IRadFile : public IRefCount
	{
	public:
		virtual void ReadAsync(void* pBuffer, unsigned int bytesToRead, radMemorySpace pBufferSpace = radMemoryMain) = 0;
		virtual void WriteAsync(const void* pBuffer, unsigned int bytesToWrite, radMemorySpace pBufferSpace = radMemoryMain) = 0;
		virtual void SetPositionAsync(unsigned int position) = 0;
		virtual void GetPositionAsync(unsigned int* pPosition) = 0;
		virtual void GetSizeAsync(unsigned int* pSize) = 0;
		virtual void CommitAsync(void) = 0;

		virtual void ReadSync(void* pBuffer, unsigned int bytesToRead, radMemorySpace pBufferSpace = radMemoryMain) = 0;
		virtual void WriteSync(const void* pBuffer, unsigned int bytesToWrite, radMemorySpace pBufferSpace = radMemoryMain) = 0;
		virtual void SetPositionSync(unsigned int position) = 0;
		virtual void GetPositionSync(unsigned int* pPosition) = 0;
		virtual void GetSizeSync(unsigned int* pSize) = 0;
		virtual void CommitSync(void) = 0;
	};

	void FileOpenSync(IRadFile** pFile, const char* pFileName, bool write = false, int flags = 0, int priority = 1, unsigned int cachesize = 0, int allocator = 1, int cacheSpace = 1);
}