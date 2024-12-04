#pragma once
#include <Utility\D3D11Utility.h>
#include <Framework\TSingleton.h>
#include <unordered_map>
#include <string>
#include <minwindef.h>
#include <Material/SimpleMaterial.h>

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
	ID3D11ShaderResourceView* GetDefaultTexture(SimpleMaterial::TEXTURE_INDEX texture);
	ID3D11ShaderResourceView* GetOneTexture();
	ID3D11ShaderResourceView* GetDefaultNormalTexture();
	ID3D11ShaderResourceView* GetZeroTexture();
	void ReleaseDefaultTexture();

private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> resourceMap;

	void CreateDefaultTexture(const float (&pixel)[4], ID3D11ShaderResourceView** ppSRV);

	ID3D11ShaderResourceView* oneTexture = nullptr;
	ID3D11ShaderResourceView* defaultNormalTexture = nullptr;
	ID3D11ShaderResourceView* zeroTexture = nullptr;
};
