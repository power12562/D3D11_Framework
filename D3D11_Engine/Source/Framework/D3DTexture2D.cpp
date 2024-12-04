#include "D3DTexture2D.h"
#include <Framework/TextureManager.h>

D3DTexture2D::~D3DTexture2D()
{
	for (std::wstring& path : pathList)
	{
		if (path != L"")
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
	ResetTexture2D(index);
	textureManager.CreateSharingTexture(path, &SRVList[index]);
	pathList[index] = path;
}

void D3DTexture2D::SetOneTexture(int index)
{
	ResetTexture2D(index);
	SRVList[index] = textureManager.GetOneTexture();
}

void D3DTexture2D::SetZeroTexture(int index)
{
	ResetTexture2D(index);
	SRVList[index] = textureManager.GetZeroTexture();
}

void D3DTexture2D::SetDefaultNormalTexture(int index)
{
	ResetTexture2D(index);
	SRVList[index] = textureManager.GetDefaultNormalTexture();
}

void D3DTexture2D::ResetTexture2D(int index)
{
	if (SRVList[index])
	{
		if(pathList[index] != L"")
			textureManager.ReleaseSharingTexture(pathList[index].c_str());

		SRVList[index] = nullptr;
		pathList[index].clear();
	}
}

D3DTexture2D::D3DTexture2D(const D3DTexture2D& rhs)
{
	if (this == &rhs)
		return;

	SRVList = rhs.SRVList;
	pathList = rhs.pathList;
	for (int i = 0; i < rhs.pathList.size(); ++i)
	{
		if(pathList[i] != L"")
			textureManager.CreateSharingTexture(pathList[i].c_str(), &SRVList[i]);
	}
}

D3DTexture2D& D3DTexture2D::operator=(const D3DTexture2D& rhs)
{
	if (this == &rhs)
		return *this;
	
	SRVList = rhs.SRVList;
	pathList = rhs.pathList;
	for (int i = 0; i < rhs.pathList.size(); ++i)
	{
		if (pathList[i] != L"")
			textureManager.CreateSharingTexture(pathList[i].c_str(), &SRVList[i]);
	}

	return *this;
}
