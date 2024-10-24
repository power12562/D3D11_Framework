#include "TextureManager.h"
#include <Framework\D3DRenderer.h>

TextureManager& textureManager = TextureManager::GetInstance();

using namespace Utility;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

ULONG TextureManager::CreateSharingTexture(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView)
{
	auto findIter = resourceMap.find(path);
	if (findIter != resourceMap.end())
	{
		*ppOut_ResourceView = findIter->second;
		return findIter->second->AddRef();
	}
	else
	{
		ID3D11ShaderResourceView* newResource;
		CheckHRESULT(CreateTextureFromFile(d3dRenderer.GetDevice(), path, nullptr, &newResource));
		resourceMap[path] = newResource;
		*ppOut_ResourceView = newResource;
		return 1;
	}
}

ULONG TextureManager::ReleaseSharingTexture(const wchar_t* path)
{
	auto findIter = resourceMap.find(path);
	if (findIter != resourceMap.end())
	{
		ULONG refcount = findIter->second->Release();
		if (refcount == 0)
		{
			resourceMap.erase(path);
		}
		return 	refcount;
	}
	else
	{
		__debugbreak(); //Ű�� x
		return -1;
	}
}

ID3D11ShaderResourceView* TextureManager::Get1x1Texture()
{
	if (noneTexture)
		return noneTexture;
	else
	{
		ID3D11Device* device = d3dRenderer.GetDevice();

		// 1x1 �ؽ�ó ������ (RGBA ��, ���⼭�� ��� ä���� 1)
		const float pixel[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // R, G, B, A ���� ��� 1

		// �ؽ�ó ���� ����
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = 1;
		textureDesc.Height = 1;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 32-bit float ����
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ؽ�ó �����͸� �������� ����
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // Shader���� ���

		// ���긮�ҽ� ������ ���� (�ȼ� �� ����)
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = pixel;
		initData.SysMemPitch = sizeof(pixel);

		// �ؽ�ó ����
		ID3D11Texture2D* texture = nullptr;
		Utility::CheckHRESULT(device->CreateTexture2D(&textureDesc, &initData, &texture));

		Utility::CheckHRESULT(device->CreateShaderResourceView(texture, nullptr, &noneTexture));
		texture->Release(); // �ؽ�ó�� ��� �� ����
	}
}
