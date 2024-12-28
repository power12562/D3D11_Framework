#pragma once
#include <Utility/D3D11Utility.h>
#include <string>
#include <atomic>

namespace E_TEXTURE
{
	//Texture2D albedoTexture : register(t0);
	//Texture2D normalTexture : register(t1);
	//Texture2D specularMap : register(t2);
	//Texture2D emissiveTexture : register(t3);
	//Texture2D opacityTexture : register(t4);
	//Texture2D metalnessTexture : register(t5);
	//Texture2D roughnessTexture : register(t6);
	//Texture2D ambientOcculusionTexture : register(t7);

	//TextureCube Diffuse_IBL_Texture : register(t9);
	//TextureCube Specular_IBL_Texture : register(t10);
	//Texture2D BRDF_LUT : register(t11);

	enum TYPE
	{
		//TEXTURE ������.
		Albedo,
		Normal,
		Specular,
		Emissive,
		Opacity,
		Metalness,
		Roughness,
		AmbientOcculusion,
		
		//SkyBox ������.
		Diffuse_IBL,
		Specular_IBL,
		BRDF_LUT,
		Null,
	};

	constexpr int BlingPhongTextureCount = Opacity + 1;
	constexpr int PBRTextureCount = AmbientOcculusion + 1;
}

namespace E_TEXTURE_DEFAULT
{
	enum DEFAULT_TEXTURE
	{
		ZERO,
		ONE,
		CUBE_ZERO,
	};
}

class D3DTexture2D
{
	static const size_t MAX_SIZE = 128;
public:
	D3DTexture2D();
	~D3DTexture2D();	

	void resize(size_t newsize);
	inline constexpr size_t size() { return SRVList.size(); }
	inline ID3D11ShaderResourceView** data() { return SRVList.data(); }

	void SetCubeMapTexture(int index, const wchar_t* path);
	void SetTexture2D(int index, const wchar_t* path);
	void SetDefaultTexture(int index, E_TEXTURE_DEFAULT::DEFAULT_TEXTURE type);
	void ResetTexture2D(int index);

	/*SRV�� ��ȯ�մϴ�. ������ nullptr ��ȯ.*/
	ID3D11ShaderResourceView* operator[](int index);

	/*���� ������ path�� �ٽ� �ε�*/
	void ReloadTexture();
	/**���� ������ Path*/
	inline std::wstring& GetPath(int index) { return pathList[index]; }
public:
	D3DTexture2D(const D3DTexture2D& rhs);
	D3DTexture2D& operator=(const D3DTexture2D& rhs);

private:
	std::vector<ID3D11ShaderResourceView*> SRVList{};
	std::vector<std::wstring> pathList{};
};

