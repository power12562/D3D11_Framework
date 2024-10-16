#include "SimpleMaterial.h"
#include <Framework\HLSLManager.h>
#include <Framework\D3DRenderer.h>
#include <Framework\TextureManager.h>

using namespace E_TEXTURE_INDEX;

SimpleMaterial::SimpleMaterial()
{
	using namespace Utility;

	int index = -1;
	index = cbuffer.CreatePSConstantBuffers<cb_Material>();

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CheackHRESULT(d3dRenderer.GetDevice()->CreateSamplerState(&sampDesc, &pSamplerLinear));
}

SimpleMaterial::~SimpleMaterial()
{
	ResetVS();
	ResetPS();

	ResetDiffuse();
	ResetNormalMap();
	ResetSpecularMap();
	ResetEmissiveMap();
	ResetOpacityMap();

	Utility::SafeRelease(pSamplerLinear);
}

void SimpleMaterial::SetVS(const wchar_t* path)
{
	if (currVS != path)
	{
		ResetVS();
		hlslManager.CreateSharingShader(path, ("vs_" + shaderModel).c_str(), &pVertexShader, &pInputLayout);
		currVS = path;
	}
}

void SimpleMaterial::ResetVS()
{
	if (!currVS.empty())
	{
		hlslManager.ReleaseSharingShader(currVS.c_str());
		pVertexShader = nullptr;
		pInputLayout = nullptr;
		currVS.clear();
	}
}

void SimpleMaterial::SetPS(const wchar_t* path)
{
	if (currPS != path)
	{
		ResetPS();
		hlslManager.CreateSharingShader(path, ("ps_" + shaderModel).c_str(), &pPixelShader);
		currPS = path;
	}
}

void SimpleMaterial::ResetPS()
{
	if (!currPS.empty())
	{
		hlslManager.ReleaseSharingShader(currPS.c_str());
		pPixelShader = nullptr;
		currPS.clear();
	}
}

void SimpleMaterial::SetDiffuse(const wchar_t* path)
{
	if (currTexture != path)
	{
		ResetDiffuse();
		textureManager.CreateSharingTexture(path, &textures[Diffuse]);
		currTexture = path;
	}
}

void SimpleMaterial::ResetDiffuse()
{
	if (!currTexture.empty())
	{
		textureManager.ReleaseSharingTexture(currTexture.c_str());
		textures[Diffuse] = nullptr;
		currTexture.clear();
	}
}

void SimpleMaterial::SetNormalMap(const wchar_t* path)
{
	if (currNormal != path)
	{
		ResetNormalMap();
		textureManager.CreateSharingTexture(path, &textures[Normal]);
		currNormal = path;
	}
}

void SimpleMaterial::ResetNormalMap()
{
	if (!currNormal.empty())
	{
		textureManager.ReleaseSharingTexture(currNormal.c_str());
		textures[Normal] = nullptr;
		currNormal.clear();
	}
}

void SimpleMaterial::SetSpecularMap(const wchar_t* path)
{
	if (currSpecular != path)
	{
		ResetSpecularMap();
		textureManager.CreateSharingTexture(path, &textures[Specular]);
		currSpecular = path;
	}
}

void SimpleMaterial::ResetSpecularMap()
{
	if (!currSpecular.empty())
	{
		textureManager.ReleaseSharingTexture(currSpecular.c_str());
		textures[Specular] = nullptr;
		currSpecular.clear();
	}
}

void SimpleMaterial::SetEmissiveMap(const wchar_t* path)
{
	if (currEmissive != path)
	{
		ResetEmissiveMap();
		textureManager.CreateSharingTexture(path, &textures[Emissive]);
		currEmissive = path;
	}
}

void SimpleMaterial::ResetEmissiveMap()
{
	if (!currEmissive.empty())
	{
		textureManager.ReleaseSharingTexture(currEmissive.c_str());
		textures[Emissive] = nullptr;
		currEmissive.clear();
	}
}

void SimpleMaterial::SetOpacityMap(const wchar_t* path)
{
	if (currOpacity != path)
	{
		ResetOpacityMap();
		textureManager.CreateSharingTexture(path, &textures[Opacity]);
		currOpacity = path;
	}
}

void SimpleMaterial::ResetOpacityMap()
{
	if (!currOpacity.empty())
	{
		textureManager.ReleaseSharingTexture(currOpacity.c_str());
		textures[Opacity] = nullptr;
		currOpacity.clear();
	}
}

