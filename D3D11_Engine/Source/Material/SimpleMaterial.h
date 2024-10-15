#pragma once
#include <Framework\D3DConstBuffer.h>
#include <directxtk/simplemath.h>

struct cb_Light
{
	DirectX::SimpleMath::Vector4 LightDir{ 0, -1, 1, 0 };
	DirectX::SimpleMath::Vector4 LightDiffuse{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Vector4 LightAmbient{ 0.01, 0.01, 0.01, 0.01 };
	DirectX::SimpleMath::Vector4 LightSpecular{ 1, 1, 1, 1 };
};

struct cb_Material
{
	DirectX::SimpleMath::Vector4 MaterialAmbient{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Vector4 MaterialDiffuse{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Vector4 MaterialSpecular{ 1, 1, 1, 1 };
	float MaterialSpecularPower{ 300 };
	DirectX::SimpleMath::Vector3 MaterialSpecularPad;
};

class SimpleMaterial
{
public:
	SimpleMaterial();
	virtual ~SimpleMaterial();

public:
	void SetVS(const wchar_t* path);
	void ResetVS();

	void SetPS(const wchar_t* path);
	void ResetPS();

public:
	std::string shaderModel = "vs_4_0";

public:
	D3DConstBuffer cbuffer;

	cb_Light Light;
	cb_Material Material;

private:
	std::wstring currVS;
	std::wstring currPS;

private:
	ID3D11InputLayout* pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;	
	ID3D11PixelShader* pPixelShader = nullptr;	

};