#include "SimpleMaterial.h"
#include <Framework\HLSLManager.h>
#include <Framework\D3DRenderer.h>
#include <Framework\TextureManager.h>
#include <Light/SimpleDirectionalLight.h>

using namespace E_TEXTURE_INDEX;

SimpleMaterial::SimpleMaterial()
{
	using namespace Utility;

	int index = -1;
	index = cbuffer.CreatePSConstantBuffers<cb_Material>();
	cbuffer.BindUpdateEvent(cb_material);

	index = cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	cbuffer.BindUpdateEvent(SimpleDirectionalLight::cb_Light);

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CheckHRESULT(d3dRenderer.GetDevice()->CreateSamplerState(&sampDesc, &pSamplerLinear));
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
		pPixelShader = nullptr;
		currPS.clear();
	}
}

void SimpleMaterial::SetDiffuse(const wchar_t* path)
{
	if (texturesNames[Diffuse] != path)
	{
		ResetDiffuse();
		textureManager.CreateSharingTexture(path, &textures[Diffuse]);
		texturesNames[Diffuse] = path;
	}
}

void SimpleMaterial::ResetDiffuse()
{
	if (!texturesNames[Diffuse].empty())
	{
		textureManager.ReleaseSharingTexture(texturesNames[Diffuse].c_str());
		textures[Diffuse] = nullptr;
		texturesNames[Diffuse].clear();
	}
}

void SimpleMaterial::SetNormalMap(const wchar_t* path)
{
	if (texturesNames[Normal] != path)
	{
		ResetNormalMap();
		textureManager.CreateSharingTexture(path, &textures[Normal]);
		texturesNames[Normal] = path;
	}
}

void SimpleMaterial::ResetNormalMap()
{
	if (!texturesNames[Normal].empty())
	{
		textureManager.ReleaseSharingTexture(texturesNames[Normal].c_str());
		textures[Normal] = nullptr;
		texturesNames[Normal].clear();
	}
}

void SimpleMaterial::SetSpecularMap(const wchar_t* path)
{
	if (texturesNames[Specular] != path)
	{
		ResetSpecularMap();
		textureManager.CreateSharingTexture(path, &textures[Specular]);
		texturesNames[Specular] = path;
	}
}

void SimpleMaterial::ResetSpecularMap()
{
	if (!texturesNames[Specular].empty())
	{
		textureManager.ReleaseSharingTexture(texturesNames[Specular].c_str());
		textures[Specular] = nullptr;
		texturesNames[Specular].clear();
	}
}

void SimpleMaterial::SetEmissiveMap(const wchar_t* path)
{
	if (texturesNames[Emissive] != path)
	{
		ResetEmissiveMap();
		textureManager.CreateSharingTexture(path, &textures[Emissive]);
		texturesNames[Emissive] = path;
	}
}

void SimpleMaterial::ResetEmissiveMap()
{
	if (!texturesNames[Emissive].empty())
	{
		textureManager.ReleaseSharingTexture(texturesNames[Emissive].c_str());
		textures[Emissive] = nullptr;
		texturesNames[Emissive].clear();
	}
}

void SimpleMaterial::SetOpacityMap(const wchar_t* path)
{
	if (texturesNames[Opacity] != path)
	{
		ResetOpacityMap();
		textureManager.CreateSharingTexture(path, &textures[Opacity]);
		texturesNames[Opacity] = path;
	}
}

void SimpleMaterial::ResetOpacityMap()
{
	if (!texturesNames[Opacity].empty())
	{
		textureManager.ReleaseSharingTexture(texturesNames[Opacity].c_str());
		textures[Opacity] = nullptr;
		texturesNames[Opacity].clear();
	}
}

