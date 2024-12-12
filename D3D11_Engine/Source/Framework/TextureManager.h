#pragma once
#include <Utility\D3D11Utility.h>
#include <Framework\TSingleton.h>
#include <unordered_map>
#include <string>
#include <minwindef.h>
#include <Material/BlingPhongMaterial.h>
#include <Framework/D3DTexture2D.h>

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
	ULONG CreateSharingCubeMap(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView);
	ULONG ReleaseSharingTexture(const wchar_t* path);

public:
	ID3D11ShaderResourceView* GetDefaultTexture(E_TEXTURE_DEFAULT::DEFAULT_TEXTURE texture);
	void ReleaseDefaultTexture();

private:
	ID3D11ShaderResourceView* GetOneTexture();
	ID3D11ShaderResourceView* GetZeroTexture();
	ID3D11ShaderResourceView* GetCubeZeroTexture();

private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> resourceMap;
	std::unordered_map<std::wstring, ID3D11Texture2D*> cubeTextureMap; //cube map은 Texture 유지해야함

	void CreateDefaultTexture(const float (&pixel)[4], ID3D11ShaderResourceView** ppSRV);
	void CreateDefaultCubeTexture(const float (&pixel)[4], ID3D11ShaderResourceView** ppSRV);
	ID3D11ShaderResourceView* oneTexture = nullptr;
	ID3D11ShaderResourceView* zeroTexture = nullptr;
	ID3D11ShaderResourceView* cubeZeroTexture = nullptr;
};
