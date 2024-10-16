#pragma once
#include <Framework\D3DConstBuffer.h>
#include <directxtk/simplemath.h>
#include <array>

#pragma warning( disable : 4305 )
struct cb_Material
{
	DirectX::SimpleMath::Vector4 MaterialAmbient{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Vector4 MaterialDiffuse{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Vector4 MaterialSpecular{ 1, 1, 1, 1 };
	float MaterialSpecularPower{ 300 };
	DirectX::SimpleMath::Vector3 MaterialSpecularPad;
};
#pragma warning( default : 4305 )

namespace E_TEXTURE_INDEX
{
	enum TEXTUR_INDEX
	{
		Diffuse,
		Normal,
		Specular,
		Emissive,
		Opacity,
		Null
	};
}

class SimpleMaterial
{
	friend class D3DRenderer;
public:
	SimpleMaterial();
	virtual ~SimpleMaterial();

public:
	void SetVS(const wchar_t* path);
	void ResetVS();

	void SetPS(const wchar_t* path);
	void ResetPS();

	void SetDiffuse(const wchar_t* path);
	void ResetDiffuse();

	void SetNormalMap(const wchar_t* path);
	void ResetNormalMap();

	void SetSpecularMap(const wchar_t* path);
	void ResetSpecularMap();

	void SetEmissiveMap(const wchar_t* path);
	void ResetEmissiveMap();

	void SetOpacityMap(const wchar_t* path);
	void ResetOpacityMap();

public:
	std::string shaderModel = "4_0";

public:
	D3DConstBuffer cbuffer;

	cb_Material cb_material;
private:
	std::wstring currVS;
	std::wstring currPS;

	std::wstring currTexture;
	std::wstring currNormal;
	std::wstring currSpecular;
	std::wstring currEmissive;
	std::wstring currOpacity;

private:
	ID3D11InputLayout* pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;	
	ID3D11PixelShader* pPixelShader = nullptr;	

	std::array<ID3D11ShaderResourceView*, E_TEXTURE_INDEX::Null> textures{nullptr, };
private:
	ID3D11SamplerState* pSamplerLinear = nullptr;

public:
	bool IsDiffuse() { return !!textures[E_TEXTURE_INDEX::Diffuse]; }
	bool IsNormalMap() { return !!textures[E_TEXTURE_INDEX::Normal]; }
	bool IsSpecularMap() { return !!textures[E_TEXTURE_INDEX::Specular]; }
	bool IsEmissiveMap() { return !!textures[E_TEXTURE_INDEX::Emissive]; }
	bool IsOpacityMap() { return !!textures[E_TEXTURE_INDEX::Opacity]; }
};