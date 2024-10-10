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
	template<typename ShaderType>
	ShaderType* CreateSharingShader(const wchar_t* path, const char* shaderModel);
	template<>
	ID3D11VertexShader* CreateSharingShader<ID3D11VertexShader>(const wchar_t* path, const char* shaderModel);
	template<>
	ID3D11PixelShader* CreateSharingShader<ID3D11PixelShader>(const wchar_t* path, const char* shaderModel);

	ULONG ReleaseSharingShader(const wchar_t* path);
private:
	std::map<std::wstring, ID3D11DeviceChild*> sharingShaderMap;

private:
	EXTENSION_TYPE ChackShaderFile(const wchar_t* extension);

private:
	HLSLManager();
	~HLSLManager();
};

template<typename ShaderType>
inline ShaderType* HLSLManager::CreateSharingShader(const wchar_t* path, const char* shaderModel)
{
	static_assert(false, "is not ShaderType");
}
