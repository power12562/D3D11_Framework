#include "TextureManager.h"
#include <Framework\D3DRenderer.h>

TextureManager& textureManager = TextureManager::GetInstance();

using namespace Utility;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

ULONG TextureManager::CreateSharingTexture(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView)
{
	auto findIter = resourceMap.find(path);
	if (findIter != resourceMap.end())
	{
		*ppOut_ResourceView = findIter->second;
		return findIter->second->AddRef();
	}
	else
	{
		ID3D11ShaderResourceView* newResource;
		CheackHRESULT(CreateTextureFromFile(d3dRenderer.GetDevice(), path, nullptr, &newResource));
		resourceMap[path] = newResource;
		*ppOut_ResourceView = newResource;
		return 1;
	}
}

ULONG TextureManager::ReleaseSharingTexture(const wchar_t* path)
{
	auto findIter = resourceMap.find(path);
	if (findIter != resourceMap.end())
	{
		ULONG refcount = findIter->second->Release();
		if (refcount == 0)
		{
			resourceMap.erase(path);
		}
		return 	refcount;
	}
	else
	{
		__debugbreak; //Å°°ª x
		return -1;
	}
}
