#pragma once

namespace rad
{
	// Basic Reference Counted Wrapper
	class IRefCount
	{
	public:
		virtual void AddRef() = 0;
		virtual void Release() = 0;
	};
}