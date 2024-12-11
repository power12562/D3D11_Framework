#pragma once
#include <Utility/D3D11Utility.h>
#include <string>

namespace E_TEXTURE
{
	constexpr int BlingPhongTextureCount = 5;

	//Texture2D albedoTexture : register(t0);
	//Texture2D normalTexture : register(t1);
	//Texture2D specularMap : register(t2);
	//Texture2D emissiveTexture : register(t3);
	//Texture2D opacityTexture : register(t4);
	//Texture2D metalnessTexture : register(t5);
	//Texture2D roughnessTexture : register(t6);
	//Texture2D RMACTexture : register(t7);
	//Texture2D ambientOcculusionTexture : register(t8);

	enum TEXTURE_INDEX
	{
		Albedo,
		Normal,
		Specular,
		Emissive,
		Opacity,
		Metalness,
		Roughness,
		RMACTexture,
		AmbientOcculusion,
		Null
	};
}

namespace E_TEXTURE_DEFAULT
{
	enum DEFAULT_TEXTURE
	{
		ZERO,
		ONE
	};
}

class D3DTexture2D
{
	static const size_t MAX_SIZE = 128;
public:
	D3DTexture2D();
	~D3DTexture2D();	

	void resize(size_t newsize);
	constexpr size_t size() { return SRVList.size(); }

	void SetCubeMapTexture(int index, const wchar_t* path);
	void SetTexture2D(int index, const wchar_t* path);
	void SetDefaultTexture(int index, E_TEXTURE_DEFAULT::DEFAULT_TEXTURE type);
	void ResetTexture2D(int index);

	ID3D11ShaderResourceView* operator[](int index) { return SRVList[index]; }
public:
	D3DTexture2D(const D3DTexture2D& rhs);
	D3DTexture2D& operator=(const D3DTexture2D& rhs);

private:
	std::vector<ID3D11ShaderResourceView*> SRVList{};
	std::vector<std::wstring> pathList{};
};

