#include "SimpleMaterial.h"
#include <Framework\HLSLManager.h>

SimpleMaterial::SimpleMaterial()
{
	int index = -1;
	index = cbuffer.CreatePSConstantBuffers<cb_Light>();
	index = cbuffer.CreatePSConstantBuffers<cb_Material>();
}

SimpleMaterial::~SimpleMaterial()
{
	ResetVS();
	ResetPS();
}

void SimpleMaterial::SetVS(const wchar_t* path)
{
	if (currVS != path)
	{
		ResetVS();
		hlslManager.CreateSharingShader(path, shaderModel.c_str(), &pVertexShader, &pInputLayout);
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
		hlslManager.CreateSharingShader(path, shaderModel.c_str(), &pPixelShader);
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

