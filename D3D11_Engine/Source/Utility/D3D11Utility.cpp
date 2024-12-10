#include "D3D11Utility.h"
#include <comdef.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>
#include <iostream>
#include <filesystem>

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
        auto ShowErrorMessageBox = [&]()
            {
                MessageBoxW(NULL, GetComErrorString(hr), szFileName, MB_OK);
            };

        std::filesystem::path filePath = szFileName;
        DX_TEXTURE_EXTENSION extension = GetTexureExtension(filePath.extension().wstring());


        switch (extension)
        {
		case Utility::DX_TEXTURE_EXTENSION::tga:
		{
			DirectX::TexMetadata metaData;
			DirectX::ScratchImage scratchImage;
			hr = DirectX::LoadFromTGAFile(szFileName, &metaData, scratchImage);
			if (SUCCEEDED(hr))
			{
				if (texture)
				{
					hr = DirectX::CreateTexture(d3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), metaData, texture);
					if (FAILED(hr))
					{
						ShowErrorMessageBox();
						return hr;
					}
				}
				if (textureView)
				{
					hr = DirectX::CreateShaderResourceView(d3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), metaData, textureView);
					if (FAILED(hr))
					{
						ShowErrorMessageBox();
						return hr;
					}
				}
			}
			else
			{
				ShowErrorMessageBox();
				return hr;
			}
		}
            break;

        case Utility::DX_TEXTURE_EXTENSION::dds:
            hr = DirectX::CreateDDSTextureFromFile(d3dDevice, szFileName, texture, textureView); 
            if (FAILED(hr))
            {
                ShowErrorMessageBox();
                return hr;
            }
            break;

        default:
            hr = DirectX::CreateWICTextureFromFile(d3dDevice, szFileName, texture, textureView);
            if (FAILED(hr))
            {
                ShowErrorMessageBox();
                return hr;
            }
            break;
        }
		return S_OK;
	}

    HRESULT CreateCubeMapFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
    {
        using namespace DirectX;

        HRESULT hr = S_OK;
        auto ShowErrorMessageBox = [&]()
            {
                MessageBoxW(NULL, GetComErrorString(hr), szFileName, MB_OK);
            };

        std::filesystem::path filePath = szFileName;
        DX_TEXTURE_EXTENSION extension = GetTexureExtension(filePath.extension().wstring());

        switch (extension)
        {
        case Utility::DX_TEXTURE_EXTENSION::dds:
        {
            TexMetadata metadata;
            ScratchImage scratchImage;
            ID3D11Texture2D* tempTexture = nullptr;
            hr = LoadFromDDSFile(szFileName, DDS_FLAGS_FORCE_RGB, &metadata, scratchImage);
            if (FAILED(hr))
            {
                ShowErrorMessageBox();
                return hr;
            }

            // 큐브맵인지 확인
            if (!(metadata.miscFlags & TEX_MISC_TEXTURECUBE))
            {
                MessageBoxW(NULL, L"Cube Map 형식이 아닙니다.", szFileName, MB_OK);
                return E_FAIL; // DDS 파일이 큐브맵 형식이 아님
            }

            // 텍스처 생성    
            D3D11_TEXTURE2D_DESC desc = {};
            const Image* image = scratchImage.GetImages();
            desc.Width = image->width;
            desc.Height = image->height;
            desc.MipLevels = GetMipmapLevels(desc.Width, desc.Height);
            desc.ArraySize = 6;
            desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
            desc.SampleDesc.Count = 1;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
            desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
            if (desc.MipLevels == 0) 
            {
                desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
                desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
            }

            hr = d3dDevice->CreateTexture2D(&desc, nullptr, &tempTexture);
            if (FAILED(hr))
            {
                ShowErrorMessageBox();
                return hr; // 텍스처 생성 실패
            }

            // Shader Resource View 생성
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
            srvDesc.TextureCube.MostDetailedMip = 0;
            srvDesc.TextureCube.MipLevels = -1;
            if (textureView)
            {
                hr = d3dDevice->CreateShaderResourceView(tempTexture, &srvDesc, textureView);
                if (FAILED(hr))
                {
                    ShowErrorMessageBox();
                    return hr; // 텍스처 생성 실패
                }
            }       
            if (texture)
            {
                *texture = tempTexture;
            }
            else
            {
                tempTexture->Release();
            }
            return hr;
        }
        default:
            MessageBoxW(NULL, L"Cube Map은 dds 파일만 사용 가능 합니다.", szFileName, MB_OK);
            return S_FALSE;
        }
    }

    UINT GetMipmapLevels(UINT width, UINT height)
    {
        UINT levels = 1;
        while ((width | height) >> levels) {
            ++levels;
        }
        return levels;
    }

    DXGI_FORMAT GetDXGIFormat(D3D_REGISTER_COMPONENT_TYPE componentType, UINT mask)
    {
        switch (componentType)
        {
        case D3D_REGISTER_COMPONENT_FLOAT32:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R32_FLOAT;            // 1 성분 (R)
            case 0b0011:  return DXGI_FORMAT_R32G32_FLOAT;         // 2 성분 (RG)
            case 0b0111:  return DXGI_FORMAT_R32G32B32_FLOAT;      // 3 성분 (RGB)
            case 0b1111:  return DXGI_FORMAT_R32G32B32A32_FLOAT;   // 4 성분 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_UINT32:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R32_UINT;             // 1 성분 (R)
            case 0b0011:  return DXGI_FORMAT_R32G32_UINT;          // 2 성분 (RG)
            case 0b0111:  return DXGI_FORMAT_R32G32B32_UINT;       // 3 성분 (RGB)
            case 0b1111:  return DXGI_FORMAT_R32G32B32A32_UINT;    // 4 성분 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_SINT32:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R32_SINT;             // 1 성분 (R)
            case 0b0011:  return DXGI_FORMAT_R32G32_SINT;          // 2 성분 (RG)
            case 0b0111:  return DXGI_FORMAT_R32G32B32_SINT;       // 3 성분 (RGB)
            case 0b1111:  return DXGI_FORMAT_R32G32B32A32_SINT;    // 4 성분 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_FLOAT16:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R16_FLOAT;            // 1 성분 (R)
            case 0b0011:  return DXGI_FORMAT_R16G16_FLOAT;         // 2 성분 (RG)
            case 0b1111:  return DXGI_FORMAT_R16G16B16A16_FLOAT;   // 4 성분 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_UINT16:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R16_UINT;             // 1 성분 (R)
            case 0b0011:  return DXGI_FORMAT_R16G16_UINT;          // 2 성분 (RG)
            case 0b1111:  return DXGI_FORMAT_R16G16B16A16_UINT;    // 4 성분 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }

        case D3D_REGISTER_COMPONENT_SINT16:
        {
            switch (mask)
            {
            case 0b0001:  return DXGI_FORMAT_R16_SINT;             // 1 성분 (R)
            case 0b0011:  return DXGI_FORMAT_R16G16_SINT;          // 2 성분 (RG)
            case 0b1111:  return DXGI_FORMAT_R16G16B16A16_SINT;    // 4 성분 (RGBA)
            default: return DXGI_FORMAT_UNKNOWN;
            }
        }
        default:
            return DXGI_FORMAT_UNKNOWN;
        }
    }

    bool XMMatrixIsNaN(const DirectX::XMMATRIX& matrix)
    {
        return DirectX::XMVector4IsNaN(matrix.r[0]) || DirectX::XMVector4IsNaN(matrix.r[1]) ||
               DirectX::XMVector4IsNaN(matrix.r[2]) || DirectX::XMVector4IsNaN(matrix.r[3]);
    }

    DX_TEXTURE_EXTENSION GetTexureExtension(const std::wstring& flieString)
    {
        if (flieString.compare(L".tga") == 0)
            return DX_TEXTURE_EXTENSION::tga;
        else if (flieString.compare(L".dds") == 0)
            return DX_TEXTURE_EXTENSION::dds;
        else
            return DX_TEXTURE_EXTENSION::null;
    }
}

