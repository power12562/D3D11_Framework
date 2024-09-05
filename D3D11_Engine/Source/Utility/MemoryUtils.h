#pragma once

namespace Utility
{
	template<typename T>
	void SafeRelease(T& p)
	{
		if (p)
		{
			p->Release();
			p = nullptr;
		}
	}

	template<typename T>
	void SafeDelete(T& p)
	{
		if (p)
		{
			delete p;
			p = nullptr;
		}
	}
}