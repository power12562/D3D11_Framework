#include "TextureManager.h"
#include <D3DCore/D3DRenderer.h>
#include <Math/Mathf.h>
#include <Utility/ImguiHelper.h>
#include <Manager/SceneManager.h>

TextureManager& textureManager = TextureManager::GetInstance();

using namespace Utility;

static void CompressPopup()
{
	// �˾��� ���� ���� �ݵ�� OpenPopup ȣ��
	ImGui::OpenPopup("Compress Texture");

	// ��� �˾� ����
	if (ImGui::BeginPopupModal("Compress Texture"))
	{



		// Ȯ�� ��ư
		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
			sceneManager.PopImGuiPopupFunc();
		}
		ImGui::EndPopup();
	}
}

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

ULONG TextureManager::CreateSharingCubeMap(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView)
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
		CheckHRESULT(CreateCubeMapFromFile(d3dRenderer.GetDevice(), path, nullptr, &newResource));
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

void TextureManager::CompressTexture()
{
	sceneManager.PushImGuiPopupFunc(CompressPopup);
}

ID3D11ShaderResourceView* TextureManager::GetDefaultTexture(E_TEXTURE_DEFAULT::DEFAULT_TEXTURE texture)
{
	switch (texture)
	{
	case E_TEXTURE_DEFAULT::ZERO:
		return GetZeroTexture();
	case E_TEXTURE_DEFAULT::ONE:
		return GetOneTexture();
	case E_TEXTURE_DEFAULT::CUBE_ZERO:
		return GetCubeZeroTexture();
	default:
		return GetZeroTexture();
	}
}

void TextureManager::ReleaseDefaultTexture()
{
	Utility::SafeRelease(oneTexture);
	Utility::SafeRelease(zeroTexture);
	Utility::SafeRelease(cubeZeroTexture);
}

void TextureManager::CreateDefaultTexture(const float(&pixel)[4], ID3D11ShaderResourceView** ppSRV)
{
	if (ID3D11Device* device = d3dRenderer.GetDevice())
	{
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

		if (texture)
		{	
			D3D_SET_OBJECT_NAME(texture, L"TextureManager");
			Utility::CheckHRESULT(device->CreateShaderResourceView(texture, nullptr, ppSRV));

			const wchar_t* objName = pixel[0] > Mathf::Epsilon ? L"Default One" : L"Default Zero";
			D3D_SET_OBJECT_NAME(*ppSRV, objName);
		}
		
		texture->Release(); // �ؽ�ó�� ��� �� ����
	}
}

void TextureManager::CreateDefaultCubeTexture(const float(&pixel)[4], ID3D11ShaderResourceView** ppSRV)
{
	if (ID3D11Device* device = d3dRenderer.GetDevice())
	{
		// �ؽ�ó ���� ����
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = 1;
		textureDesc.Height = 1;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ؽ�ó �����͸� �������� ����
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // Shader���� ���
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		// ���긮�ҽ� ������ ���� (�ȼ� �� ����)
		D3D11_SUBRESOURCE_DATA initData[6] = {};
		for (auto& i : initData)
		{
			i.pSysMem = pixel;
			i.SysMemPitch = sizeof(pixel);
		}
	
		// �ؽ�ó ����
		ID3D11Texture2D* texture = nullptr;
		Utility::CheckHRESULT(device->CreateTexture2D(&textureDesc, initData, &texture));

		if (texture)
		{
			D3D_SET_OBJECT_NAME(texture, L"TextureManager");

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = textureDesc.MipLevels;
			Utility::CheckHRESULT(device->CreateShaderResourceView(texture, &srvDesc, ppSRV));

			const wchar_t* objName = pixel[0] > Mathf::Epsilon ? L"Default Cube One" : L"Default Cube Zero";
			D3D_SET_OBJECT_NAME(*ppSRV, objName);
		}

		texture->Release(); // �ؽ�ó�� ��� �� ����
	}
}

ID3D11ShaderResourceView* TextureManager::GetOneTexture()
{
	if (oneTexture)
		return oneTexture;
	else
	{
		constexpr float pixel[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		CreateDefaultTexture(pixel, &oneTexture);
		return oneTexture;
	}
}

ID3D11ShaderResourceView* TextureManager::GetZeroTexture()
{
	if (zeroTexture)
		return zeroTexture;
	else
	{
		constexpr float pixel[4] = { 0.f,0.f,0.f,0.f };
		CreateDefaultTexture(pixel, &zeroTexture);
		return zeroTexture;
	}
}

ID3D11ShaderResourceView* TextureManager::GetCubeZeroTexture()
{
	if (cubeZeroTexture)
		return cubeZeroTexture;
	else
	{
		constexpr float pixel[4] = { 0.f,0.f,0.f,0.f };
		CreateDefaultCubeTexture(pixel, &cubeZeroTexture);
		return cubeZeroTexture;
	}
}

