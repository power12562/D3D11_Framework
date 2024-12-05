#include "SimpleMeshRender.h"
#include <Framework\D3DRenderer.h>
#include <Material\BlingPhongMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Framework\ResourceManager.h>

SimpleMeshRender::SimpleMeshRender()
{
	
}

SimpleMeshRender::~SimpleMeshRender()
{
	using namespace Utility;
	if (meshResource.use_count() == 1)
	{
		SafeRelease(meshResource->pVertexBuffer);
		SafeRelease(meshResource->pIndexBuffer);
	}
}

void SimpleMeshRender::Start()
{
	using namespace Utility;

	wchar_t materialName[50]{};
	swprintf_s(materialName, L"%s (%d)", gameObject.Name.c_str(), gameObject.GetInstanceID());

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerState.resize(1);
	samplerState.SetSamplerState(0, sampDesc);
}

void SimpleMeshRender::FixedUpdate()
{
}

void SimpleMeshRender::Update()
{
}

void SimpleMeshRender::LateUpdate()
{
}

void SimpleMeshRender::Render()
{
	if (!meshResource->pVertexBuffer || !meshResource->pIndexBuffer)
	{
		__debugbreak(); //데이터 없음.
		return;
	}
	
    const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
    if (IsVSShader() && IsPSShader())
    {
		constBuffer.UpdateEvent();
		RENDERER_DRAW_DESC desc = GetRendererDesc();
        d3dRenderer.DrawIndex(desc, false);
    } 
}

void SimpleMeshRender::CreateMesh()
{
	using namespace Utility;
	if (vertices.empty() || indices.empty())
		return;

	if (meshResource->pIndexBuffer)
		SafeRelease(meshResource->pIndexBuffer);

	if (meshResource->pVertexBuffer)
		SafeRelease(meshResource->pVertexBuffer);

	meshResource->vertexBufferOffset = 0;
	meshResource->vertexBufferStride = sizeof(Vertex);
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &meshResource->pVertexBuffer));
	
	meshResource->indicesCount = indices.size();
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &meshResource->pIndexBuffer));

	vertices.clear();
	indices.clear();
}
