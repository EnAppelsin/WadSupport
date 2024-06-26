#pragma once

#include "Rad_IRefCount.h"

namespace rad
{
	enum radMemorySpace
	{
		radMemoryNull = 0,
		radMemoryMain = 1
	};

	enum class FileFlags : int
	{
		OpenExisting = 0,
		OpenOrCreate = 1,
		AlwaysCreate = 3,
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

		virtual unsigned int GetSize() = 0;		
		virtual unsigned int GetSectorSize() = 0;

		virtual void Filler1() = 0;
		virtual void Filler2() = 0;
		virtual void Filler3() = 0;
		virtual void Filler4() = 0;
		virtual void Filler5() = 0;
		virtual void Filler6() = 0;
		
		virtual bool IsOpen() = 0;
		virtual const char* GetFilename() = 0;
		virtual const char* GetDrivename() = 0;

		virtual void Filler7() = 0;
		virtual void Filler8() = 0;
		virtual void Filler9() = 0;
		virtual void Filler10() = 0;

		virtual unsigned int GetHandle() = 0;
	};

	void FileOpenSync(IRadFile** pFile, const char* pFileName, bool write = false, FileFlags flags = FileFlags::OpenExisting, int priority = 1, unsigned int cachesize = 0, int allocator = 1, int cacheSpace = 1);
}