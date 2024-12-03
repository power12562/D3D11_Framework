#include "D3DTexture2D.h"
#include <Framework/TextureManager.h>

D3DTexture2D::~D3DTexture2D()
{
	for (std::wstring& path : pathList)
	{
		textureManager.ReleaseSharingTexture(path.c_str());
	}
}

void D3DTexture2D::resize(size_t newsize)
{
	if (newsize < D3DTexture2D::MAX_SIZE)
	{
		SRVList.resize(newsize);
		pathList.resize(newsize);
	}
	else
	{
		__debugbreak();
		//사이즈 제한 초과
	}
}

void D3DTexture2D::SetTexture2D(int index, const wchar_t* path)
{
	textureManager.CreateSharingTexture(path, &SRVList[index]);
	pathList[index] = path;
}

void D3DTexture2D::ResetTexture2D(int index)
{
	textureManager.ReleaseSharingTexture(pathList[index].c_str());
	SRVList[index] = nullptr;
	pathList[index].clear();
}
