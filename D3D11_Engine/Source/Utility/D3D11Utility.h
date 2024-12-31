#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <directxtk/simplemath.h>
#include <exception>
#include <cstdio>
#include <wrl.h>
#include <memory>
#include <string>
#include <Utility/MemoryUtility.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <directxtk/PrimitiveBatch.h>
#include <directxtk/VertexTypes.h>
#include <directxtk/Effects.h>
#include <directxtk/DirectXHelpers.h>
#include <DirectXTex.h>
using namespace Microsoft::WRL;

namespace Utility
{
	namespace E_COMPRESS
	{
		enum TYPE
		{
			DXT1,
			DXT5,
			BC1 = DXT1,
			BC3 = DXT5,
			BC4,
			BC5,
			BC6,
			BC7,

			MAX
		};
	};

	enum class DX_TEXTURE_EXTENSION
	{
		tga,
		dds,
		null
	};

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
	
	/** HRESULT Ȯ�� �� ������ exception ��ü�� throw �մϴ�. */
	inline void CheckHRESULT(HRESULT hr)
	{
		if (FAILED(hr))
		{
			com_exception ex(hr);
			__debugbreak(); //���� �߻�
			throw ex;
		}
	}

	LPCWSTR GetComErrorString(HRESULT hr);
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT LoadShadeFormFile(const WCHAR* szFileName, ID3DBlob** ppBlobOut);
	HRESULT CreateTextureFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
	HRESULT CreateCubeMapFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
	std::shared_ptr<DirectX::ScratchImage> CreateCompressTexture(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView, E_COMPRESS::TYPE type);
	HRESULT SaveTextureForDDS(const wchar_t* szFileName, const std::shared_ptr<DirectX::ScratchImage>& image);
	UINT GetMipmapLevels(UINT width, UINT height);
	DXGI_FORMAT GetDXGIFormat(D3D_REGISTER_COMPONENT_TYPE componentType, UINT mask);

	bool XMMatrixIsNaN(const DirectX::XMMATRIX& matrix);

	/*ī�޶� ��ķ� �������� �ڳʸ� ����մϴ�.*/
	void CalculateBoundsFrustumCorners(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, DirectX::XMFLOAT3(&out_frustumCorners)[DirectX::BoundingFrustum::CORNER_COUNT]);
	/*View ��Ʈ������ ���������� ���� BundingBox�� ����մϴ�.*/
	DirectX::BoundingBox CalculateBoundsLightSpace(const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMFLOAT3(&frustumCorners)[DirectX::BoundingFrustum::CORNER_COUNT]);
	/*Bounding Box�� OrthographicProjection �����մϴ�..*/
	DirectX::XMMATRIX CreateOrthographicProjection(const DirectX::BoundingBox& boundingBox, float nearPlane, float farPlane);

	/*�� �� �������� ��� �޼� -> ������ ��ǥ��� ��ȯ�ϴ� �Լ�.*/
	void ConvertLHtoRHViewAndProj(const DirectX::XMMATRIX& viewLH, const DirectX::XMMATRIX& projLH, float* viewRH_m16, float* projRH_m16);

	DX_TEXTURE_EXTENSION GetTexureExtension(const std::wstring& flieString);

	/*���ؽ� ���� ���� �б�*/
	void RetrieveVertexBufferData(ID3D11DeviceContext* context, ID3D11Device* device, ID3D11Buffer* vertexBuffer, void* outputData, size_t bufferSize);
	/*�ε��� ���� ���� �б�*/
	void RetrieveIndexBufferData(ID3D11DeviceContext* context, ID3D11Device* device, ID3D11Buffer* indexBuffer, void* outputData, size_t bufferSize);
}

struct DRAW_INDEX_DATA
{
	// ���ؽ� ����.
	ID3D11Buffer* pVertexBuffer = nullptr;	
	// ���ؽ� �ϳ��� ũ��.
	unsigned int vertexBufferStride = 0;	
	// ���ؽ� ������ ������.
	unsigned int vertexBufferOffset = 0;					

	// �ε��� ����.
	ID3D11Buffer* pIndexBuffer = nullptr;	
	// �ε��� ����.
	int indicesCount = 0;									
};

using DRAW_INDEX_RESOURCE = std::shared_ptr<DRAW_INDEX_DATA>;

#ifdef _DEBUG
#define D3D_SET_OBJECT_NAME(pObject, pName) D3D_SET_OBJECT_NAME_N_W(pObject, (UINT)wcslen(pName), pName)
#else
#define D3D_SET_OBJECT_NAME(pObject, pName)
#endif

namespace DebugDraw
{
	void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::BoundingSphere& sphere,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::BoundingBox& box,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::BoundingOrientedBox& obb,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::BoundingFrustum& frustum,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawGrid(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis,
		DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs,
		DirectX::GXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawRing(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR origin, DirectX::FXMVECTOR majorAxis, DirectX::FXMVECTOR minorAxis,
		DirectX::GXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawRay(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR origin, DirectX::FXMVECTOR direction, bool normalize = true,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawTriangle(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR pointA, DirectX::FXMVECTOR pointB, DirectX::FXMVECTOR pointC,
		DirectX::GXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawQuad(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR pointA, DirectX::FXMVECTOR pointB,
		DirectX::FXMVECTOR pointC, DirectX::GXMVECTOR pointD,
		DirectX::HXMVECTOR color = DirectX::Colors::White);
}