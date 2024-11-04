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
	void CreateSharingShader(const wchar_t* path, const char* shaderModel, ID3D11VertexShader** ppOut_VertexShader, ID3D11InputLayout** ppOut_InputLayout);
	void CreateSharingShader(const wchar_t* path, const char* shaderModel, ID3D11PixelShader** ppOutput);

	/*사용중인 Shader 객체들을 Release 합니다. 씬 소멸자에서 호출됩니다.*/
	void ClearSharingShader();
private:
	std::map<std::wstring, ID3D11DeviceChild*> sharingShaderMap;
	std::map<std::wstring, ID3D11InputLayout*> sharingInputLayoutMap;

private:
	EXTENSION_TYPE ChackShaderFile(const wchar_t* extension);

private:
	HLSLManager();
	~HLSLManager();
};

