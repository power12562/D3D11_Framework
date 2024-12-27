#pragma once
#include <Utility\D3D11Utility.h>
#include <Core/TSingleton.h>
#include <unordered_map>
#include <string>
#include <minwindef.h>
#include <Material/BlingPhongMaterial.h>
#include <D3DCore/D3DTexture2D.h>

class TextureManager;
extern TextureManager& textureManager;

class TextureManager : public TSingleton<TextureManager>
{
private:
	friend class TSingleton;
	TextureManager();
	~TextureManager();

public:
	/**���� �а� �ؽ��� ���.*/
	ULONG CreateSharingTexture(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView);
	/**�ؽ��� ���� �� ���.*/
	ULONG CreateSharingCompressTexture(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView, Utility::E_COMPRESS::TYPE type);
	/**ť��� �ؽ��� ���. dds ť��ʸ� ��� ����*/
	ULONG CreateSharingCubeMap(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView);
	/** �ؽ��� �ݳ�.*/
	ULONG ReleaseSharingTexture(const wchar_t* path);

	/** �ؽ��� ���� ���� ��ȯ. ������ CreateSharing*** �Լ��� ������ �ִ�.*/
	bool IsTextureLoaded(const wchar_t* path);
	/** �Ŵ����� ��ġ�� �ʴ� �ؽ��ĸ� ���.*/
	void InsertTexture(const wchar_t* key, ID3D11ShaderResourceView* pSRV);
	
public:
	ID3D11ShaderResourceView* GetDefaultTexture(E_TEXTURE_DEFAULT::DEFAULT_TEXTURE texture);
	void ReleaseDefaultTexture();

private:
	ID3D11ShaderResourceView* GetOneTexture();
	ID3D11ShaderResourceView* GetZeroTexture();
	ID3D11ShaderResourceView* GetCubeZeroTexture();

private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> resourceMap;

	void CreateDefaultTexture(const float (&pixel)[4], ID3D11ShaderResourceView** ppSRV);
	void CreateDefaultCubeTexture(const float (&pixel)[4], ID3D11ShaderResourceView** ppSRV);
	ID3D11ShaderResourceView* oneTexture = nullptr;
	ID3D11ShaderResourceView* zeroTexture = nullptr;
	ID3D11ShaderResourceView* cubeZeroTexture = nullptr;
};


