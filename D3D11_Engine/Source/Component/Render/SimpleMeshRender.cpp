#include "SimpleMeshRender.h"
#include <Framework\D3DRenderer.h>
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Framework\MaterialManager.h>

SimpleMeshRender::SimpleMeshRender()
{
	if (Material == nullptr)
	{
		Material = materialManager.GetMaterial(L"Standard");
	}
	Material->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
}

SimpleMeshRender::~SimpleMeshRender()
{
	using namespace Utility;
	SafeRelease(meshData.pVertexBuffer);
	SafeRelease(meshData.pIndexBuffer);
}

void SimpleMeshRender::Start()
{
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
    const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
    if (Material)
    {
		Material->cbuffer.UpdateConstBuffer(SimpleDirectionalLight::cb_Light);
        Material->cbuffer.UpdateConstBuffer(Material->cb_material);
        d3dRenderer.DrawIndex(meshData, *Material);
    } 
}

void SimpleMeshRender::CreateMesh()
{
	using namespace Utility;
	if (vertices.empty() || indices.empty())
		return;

	if (meshData.pIndexBuffer)
		SafeRelease(meshData.pIndexBuffer);

	if (meshData.pVertexBuffer)
		SafeRelease(meshData.pVertexBuffer);

	meshData.vertexBufferOffset = 0;
	meshData.vertexBufferStride = sizeof(Vertex);
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &meshData.pVertexBuffer));
	
	meshData.indicesCount = indices.size();
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &meshData.pIndexBuffer));
}
