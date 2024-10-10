#include "HLSLManager.h"
#include <Framework\D3DRenderer.h>
#include <_Debug\Console.h>
#include <intrin.h>

HLSLManager& hlslManager = HLSLManager::GetInstance();

using namespace Utility;

HLSLManager::HLSLManager()
{

}

HLSLManager::~HLSLManager()
{
	if (!sharingShaderMap.empty())
	{
		__debugbreak(); //sharingShaderMap refCounter Error
	}
}

HLSLManager::EXTENSION_TYPE HLSLManager::ChackShaderFile(const wchar_t* extension)
{
	if (wcsstr(extension, L".hlsl"))
	{
		return EXTENSION_TYPE::hlsl;
	}
	else if (wcsstr(extension, L".cso"))
	{
		return EXTENSION_TYPE::cso;
	}
	else
	{
		return EXTENSION_TYPE::null;
	}
}

ULONG HLSLManager::ReleaseSharingShader(const wchar_t* path)
{
	auto findIter = sharingShaderMap.find(path);
	if (findIter != sharingShaderMap.end())
	{
		ULONG refcount = findIter->second->Release();
		if (refcount == 0)
		{
			sharingShaderMap.erase(findIter);
		}
		return refcount;
	}
	else
	{
		Debug_printf("ReleaseSharingShader Error. �������� �ʴ� Ű���Դϴ�.\n");
		return -1;
	}
}

template<>
ID3D11VertexShader* HLSLManager::CreateSharingShader(const wchar_t* path, const char* shaderModel)
{
	auto findIter = sharingShaderMap.find(path);
	if (findIter != sharingShaderMap.end())
		return dynamic_cast<ID3D11VertexShader*>(findIter->second);
	else
	{
		ID3D10Blob* vertexShaderBuffer = nullptr;	// ���� ���̴� �ڵ尡 ����� ����.
		ID3D11VertexShader* vertexShader = nullptr;	// ���� ���̴��� ����� ��.
		EXTENSION_TYPE type = ChackShaderFile(path);
		switch (type)
		{
		case HLSLManager::EXTENSION_TYPE::hlsl:	
			CheackHRESULT(CompileShaderFromFile(path, "main", shaderModel, &vertexShaderBuffer));
			break;
		case HLSLManager::EXTENSION_TYPE::cso:
			CheackHRESULT(LoadShadeFormFile(path, &vertexShaderBuffer));
			break;
		case HLSLManager::EXTENSION_TYPE::null:
			__debugbreak(); //not shader file
			return nullptr;
		}
		CheackHRESULT(d3dRenderer.GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader));
		sharingShaderMap[path] = vertexShader;
	}
}

template<>
ID3D11PixelShader* HLSLManager::CreateSharingShader(const wchar_t* path, const char* shaderModel)
{
	auto findIter = sharingShaderMap.find(path);
	if (findIter != sharingShaderMap.end())
		return dynamic_cast<ID3D11PixelShader*>(findIter->second);
	else
	{
		ID3D10Blob* pixelShaderBuffer = nullptr;	// �ȼ� ���̴� �ڵ尡 ����� ����.
		ID3D11PixelShader* pixelShader = nullptr;	// �ȼ� ���̴��� ����� ��.
		EXTENSION_TYPE type = ChackShaderFile(path);
		switch (type)
		{
		case HLSLManager::EXTENSION_TYPE::hlsl:
			CheackHRESULT(CompileShaderFromFile(path, "main", shaderModel, &pixelShaderBuffer));
			break;
		case HLSLManager::EXTENSION_TYPE::cso:
			CheackHRESULT(LoadShadeFormFile(path, &pixelShaderBuffer));
			break;
		case HLSLManager::EXTENSION_TYPE::null:
			__debugbreak(); //not shader file
			return nullptr;
		}
		CheackHRESULT(d3dRenderer.GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader));
		sharingShaderMap[path] = pixelShader;
	}
}

