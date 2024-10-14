#include "D3D11Utility.h"
#include <comdef.h>
#include <d3dcompiler.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

namespace Utility
{
	LPCWSTR GetComErrorString(HRESULT hr)
	{
		_com_error err(hr);
		LPCWSTR errMsg = err.ErrorMessage();
		return errMsg;
	}

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
				pErrorBlob->Release();
			}
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	HRESULT LoadShadeFormFile(const WCHAR* szFileName, ID3DBlob** ppBlobOut)
	{
		HRESULT hr{};
		hr = D3DReadFileToBlob(szFileName, ppBlobOut);
		if (FAILED(hr))
		{
			_com_error err(hr);
			MessageBox(NULL, err.ErrorMessage(), L"LoadShadeFormFile", MB_OK);
			return hr;
		}
		return S_OK;
	}

	HRESULT CreateTextureFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
	{
		HRESULT hr = S_OK;

		// Load the Texture
		hr = DirectX::CreateDDSTextureFromFile(d3dDevice, szFileName, texture, textureView);
		if (FAILED(hr))
		{
			hr = DirectX::CreateWICTextureFromFile(d3dDevice, szFileName, texture, textureView);
			if (FAILED(hr))
			{
				MessageBoxW(NULL, GetComErrorString(hr), szFileName, MB_OK);
				return hr;
			}
		}
		return S_OK;
	}

    DXGI_FORMAT GetDXGIFormat(D3D_REGISTER_COMPONENT_TYPE componentType, UINT mask)
    {
        switch (componentType)
        {
        case D3D_REGISTER_COMPONENT_FLOAT32:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R32_FLOAT;            // 1 己盒 (R)
            case 0b0011:  return DXGI_FORMAT_R32G32_FLOAT;         // 2 己盒 (RG)
            case 0b0111:  return DXGI_FORMAT_R32G32B32_FLOAT;      // 3 己盒 (RGB)
            case 0b1111:  return DXGI_FORMAT_R32G32B32A32_FLOAT;   // 4 己盒 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_UINT32:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R32_UINT;             // 1 己盒 (R)
            case 0b0011:  return DXGI_FORMAT_R32G32_UINT;          // 2 己盒 (RG)
            case 0b0111:  return DXGI_FORMAT_R32G32B32_UINT;       // 3 己盒 (RGB)
            case 0b1111:  return DXGI_FORMAT_R32G32B32A32_UINT;    // 4 己盒 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_SINT32:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R32_SINT;             // 1 己盒 (R)
            case 0b0011:  return DXGI_FORMAT_R32G32_SINT;          // 2 己盒 (RG)
            case 0b0111:  return DXGI_FORMAT_R32G32B32_SINT;       // 3 己盒 (RGB)
            case 0b1111:  return DXGI_FORMAT_R32G32B32A32_SINT;    // 4 己盒 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_FLOAT16:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R16_FLOAT;            // 1 己盒 (R)
            case 0b0011:  return DXGI_FORMAT_R16G16_FLOAT;         // 2 己盒 (RG)
            case 0b1111:  return DXGI_FORMAT_R16G16B16A16_FLOAT;   // 4 己盒 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_UINT16:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R16_UINT;             // 1 己盒 (R)
            case 0b0011:  return DXGI_FORMAT_R16G16_UINT;          // 2 己盒 (RG)
            case 0b1111:  return DXGI_FORMAT_R16G16B16A16_UINT;    // 4 己盒 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_SINT16:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R16_SINT;             // 1 己盒 (R)
            case 0b0011:  return DXGI_FORMAT_R16G16_SINT;          // 2 己盒 (RG)
            case 0b1111:  return DXGI_FORMAT_R16G16B16A16_SINT;    // 4 己盒 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }
        default:
            return DXGI_FORMAT_UNKNOWN;
        }
    }

}

