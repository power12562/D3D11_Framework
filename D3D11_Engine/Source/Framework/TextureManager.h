#pragma once
#include <Utility\D3D11Utility.h>
#include <Framework\TSingleton.h>
#include <unordered_map>
#include <string>
#include <minwindef.h>

class TextureManager;
extern TextureManager& textureManager;

class TextureManager : public TSingleton<TextureManager>
{
private:
	friend class TSingleton;
	TextureManager();
	~TextureManager();

public:
	ULONG CreateSharingTexture(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView);
	ULONG ReleaseSharingTexture(const wchar_t* path);

public:
	ID3D11ShaderResourceView* Get1x1Texture();

private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> resourceMap;
	ID3D11ShaderResourceView* noneTexture = nullptr;
};
