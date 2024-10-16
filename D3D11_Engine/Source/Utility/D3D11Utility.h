#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <directxtk/simplemath.h>
#include <exception>
#include <cstdio>
#include <wrl.h>
#include <Utility/MemoryUtility.h>
using namespace Microsoft::WRL;

namespace Utility
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		const char* what() const noexcept override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};
	
	/** HRESULT 확인 후 에러시 exception 객체를 throw 합니다. */
	inline void CheckHRESULT(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}

	LPCWSTR GetComErrorString(HRESULT hr);
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT LoadShadeFormFile(const WCHAR* szFileName, ID3DBlob** ppBlobOut);
	HRESULT CreateTextureFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);

	DXGI_FORMAT GetDXGIFormat(D3D_REGISTER_COMPONENT_TYPE componentType, UINT mask);
}

struct DRAW_INDEX_DATA
{
	// 버텍스 버퍼.
	ID3D11Buffer* pVertexBuffer = nullptr;	
	// 버텍스 하나의 크기.
	unsigned int vertexBufferStride = 0;	
	// 버텍스 버퍼의 오프셋.
	unsigned int vertexBufferOffset = 0;					

	// 인덱스 버퍼.
	ID3D11Buffer* pIndexBuffer = nullptr;	
	// 인덱스 개수.
	int indicesCount = 0;									
};