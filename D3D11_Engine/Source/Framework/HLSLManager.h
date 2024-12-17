#pragma once
#include <Framework\TSingleton.h>
#include <d3d11.h>
#include <map>
#include <string>

class HLSLManager;
extern HLSLManager& hlslManager;   

class HLSLManager : public TSingleton<HLSLManager>
{
	enum class EXTENSION_TYPE
	{
		hlsl,
		cso,
		null,
	};
	friend class TSingleton;
public:
	static constexpr wchar_t EngineShaderPath[] = L"Resource/EngineShader/";

public:
	/** ���ؽ� ���̴� �� �Է� ���̾ƿ� ����.*/
	void CreateSharingShader(const wchar_t* path, const char* shaderModel, ID3D11VertexShader** ppOut_VertexShader, ID3D11InputLayout** ppOut_InputLayout);
	/** �ȼ� ���̴� ����.*/
	void CreateSharingShader(const wchar_t* path, const char* shaderModel, ID3D11PixelShader** ppOutput);

	/*������� Shader ��ü���� Release �մϴ�. �� �Ҹ��ڿ��� ȣ��˴ϴ�.*/
	void ClearSharingShader();

private:
	std::map<std::wstring, ID3D11DeviceChild*> sharingShaderMap;
	std::map<std::wstring, ID3D11InputLayout*> sharingInputLayoutMap;

private:
	EXTENSION_TYPE ChackShaderFile(const wchar_t* extension);

public:
	/** ���̴��� �����մϴ�. */
	void MakeShader(const wchar_t* path, const char* shaderModel, ID3D11VertexShader** ppOut_VertexShader, ID3D11InputLayout** ppOut_InputLayout);
	/** ���̴��� �����մϴ�. */
	void MakeShader(const wchar_t* path, const char* shaderModel, ID3D11PixelShader** ppOutput);
private:
	HLSLManager();
	~HLSLManager();
};

